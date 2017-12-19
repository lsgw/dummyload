#include "ReadSmallFile.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
using namespace std;

ReadSmallFile::ReadSmallFile(const string& filename) :
	fd_(::open(filename.c_str(), O_RDONLY|O_CLOEXEC)),  // 'e' for O_CLOEXEC
	err_(0)
{
	buf_[0] = '\0';
	if (fd_ < 0) {
		err_ = errno;
	}
}
ReadSmallFile::~ReadSmallFile()
{
	if (fd_ >= 0) {
		::close(fd_);   // FIXME: check EINTR
	}
}

int ReadSmallFile::readToBuffer(int* size)
{
	int err = err_;
	if (fd_ >= 0) {
		ssize_t n = ::pread(fd_, buf_, sizeof(buf_)-1, 0);
		if (n >= 0) {
			if (size != NULL) {
				*size = static_cast<int>(n);
			}
			buf_[n] = '\0';
		} else {
			err = errno;
		}
	}
	return err;
}
int ReadSmallFile::readToString(int maxSize, string* content, int64_t* fileSize, int64_t* modifyTime, int64_t* createTime)
{
	assert(content != NULL);
	int err = err_;
	if (fd_ >= 0) {
		content->clear();
		if (fileSize != NULL) {
			struct stat statbuf;
			if (::fstat(fd_, &statbuf) == 0) {
				if (S_ISREG(statbuf.st_mode)) {
					*fileSize = statbuf.st_size;
					content->reserve(static_cast<int>(std::min(static_cast<int64_t>(maxSize), *fileSize)));
				} else if (S_ISDIR(statbuf.st_mode)) {
					err = EISDIR;
				}
				if (modifyTime != NULL) {
					*modifyTime = statbuf.st_mtime;
				}
				if (createTime != NULL) {
					*createTime = statbuf.st_ctime;
				}
			} else {
				err = errno;
			}
		}
		while (content->size() < static_cast<size_t>(maxSize)) {
			size_t toRead = std::min(static_cast<size_t>(maxSize) - content->size(), sizeof(buf_));
      		ssize_t n = ::read(fd_, buf_, toRead);
      		if (n > 0) {
      			content->append(buf_, n);
      		} else {
      			if (n < 0) {
      				err = errno;
      			}
      			break;
      		}
		}
	}
	return err;
}

int readFile(const string& filename, int maxSize, string* content, int64_t* fileSize, int64_t* modifyTime, int64_t* createTime)
{
	ReadSmallFile file(filename);
	return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
}