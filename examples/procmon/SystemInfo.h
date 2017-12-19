#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <list>
#include <map>
#include <string>

struct Process {
	Process() : pid(0), threads(0), lastUtime_(0), lastStime_(0), lastCtime_(0) { }
	pid_t pid;
	std::string user;
	std::string command;
	int threads;

	void pushMemory(int mem);
	void pushCpuRate(double rate);

	const std::list<int> mem() const { return mem_; }
	const std::list<double> cpu() const { return cpu_; }

	std::list<int> mem_;
	std::list<double> cpu_;


	int lastUtime_;
	int lastStime_;
	int lastCtime_;
};

struct MinProcessData {
	pid_t pid;
	std::string user;
	std::string command;
	int threads;
	int mem;
	double cpu;
};

class SystemInfo {
public:
	SystemInfo();

	void update();

	void updateMem();
	void updateCpu();
	void updateProcess();

	int processNum() { return processNum_; }
	int threadsNum() { return threadsNum_; }

	int memTotal() { return memTotal_; }
	int memFree() { return memFree_; }
	int memCached() { return memCached_; }
	int memSwapCached() { return memSwapCached_; }

	double systemRate() { return systemRate_; }
	double userRate() { return userRate_; }
	double idleRate() { return idleRate_; }

	const std::list<double> cpuRate() const { return cpuRate_; }
	const std::list<double> memRate() const { return memRate_; }
	const std::map<int, Process> process() const { return process_; }
	const std::map<int, MinProcessData> minProcessData() const { return minProcessData_; }
private:
	int processNum_;
	int threadsNum_;

	int memTotal_;
	int memFree_;
	int memCached_;
	int memSwapCached_;
	std::list<double> memRate_;

	double systemRate_;
	double userRate_;
	double idleRate_;
	std::list<double> cpuRate_;

	std::map<int, Process> process_;
	std::map<int, MinProcessData> minProcessData_;

	const static int kPeriod_ = 2.0;
};

#endif