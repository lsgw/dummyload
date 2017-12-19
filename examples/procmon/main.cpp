#include "HttpServer.h"
#include "InetAddress.h"
#include "HttpResponse.h"
#include "HttpRequest.h"
#include "EventLoop.h"
#include "Imagepng.h"
#include "ReadSmallFile.h"
#include "SystemInfo.h"
#include "jsonxx.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <regex>
#include <list>
#include <map>
#include <vector>

using namespace std;
using namespace std::placeholders;

string readHtmlFile(const char* filename)
{
	string content;
	readFile(filename, 1024*1024, &content);
	return content;
}

class Procmon {
public:
	Procmon(EventLoop* loop, const InetAddress& listenAddr) :
		server_(loop, listenAddr)
	{
		server_.setHttpCallback(std::bind(&Procmon::onRequest, this, _1, _2));
	}
	void start()
	{
		server_.getLoop()->runEvery(2.0, std::bind(&SystemInfo::update, &systemInfo_));
		server_.start();
	}
	void onRequest(const HttpRequest& req, HttpResponse* resp)
	{
		if (req.path() == "/") {
			resp->setStatusCode(HttpResponse::k200Ok);
			resp->setStatusMessage("OK");
			resp->setContentType("text/html");
			resp->setBody(readHtmlFile("phone.html"));
		} else if (req.path() == "/proc") {
			auto tmp = systemInfo_.minProcessData();
			std::vector<MinProcessData> process;

			if (req.query().empty() || req.query() == "?all") {
				for (auto& m : tmp) {
					process.push_back(m.second);
				}
			} else {
				string pidUserCommand = req.query();
				pidUserCommand.erase(0, 1);
				int pid = atoi(pidUserCommand.c_str());
				for (auto& m : tmp) {
					const char* f1 = strstr(m.second.command.c_str(), pidUserCommand.c_str());
					const char* f2 = strstr(m.second.user.c_str(), pidUserCommand.c_str());
					if (m.first == pid || f1 != NULL || f2 != NULL) {
						process.push_back(m.second);
					}
				}
			}

			std::sort(process.begin(), process.end(), [](const MinProcessData& p1, const MinProcessData& p2) {
					int c1 = (int)(p1.cpu * 10000);
					int c2 = (int)(p2.cpu * 10000);
					int m1 = p1.mem;
					int m2 = p2.mem;
					return (c1 > c2) || (c1==c2 && m1>m2);
			});
			int num = 0;
			jsonxx::Array a;
			for (auto& m : process) {
				jsonxx::Object o;
				o << "pid" << m.pid;
				o << "user" << m.user;
				o << "command" << m.command;
				o << "threads" << m.threads;
				o << "mem" << m.mem;
				o << "cpu" << m.cpu;
				a << o;
				if (++num >= 20) {
					break;
				}
			}
			resp->setStatusCode(HttpResponse::k200Ok);
			resp->setStatusMessage("OK");
			resp->setContentType("text/html");
			resp->setBody(a.json());
		} else if (req.path() == "/pid") {
			const char* p = req.query().c_str() + 1;
			int pid = atoi(p);
			auto process = systemInfo_.process();
			auto it = process.find(pid);
			std::list<double> cpu;
			std::list<int> mem;
			if (it != process.end()) {
				cpu = it->second.cpu();
				mem = it->second.mem();
			}
			
			jsonxx::Object o;
			jsonxx::Array a;
			jsonxx::Array b;
			a << 1 - systemInfo_.idleRate();
			b << systemInfo_.memTotal();
			for (auto& c : cpu) {
				a << c;
			}
			for (auto& m : mem) {
				b << m;
			}
			
			o << "cpu" << a;
			o << "mem" << b;

			resp->setStatusCode(HttpResponse::k200Ok);
			resp->setStatusMessage("OK");
			resp->setContentType("text/html");
			resp->setBody(o.json());
		} else if (req.path() == "/host") {
			auto cpu = systemInfo_.cpuRate();
			auto mem = systemInfo_.memRate();
			jsonxx::Object o;
			jsonxx::Array a;
			jsonxx::Array b;
			a << 1 - systemInfo_.idleRate();
			b << systemInfo_.memTotal();
			for (auto& c : cpu) {
				a << c;
			}
			for (auto& m : mem) {
				b << m;
			}

			o << "cpu" << a;
			o << "mem" << b;

			resp->setStatusCode(HttpResponse::k200Ok);
			resp->setStatusMessage("OK");
			resp->setContentType("text/html");
			resp->setBody(o.json());
			
		} else {
			resp->setStatusCode(HttpResponse::k404NotFound);
			resp->setStatusMessage("Not Found");
			resp->setCloseConnection(true);
		}
	}

private:
	HttpServer server_;
	SystemInfo systemInfo_;
};

int main(int argc, char* argv[])
{
	if (argc != 3) return 0;

	EventLoop loop;
	
	Procmon procmon(&loop, InetAddress(argv[1], atoi(argv[2])));
	procmon.start();

	loop.loop();
}