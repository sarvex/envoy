#include "common/common/thread.h"

#ifdef __linux__
#include <sys/syscall.h>
#elif defined(__APPLE__)
#include <pthread.h>
#endif

#include <functional>

#include "common/common/assert.h"
#include "common/common/macros.h"

namespace Envoy {
namespace Thread {

#if !defined(WIN32)
Thread::Thread(std::function<void()> thread_routine) : thread_routine_(thread_routine) {
  int rc = pthread_create(&thread_id_, nullptr,
                          [](void* arg) -> void* {
                            static_cast<Thread*>(arg)->thread_routine_();
                            return nullptr;
                          },
                          this);
  RELEASE_ASSERT(rc == 0);
  UNREFERENCED_PARAMETER(rc);
}

int32_t Thread::currentThreadId() {
#ifdef __linux__
  return syscall(SYS_gettid);
#elif defined(__APPLE__)
  uint64_t tid;
  pthread_threadid_np(NULL, &tid);
  return static_cast<int32_t>(tid);
#else
#error "Enable and test pthread id retrieval code for you arch in thread.cc"
#endif
}

void Thread::join() {
  int rc = pthread_join(thread_id_, nullptr);
  RELEASE_ASSERT(rc == 0);
  UNREFERENCED_PARAMETER(rc);
}

#else
  Thread::Thread(std::function<void()> thread_routine) : thread_routine_(thread_routine) {
    thread_id_ = CreateThread(NULL, 0,
      [](void* arg) -> DWORD WINAPI {
      static_cast<Thread*>(arg)->thread_routine_();
      return 0;
    },
      this,
      0, // use default creation flags
      NULL);
    RELEASE_ASSERT(thread_id_ != NULL);
  }

  int32_t Thread::currentThreadId() { return GetCurrentThreadId(); }

  void Thread::join() { WaitForSingleObject(thread_id_, INFINITE); }
#endif

} // namespace Thread
} // namespace Envoy
