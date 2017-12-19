#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

class noncopyable {
public:
	noncopyable() = default;
	noncopyable(const noncopyable& n) = delete;
	noncopyable& operator=(const noncopyable& ) = delete;
};

#endif