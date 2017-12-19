// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef CURRENTTHREAD_H
#define CURRENTTHREAD_H

#include <time.h>
#include <stdint.h>
#include "Timestamp.h"


namespace CurrentThread
{
  // internal
  extern __thread int t_cachedTid;
  extern __thread char t_tidString[32];
  extern __thread int t_tidStringLength;
  extern __thread const char* t_threadName;
  void cacheTid();

  inline int tid()
  {
    if (__builtin_expect(t_cachedTid == 0, 0))
    {
      cacheTid();
    }
    return t_cachedTid;
  }

  inline const char* tidString() // for logging
  {
    return t_tidString;
  }

  inline int tidStringLength() // for logging
  {
    return t_tidStringLength;
  }

  inline const char* name()
  {
    return t_threadName;
  }

  void sleepUsec(int64_t usec)
  {
    struct timespec ts = { 0, 0 };
    ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicrosecondsPerSecond);
    ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicrosecondsPerSecond * 1000);
    ::nanosleep(&ts, NULL);
  }
}

#endif
