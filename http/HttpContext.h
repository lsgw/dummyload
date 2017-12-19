#ifndef HTTPCONTEXT_H
#define HTTPCONTEXT_H

#include "HttpRequest.h"

class Buffer;
class HttpContext {
public:
	enum HttpRequestParseState {
		kExpectRequestLine,
	    kExpectHeaders,
	    kExpectBody,
	    kGotAll,
	};
	HttpContext();
	bool parseRequest(Buffer* buf);
	bool processRequestLine(const char* begin, const char* end);

	bool gotAll() const { return state_ == kGotAll; }
	HttpRequest& request() { return request_; }

private:
	HttpRequestParseState state_;
	HttpRequest request_;
};

#endif