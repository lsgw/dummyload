#include "HttpContext.h"
#include "Buffer.h"
#include <string.h>
#include <stdlib.h>

HttpContext::HttpContext() :
	state_(kExpectRequestLine)
{

}

bool HttpContext::parseRequest(Buffer* buf)
{
	bool ok = true;
	bool hasMore = true;
	while (hasMore) {
		if (state_ == kExpectRequestLine) {
			const char* crlf = buf->findCRLF();
			//const char* crlf = strstr(buf->peek(), "\r\n");
			if (crlf) {
				ok = processRequestLine(buf->peek(), crlf);
				if (ok) {
					state_ = kExpectHeaders;
					buf->retrieveUntil(crlf+2);
				} else {
					hasMore = false;
				}
				
			} else {
				hasMore = false;
			}
		} else if (state_ == kExpectHeaders) {
			const char* crlf = buf->findCRLF();
			//const char* crlf = strstr(buf->peek(), "\r\n");
			if (crlf) {
				const char* colon = std::find(buf->peek(), crlf, ':');
				if (colon != crlf) {
					request_.setHeader(buf->peek(), colon, crlf);
				} else {
					state_ = kExpectBody;
				}
				buf->retrieveUntil(crlf+2);
			} else {
				hasMore = false;
			}
		} else if (state_ == kExpectBody) {
			string contentLength = request_.header("Content-Length");
			if (contentLength.empty()) {
				state_  = kGotAll;
				hasMore = false;
			} else {
				int bodyLen = atoi(contentLength.c_str());
				if (bodyLen <= buf->readableBytes()) {
					request_.setBody(buf->retrieveAsString(bodyLen));
					state_  = kGotAll;
					hasMore = false;
				}
			}
		}
	}
	
	return ok;
}

bool HttpContext::processRequestLine(const char* begin, const char* end)
{
	bool succeed = false;
	const char* start = begin;
	const char* space = std::find(start, end, ' ');

	if (space != end && request_.setMethod(start, space)) {
		start = space+1;
		space = std::find(start, end, ' ');
		if (space != end) {
			const char* question = std::find(start, space, '?');
			if (question != space) {
				request_.setPath(start, question);
				request_.setQuery(question, space);
			} else {
				request_.setPath(start, space);
			}

			start = space+1;
      		succeed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");
		
			if (succeed) {
				if (*(end-1) == '1') {
					request_.setVersion(HttpRequest::kHttp11);
				} else if (*(end-1) == '0') {
					request_.setVersion(HttpRequest::kHttp10);
				} else {
					succeed = false;
				}
			}
		}
	}
	return succeed;
}
