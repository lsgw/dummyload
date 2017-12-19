#ifndef READSMALLFILE_H
#define READSMALLFILE_H
#include <string>


class ReadSmallFile{
public:
	ReadSmallFile(const std::string& filename);
	~ReadSmallFile();

	int readToBuffer(int* size);
	int readToString(int maxSize, std::string* content, int64_t* fileSize, int64_t* modifyTime, int64_t* createTime);
	
	const char* buffer() const { return buf_; }
	static const int kBufferSize = 64*1024;
private:
	int fd_;
	int err_;
	char buf_[kBufferSize];
};

int readFile(const std::string& filename, int maxSize, std::string* content, int64_t* fileSize=NULL, int64_t* modifyTime=NULL, int64_t* createTime=NULL);


#endif