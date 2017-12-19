#include "Atomic.h"
#include "Condition.h"
#include "CurrentThread.h"
#include "Mutex.h"
#include "Thread.h"
#include "Timestamp.h"
#include "EventLoop.h"
#include "HttpServer.h"
#include "InetAddress.h"
#include "HttpResponse.h"
#include "HttpRequest.h"
#include <stdlib.h>

#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int g_core = 0;
int g_cycles = 0;
int g_percent = 2;
AtomicInt32 g_done;
bool g_busy = false;
Mutex g_mutex;
Condition g_cond(g_mutex);

Mutex g_percent_mutex;

void setGlobalPercent(int percent)
{
  LockGuard guard(g_percent_mutex);
  if (percent>=0 && percent<=100) {
    g_percent = percent;
  }
}

double busy(int cycles)
{
  double result = 0;
  for (int i = 0; i < cycles; ++i)
  {
    result += sqrt(i) * sqrt(i+1);
  }
  return result;
}

double getSeconds(int cycles)
{
  Timestamp start = Timestamp::now();
  busy(cycles);
  return timeDifference(Timestamp::now(), start);
}

void findCycles()
{
  g_cycles = 1000;
  while (getSeconds(g_cycles) < 0.001)
    g_cycles = g_cycles + g_cycles / 4;  // * 1.25
  printf("cycles %d\n", g_cycles);
}

void threadFunc()
{
  while (g_done.get() == 0)
  {
    {
      LockGuard guard(g_mutex);
      while (!g_busy)
        g_cond.wait();
    }
    busy(g_cycles);
  }
  printf("thread exit\n");
}

int getCpuTime();
// this is open-loop control
void load(int percent)
{
  percent = std::max(0, percent);
  percent = std::min(100, percent);

  // Bresenham's line algorithm
  int err = 2*percent - 100;
  int count = 0;

  for (int i = 0; i < 100; ++i)
  {
    bool busy = false;
    if (err > 0)
    {
      busy = true;
      err += 2*(percent - 100);
      ++count;
      // printf("%2d, ", i);
    }
    else
    {
      err += 2*percent;
    }

    {
      LockGuard guard(g_mutex);
      g_busy = busy;
      g_cond.notifyAll();
    }

    CurrentThread::sleepUsec(10*1000); // 10 ms
  }
  assert(count == percent);
}

void fixed()
{
  int su = 0;
  while (true)
  {
    int percent = 0;
    {
      LockGuard guard(g_percent_mutex);
      percent = g_percent;
    }
    int cpuRate = getCpuTime();
    if (cpuRate > percent) {
        su--;
    } else if (cpuRate < percent) {
        su++;
    }
    if (su < 0) {
        su = 0;
    }
    if (su > 100) {
        su = percent;
    }
    load(su);
  }
}

void cosine()
{
  while (true)
    for (int i = 0; i < 200; ++i)
    {
      int percent = 0;
      {
        LockGuard guard(g_percent_mutex);
        percent = g_percent;
      }
      percent = static_cast<int>((1.0 + cos(i * 3.14159 / 100)) / 2 * percent + 0.5);
      load(percent);
    }
}

void sawtooth()
{
  while (true)
    for (int i = 0; i <= 100; ++i)
    {
      int percent = 0;
      {
        LockGuard guard(g_percent_mutex);
        percent = g_percent;
      }
      percent = static_cast<int>(i / 100.0 * percent);
      load(percent);
    }
}

int dummyload(int numThreads, int type)
{
  findCycles();
  numThreads = numThreads<1? 1 : numThreads;
  std::vector<std::unique_ptr<Thread>> threads;
  for (int i = 0; i < numThreads; ++i)
  {
    threads.emplace_back(new Thread(threadFunc));
    threads.back()->start();
  }

  switch (type) {
    case 1: fixed(); break;
    case 2: cosine(); break;
    case 3: sawtooth(); break;
    default: break;
  }

  g_done.getAndSet(1);
  {
    LockGuard guard(g_mutex);
    g_busy = true;
    g_cond.notifyAll();
  }
  for (int i = 0; i < numThreads; ++i)
  {
    threads[i]->join();
  }

  return 0;
}

void onRequest(const HttpRequest& req, HttpResponse* resp)
{
  if (req.path() == "/") {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/html");
    resp->setBody("this is dummyload");
  } else if (req.path() == "/percent" && req.query().size() > 1) {
    const char* p = req.query().c_str() + 1;
    int percent = atoi(p);
    setGlobalPercent(percent);
    resp->setStatusMessage("OK");
    resp->setContentType("text/html");
    resp->setBody("ok: " + std::to_string(percent));
  } else {
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
  }
}

string readFile(const string& path)
{
	int n = 0;
	string tmp;
	char buf[1024];

	int fd = open(path.c_str(), O_RDONLY|O_CLOEXEC);
	if (fd == -1) {
		printf("open %s error\n", path.c_str());
		return tmp;
	}
	while ((n=read(fd, buf, 1024)) > 0) {
		if (n > 0) {
			tmp.append(buf, n);
		} else {
			break;
		}
	}
	close(fd);

	return tmp;
}

int getCpuTime()
{
	
	static int lastIdleTime = 0;
	static int currIdleTime = 0;

	static int lastCpuTime = 0;
	static int currCpuTime = 0;

	string tmp;
	char buf[64];
	int user, nice, system, idle, iowait, irq, softirq, stealstolen, guest;

	tmp = readFile("/proc/stat");
	if (tmp.empty()) {
		return 100;
	}

	sscanf(tmp.c_str(), "%s%d%d%d%d%d%d%d%d%d", buf, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &stealstolen, &guest);

	currIdleTime = idle;
	currCpuTime  = user + nice + system + idle + iowait + irq + softirq + stealstolen + guest;
	

	int i = currIdleTime - lastIdleTime;
	int a = currCpuTime  - lastCpuTime;
	int t = (a - i) * 100 / (double)a;

	lastIdleTime = currIdleTime;
	lastCpuTime  = currCpuTime;
    printf("cpu: %d\n", t);
	if (t < 0) {
		return 0;
	} else if (t > 100) {
		return 100;
	} else {
		return t;
	}
}
int getcpunum() 
{  
        char buf[16] = {0};  
        int num;  
        FILE* fp = popen("cat /proc/cpuinfo |grep processor|wc -l", "r");  
        if(fp) {                                                                                                                                                                               
           fread(buf, 1, sizeof(buf) - 1, fp);  
           pclose(fp);
        }     
        num = atoi(buf);  
        if(num <= 0){  
            num = 1; 
        }     
        return num; 
}

int main(int argc, char* argv[])
{
  if (argc != 3) {
    return 0;
  }
  g_core = getcpunum();
  printf("num core: %d\n", g_core);

  Thread t(std::bind(dummyload, g_core, 1));
  t.start();

  EventLoop loop;
  HttpServer server(&loop, InetAddress(argv[1], atoi(argv[2])));
  server.setHttpCallback(onRequest);
  server.start();
  loop.loop();

  t.join();
}
