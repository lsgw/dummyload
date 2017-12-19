#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <string>

class Timestamp {
public:
	explicit Timestamp(int64_t microsecondsSinceEpoch);
	Timestamp() : microsecondsSinceEpoch_(0) 
	{
	}
	void swap(Timestamp& that) 
	{
		std::swap(microsecondsSinceEpoch_, that.microsecondsSinceEpoch_);
	}
	bool valid() const 
	{
		return microsecondsSinceEpoch_ > 0;
	}

	// for internal usage.
	int64_t microsecondsSinceEpoch() const 
	{
		return microsecondsSinceEpoch_;
	}

	time_t secondsSinceEpoch() const 
	{
		return static_cast<time_t>(microsecondsSinceEpoch_ / kMicrosecondsPerSecond);
	}
public:
	std::string toString() const;
	std::string toFormattedString(bool showMicroseconds = true) const;

	static Timestamp now();
	static Timestamp invalid();

	static const int kMicrosecondsPerSecond = 1000 * 1000;
	static const int kMillisecondsPerSecond = 1000;

private:
	int64_t microsecondsSinceEpoch_;
};














inline bool operator<(Timestamp lhs, Timestamp rhs) 
{
	return lhs.microsecondsSinceEpoch() < rhs.microsecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs) 
{
	return lhs.microsecondsSinceEpoch() == rhs.microsecondsSinceEpoch();
}


inline int64_t timeDifferenceMicroseconds(Timestamp high, Timestamp low) 
{
	return high.microsecondsSinceEpoch() - low.microsecondsSinceEpoch();
}


inline int64_t timeDifferenceMilliseconds(Timestamp high, Timestamp low) 
{
	int64_t diff = high.microsecondsSinceEpoch() - low.microsecondsSinceEpoch();
	return diff / Timestamp::kMillisecondsPerSecond;
}

inline double timeDifference(Timestamp high, Timestamp low) 
{
	int64_t diff = high.microsecondsSinceEpoch() - low.microsecondsSinceEpoch();
	return static_cast<double>(diff) / Timestamp::kMicrosecondsPerSecond;
}

inline Timestamp addTime(Timestamp timestamp, double seconds) 
{
	int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicrosecondsPerSecond);
	return Timestamp(timestamp.microsecondsSinceEpoch() + delta);
}


#endif