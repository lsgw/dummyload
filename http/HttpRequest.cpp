#include "HttpRequest.h"
#include <assert.h>

using namespace std;

HttpRequest::HttpRequest() :
	method_(kInvalid),
	version_(kUnknown)
{

}

bool HttpRequest::setMethod(const char* start, const char* end)
{
	assert(method_ == kInvalid);
	string m(start, end);

	if (m == "GET") {
		method_ = kGet;
	} else if (m == "POST") {
		method_ = kPost;
	} else if (m == "HEAD") {
		method_ = kHead;
	} else if (m == "PUT") {
		method_ = kPut;
	} else if (m == "DELETE") {
		method_ = kDelete;
	} else {
		method_ = kInvalid;
	}
	return method_ != kInvalid;
}

string HttpRequest::methodString() const
{
	const char* result = "UNKNOWN";
	switch (method_) {
	case kGet: 
		result = "GET";
		break;
	case kPost:
		result = "POST";
		break;
	case kHead:
		result = "HEAD";
		break;
	case kPut:
		result = "PUT";
		break;
	case kDelete:
		result = "DELETE";
		break;
	default:
		break;
	}
	return result;
}

void HttpRequest::setHeader(const char* start, const char* colon, const char* end)
{
	string field(start, colon);
	++colon;
	while (colon < end && isspace(*colon)) {
		++colon;
	}
	string value(colon, end);
	while (!value.empty() && isspace(value[value.size()-1])) {
		value.resize(value.size()-1);
	}
	headers_[field] = value;
}

string HttpRequest::header(const string& field) const
{
	string result;
	std::map<string, string>::const_iterator it = headers_.find(field);
	if (it != headers_.end()) {
		result = it->second;
	}
	return result;
}
