#pragma once

#include "envoy/api/os_sys_calls.h"

#include "common/singleton/threadsafe_singleton.h"

namespace Envoy {
namespace Api {

class OsSysCallsImpl : public OsSysCalls {
public:
  // Api::OsSysCalls
  int bind(SOCKET_FD_TYPE sockfd, const sockaddr* addr, socklen_t addrlen) override;
  int ioctl(SOCKET_FD_TYPE sockfd, unsigned long int request, void* argp) override;
  int open(const std::string& full_path, int flags, int mode) override;
  ssize_t write(int fd, const void* buffer, size_t num_bytes) override;
  ssize_t writev(SOCKET_FD_TYPE fd, const IOVEC* iovec, int num_iovec) override;
  ssize_t readv(SOCKET_FD_TYPE fd, const IOVEC* iovec, int num_iovec) override;
  ssize_t recv(SOCKET_FD_TYPE socket, void* buffer, size_t length, int flags) override;
  int close(int fd) override;
#if !defined(_WIN32)
  int shmOpen(const char* name, int oflag, mode_t mode) override;
  int shmUnlink(const char* name) override;
#endif
  int ftruncate(int fd, off_t length) override;
#if !defined(_WIN32)
  void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) override;
#endif
  int stat(const char* pathname, struct stat* buf) override;
  int setsockopt(SOCKET_FD_TYPE sockfd, int level, int optname, const void* optval, socklen_t optlen) override;
  int getsockopt(SOCKET_FD_TYPE sockfd, int level, int optname, void* optval, socklen_t* optlen) override;
};

typedef ThreadSafeSingleton<OsSysCallsImpl> OsSysCallsSingleton;

} // namespace Api
} // namespace Envoy
