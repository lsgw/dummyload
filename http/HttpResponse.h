#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>

class Buffer;
class HttpResponse {
public:
	enum HttpStatusCode {
		kUnknown,
		k200Ok = 200,
		k301MovedPermanently = 301,
		k400BadRequest = 400,
		k404NotFound = 404,
	};

	HttpResponse(bool close);
	void appendToBuffer(Buffer* output) const;
	
	void setStatusCode(HttpStatusCode code) { statusCode_ = code; }
	void setStatusMessage(const std::string& message) { statusMessage_ = message; }
	void setContentType(const std::string& contentType) { headers_["Content-Type"] = contentType; }
	void setHeader(const std::string& field, const std::string& value) { headers_[field] = value; }
	void setBody(const std::string& body) { body_ = body; }
	void setCloseConnection(bool on) { closeConnection_ = on; }
	
	bool closeConnection() const { return closeConnection_; }

private:
	bool closeConnection_;
	HttpStatusCode statusCode_;
	std::string statusMessage_; 
	std::map<std::string,std::string> headers_;
	std::string body_;
};

#endif