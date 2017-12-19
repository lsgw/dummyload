//http://www.blogjava.net/fjzag/articles/317773.html

#include "SystemInfo.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h> 
#include <sys/types.h>
#include <strings.h>
#include <pwd.h>
#include <sstream>


using namespace std;

int kClockTicksPerSecond = static_cast<int>(::sysconf(_SC_CLK_TCK));


void Process::pushMemory(int mem)
{
	mem_.push_front(mem);
	if (mem_.size() > 512) {
		mem_.pop_back();
	}
}
void Process::pushCpuRate(double rate)
{
	cpu_.push_front(rate);
	if (cpu_.size() > 512) {
		cpu_.pop_back();
	}
}


struct StatData
{
  void parse(const string& procPidStat)
  {
  	string cmd;
    // istringstream is probably not the most efficient way to parse it,
    // see muduo-protorpc/examples/collect/ProcFs.cc for alternatives.
    istringstream iss(procPidStat);

    //            0    1    2    3     4    5       6   7 8 9  11  13   15
    // 3770 (cat) R 3718 3770 3718 34818 3770 4202496 214 0 0 0 0 0 0 0 20
    // 16  18     19      20 21                   22      23      24              25
    //  0 1 0 298215 5750784 81 18446744073709551615 4194304 4242836 140736345340592
    //              26
    // 140736066274232 140575670169216 0 0 0 0 0 0 0 17 0 0 0 0 0 0
    iss >> pid >> cmd;
    iss >> state;
    iss >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags;
    iss >> minflt >> cminflt >> majflt >> cmajflt;
    iss >> utime >> stime >> cutime >> cstime;
    iss >> priority >> nice >> num_threads >> itrealvalue >> starttime;
    long vsize, rss;
    iss >> vsize >> rss >> rsslim;
    vsizeKb = vsize / 1024;
    //rssKb = rss * kbPerPage;
  }
  int pid;
  char state;
  int ppid;
  int pgrp;
  int session;
  int tty_nr;
  int tpgid;
  int flags;

  long minflt;
  long cminflt;
  long majflt;
  long cmajflt;

  long utime;
  long stime;
  long cutime;
  long cstime;

  long priority;
  long nice;
  long num_threads;
  long itrealvalue;
  long starttime;

  long vsizeKb;
  long rssKb;
  long rsslim;
};

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
	string tmp;
	char buf[64];
	int user, nice, system, idle, iowait, irq, softirq, stealstolen, guest;

	tmp = readFile("/proc/stat");
	if (tmp.empty()) {
		return 0;
	}

	sscanf(tmp.c_str(), "%s%d%d%d%d%d%d%d%d%d", buf, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &stealstolen, &guest);

	return user + nice + system + idle + iowait + irq + softirq + stealstolen + guest;
}

SystemInfo::SystemInfo() :
	memTotal_(0),
	memFree_(0),
	memCached_(0),
	memSwapCached_(0),
	systemRate_(0),
	userRate_(0),
	idleRate_(0),
	processNum_(0),
	threadsNum_(0)
{

	update();
}

void SystemInfo::update()
{
	updateMem();
	updateCpu();
	updateProcess();
}

void SystemInfo::updateMem()
{
	const char* p = NULL;
	string tmp;
	char buf[64];

	tmp = readFile("/proc/meminfo");
	if (tmp.empty()) {
		return;
	}

	p = strstr(tmp.c_str(), "MemTotal:");
	sscanf(p, "%s%d", buf, &memTotal_);

	p = strstr(tmp.c_str(), "MemFree:");
	sscanf(p, "%s%d", buf, &memFree_);

	p = strstr(tmp.c_str(), "Buffers:");
	sscanf(p, "%s%d", buf, &memSwapCached_);
	p = strstr(tmp.c_str(), "Cached:");
	sscanf(p, "%s%d", buf, &memCached_);
	memCached_ += memSwapCached_;

	p = strstr(tmp.c_str(), "SwapCached:");
	sscanf(p, "%s%d", buf, &memSwapCached_);

	memRate_.push_front((memTotal_-memFree_)/(double)memTotal_);
	if (memRate_.size() >= 512) {
		memRate_.pop_back();
	}
}
void SystemInfo::updateCpu()
{
	static int lastSysTime = 0;
	static int currSysTime = 0;

	static int lastUserTime = 0;
	static int currUserTime = 0;
	
	static int lastIdleTime = 0;
	static int currIdleTime = 0;

	static int lastCpuTime = 0;
	static int currCpuTime = 0;

	string tmp;
	char buf[64];
	int user, nice, system, idle, iowait, irq, softirq, stealstolen, guest;

	tmp = readFile("/proc/stat");
	if (tmp.empty()) {
		return;
	}

	sscanf(tmp.c_str(), "%s%d%d%d%d%d%d%d%d%d", buf, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &stealstolen, &guest);

	currSysTime  = system;
	currUserTime = user;
	currIdleTime = idle;
	currCpuTime  = user + nice + system + idle + iowait + irq + softirq + stealstolen + guest;
	
	int s = currSysTime  - lastSysTime;
	int u = currUserTime - lastUserTime;
	int i = currIdleTime - lastIdleTime;
	int a = currCpuTime  - lastCpuTime;
	// int a = kClockTicksPerSecond * kPeriod_;


	systemRate_ = s / (double)a;
	userRate_   = u / (double)a;
	idleRate_   = i / (double)a;
	
	cpuRate_.push_front(1.0 - idleRate_);
	if (cpuRate_.size() >= 512) {
		cpuRate_.pop_back();
	}

	// printf("------------------------------\n");
	// printf("cpu = %lf\n", cpuRate_.front());


	lastSysTime  = currSysTime;
	lastUserTime = currUserTime;
	lastIdleTime = currIdleTime;
	lastCpuTime  = currCpuTime;
}


void SystemInfo::updateProcess()
{
	int processNum = 0;
	int threadsNum = 0;
	
	DIR* dirp = opendir("/proc");
	struct dirent* dp = readdir(dirp);
	while ((dp=readdir(dirp)) != NULL) {
		if (isdigit(dp->d_name[0])) {
			const char* p = NULL;
			Process ps;
			MinProcessData minps;
			char name[64];
			char buf[64];
			string tmp;
			int n = 0;

			snprintf(buf, sizeof(buf), "/proc/%s/status", dp->d_name);	
			tmp = readFile(buf);
			if (tmp.empty()) {
				closedir(dirp);
				return ;	
			}
			p = strstr(tmp.c_str(), "Pid:");
			sscanf(p, "%s%d", buf, &n);
			ps.pid = n;
			minps.pid = n;

			p = strstr(tmp.c_str(), "Uid:");
			sscanf(p, "%s%d", buf, &n);
			struct passwd* pa = getpwuid(n);
			ps.user = pa->pw_name;
			minps.user = pa->pw_name;

			p = strstr(tmp.c_str(), "Name:");
			sscanf(p, "%s%s", buf, name);
			ps.command = name;
			minps.command = name;

			p = strstr(tmp.c_str(), "Threads:");
			sscanf(p, "%s%d", buf, &n);
			ps.threads = n;
			minps.threads = n;

			processNum += 1;
			threadsNum += n;

			p = strstr(tmp.c_str(), "VmRSS:");
			if (p == NULL) {
				n = 0;
			} else {
				sscanf(p, "%s%d", buf, &n);
			}
		
			snprintf(buf, sizeof(buf), "/proc/%s/stat", dp->d_name);	
			tmp = readFile(buf);
			if (tmp.empty()) {
				closedir(dirp);
				return;
			}
			
			StatData data;
			data.parse(tmp);
			int ctime = getCpuTime();
			auto it = process_.find(ps.pid);
			if (it == process_.end()) {
				ps.pushMemory(0);
				ps.pushCpuRate(0.0);
				ps.lastUtime_ = data.utime;
				ps.lastStime_ = data.stime;
				ps.lastCtime_ = ctime;
				process_.insert({ps.pid, ps});

				minps.mem = 0;
				minps.cpu = 0.0;
				minProcessData_.insert({minps.pid, minps});
				continue;
			}

			int userTime = std::max(0, static_cast<int>(data.utime - process_[ps.pid].lastUtime_));
			int sysTime  = std::max(0, static_cast<int>(data.stime - process_[ps.pid].lastStime_));
			int cpuTime  = std::max(1, static_cast<int>(     ctime - process_[ps.pid].lastCtime_));
			double cpuRate = ((double)(userTime + sysTime)) / (double)cpuTime;
			// double cpuRate = ((double)(userTime + sysTime)) / (double)(kClockTicksPerSecond * kPeriod_);
			
			process_[ps.pid].pushMemory(n);
			process_[ps.pid].pushCpuRate(cpuRate);
			process_[ps.pid].lastUtime_ = data.utime;
			process_[ps.pid].lastStime_ = data.stime;
			process_[ps.pid].lastCtime_ = ctime;

			minProcessData_[minps.pid].mem = n;
			minProcessData_[minps.pid].cpu = cpuRate;
		}
	}
	closedir(dirp);

	processNum_ = processNum;
	threadsNum_ = threadsNum;

}