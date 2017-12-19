#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <map>

class HttpRequest {
public:
		enum Method { kInvalid, kGet, kPost, kHead, kPut, kDelete };
		enum Version { kUnknown, kHttp10, kHttp11 };

		HttpRequest();
		
		void setVersion(Version v) { version_ = v; }
		bool setMethod(const char* start, const char* end);
		void setPath(const char* start, const char* end) { path_.assign(start, end); }
		void setQuery(const char* start, const char* end) { query_.assign(start, end); }
		void setQuery(const std::string& query) { query_ = query; }
		void setHeader(const char* start, const char* colon, const char* end);
		void setBody(const std::string& body) { body_ = body; }

		const std::string& path() const { return path_; }
		const std::string& query() const { return query_; }
		const std::string& body() const { return body_; }
		
		Version version() const { return version_; }
		Method  method() const { return method_; }

		std::string methodString() const;
		std::string header(const std::string& field) const;

		const std::map<std::string,std::string>& header() { return headers_; }
private:
	Method method_;
	Version version_;
	std::string path_;
	std::string query_;
	std::string body_;
	std::map<std::string, std::string> headers_;
};

#endif