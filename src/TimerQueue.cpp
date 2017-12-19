#include "TimerQueue.h"
#include "EventLoop.h"
#include "Timer.h"
#include <sys/timerfd.h>
#include <unistd.h>
#include <strings.h>
#include <assert.h>
using namespace std;

static int createTimerfd()
{
	int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (timerfd < 0) {
		//LOG_SYSFATAL << "Failed in timerfd_create";
		assert(0);
	}
	return timerfd;
}

static struct timespec howMuchTimeFromNow(Timestamp when)
{
	int64_t microseconds = when.microsecondsSinceEpoch() - Timestamp::now().microsecondsSinceEpoch();
	if (microseconds < 100) {
		microseconds = 100;
	}
	struct timespec ts;
	ts.tv_sec = static_cast<time_t>( microseconds / Timestamp::kMicrosecondsPerSecond);
	ts.tv_nsec = static_cast<long>((microseconds % Timestamp::kMicrosecondsPerSecond) * 1000);

	return ts;
}


static void readTimerfd(int timerfd, Timestamp now)
{
	uint64_t howmany;
	ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));
	//LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
	if (n != sizeof(howmany)) {
		//LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
		assert(0);
	}
}

void resetTimerfd(int timerfd, Timestamp expiration)
{
	// wake up loop by timerfd_settime()
	struct itimerspec newValue;
	struct itimerspec oldValue;
	bzero(&newValue, sizeof newValue);
	bzero(&oldValue, sizeof oldValue);

	newValue.it_value = howMuchTimeFromNow(expiration);
	int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
	if (ret) {
		//LOG_SYSERR << "timerfd_settime()";
		assert(0);
	}
}

TimerQueue::TimerQueue(EventLoop* loop) : 
	loop_(loop),
	timerfd_(createTimerfd()),
	channel_(new Channel(loop_, timerfd_)),
	timers_(),
	callingExpiredTimers_(false)
{
	channel_->setReadCallback(std::bind(&TimerQueue::handleRead, this));
	channel_->enableReading();

}

TimerQueue::~TimerQueue()
{
	channel_->disableAll();
	::close(timerfd_);
	for (auto& m : timers_) {
		delete m.second;
	}
}

Timer* TimerQueue::addTimer(const TimerCallback& cb, Timestamp when, double interval)
{
	Timer* timer = new Timer(cb, when, interval);
	
	bool earliestChanged = insert(timer);
	if ( earliestChanged ) {
		resetTimerfd(timerfd_, timer->expiration());
	}
	
	return timer;
}

void TimerQueue::cancel(Timer* timer)
{
	Timestamp when = timer->expiration();
	auto it = timers_.find(pair<Timestamp, Timer*>(when, timer));
	if ( it != timers_.end()) {
		size_t n = timers_.erase(pair<Timestamp, Timer*>(when, timer));
		assert(n == 1); 
		delete it->second; // FIXME: no delete please
	} else if (callingExpiredTimers_) {
		cancelingTimers_.insert(timer);
	}
}

void TimerQueue::handleRead()
{
	Timestamp now(Timestamp::now());
	readTimerfd(timerfd_, now);
	
	cancelingTimers_.clear();
	vector<pair<Timestamp, Timer*>> expired = getExpired(now);
	callingExpiredTimers_ = true;
	for (auto &m : expired) {
		m.second->run();
	}
	callingExpiredTimers_ = false;

	reset(expired, now);
}

vector<pair<Timestamp, Timer*>> TimerQueue::getExpired(Timestamp now)
{
	vector<pair<Timestamp, Timer*>> expired;
	pair<Timestamp, Timer*> sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
	auto end = timers_.lower_bound(sentry);
	assert(end == timers_.end() || now < end->first);
	std::copy(timers_.begin(), end, back_inserter(expired));
	timers_.erase(timers_.begin(), end);

	return expired;
}

void TimerQueue::reset(const vector<pair<Timestamp, Timer*>>& expired, Timestamp now)
{
	Timestamp nextExpire;

	for (auto it=expired.begin(); it!=expired.end(); ++it) {
		Timer* timer = it->second;
		if (it->second->repeat() && cancelingTimers_.find(timer) == cancelingTimers_.end()) {
			it->second->restart(now);
			insert(it->second);
		} else {
			delete it->second; // FIXME: no delete please
		}
	}

	if (!timers_.empty()) {
		nextExpire = timers_.begin()->second->expiration();
	}

	if (nextExpire.valid()) {
		resetTimerfd(timerfd_, nextExpire);
	}
}

bool TimerQueue::insert(Timer* timer)
{
	bool earliestChanged = false;
	Timestamp when = timer->expiration();
	auto it = timers_.begin();
	if (it == timers_.end() || when < it->first) {
		earliestChanged = true;
	}
	
	auto result = timers_.insert(pair<Timestamp, Timer*>(when, timer));
	assert(result.second);
	
	return earliestChanged;
}

