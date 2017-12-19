#include "HttpServer.h"
#include "InetAddress.h"
#include "HttpResponse.h"
#include "HttpRequest.h"
#include "EventLoop.h"
#include <stdlib.h>

void onRequest(const HttpRequest& req, HttpResponse* resp)
{
	if (req.path() == "/") {
		resp->setStatusCode(HttpResponse::k200Ok);
		resp->setStatusMessage("OK");
		resp->setContentType("text/html");
		resp->setBody("hello http");
	} else if (req.path() == "/pageSwitch.html") {
		resp->setStatusCode(HttpResponse::k200Ok);
		resp->setStatusMessage("OK");
		resp->setContentType("text/html");
		string content;
  		FILE* fp = ::fopen("/home/zm/Desktop/pageSwitch.html", "rb");

		if (fp) {
			const int kBufSize = 1024*1024;
			char iobuf[kBufSize];
			::setbuffer(fp, iobuf, sizeof iobuf);

			char buf[kBufSize];
			size_t nread = 0;
			while ( (nread = ::fread(buf, 1, sizeof buf, fp)) > 0) {
				content.append(buf, nread);
			}
			::fclose(fp);
  		}
  		resp->setBody(content);
	}
}

int main(int argc, char* argv[])
{
	if (argc != 3) {
		return 0;
	}

	EventLoop loop;
	HttpServer server(&loop, InetAddress(argv[1], atoi(argv[2])));
	server.setHttpCallback(onRequest);
	server.start();
	loop.loop();
}

