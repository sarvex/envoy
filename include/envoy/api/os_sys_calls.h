#pragma once

#include "envoy/common/socket_fd.h"
#if !defined(_WIN32)
#include <sys/mman.h>   // for mode_t
#include <sys/socket.h> // for sockaddr
#include <sys/uio.h> // for iovec
#else
#include <ws2tcpip.h>
#endif
#include <sys/stat.h>

#include <memory>
#include <string>

#include "envoy/common/pure.h"
#include "envoy/common/ssize.h"

namespace Envoy {
namespace Api {

class OsSysCalls {
public:
  virtual ~OsSysCalls() {}

  /**
   * @see bind (man 2 bind)
   */
  virtual int bind(SOCKET_FD_TYPE sockfd, const sockaddr* addr, socklen_t addrlen) PURE;

  /**
   * @see ioctl (man 2 ioctl)
   */
  virtual int ioctl(SOCKET_FD_TYPE sockfd, unsigned long int request, void* argp) PURE;

  /**
   * Open file by full_path with given flags and mode.
   * @return file descriptor.
   */
  virtual int open(const std::string& full_path, int flags, int mode) PURE;

  /**
   * Write num_bytes to fd from buffer.
   * @return number of bytes written if non negative, otherwise error code.
   */
  virtual ssize_t write(int fd, const void* buffer, size_t num_bytes) PURE;

  /**
   * @see writev (man 2 writev)
   */
  virtual ssize_t writev(SOCKET_FD_TYPE fd, const IOVEC* iovec, int num_iovec) PURE;

  /**
   * @see readv (man 2 readv)
   */
  virtual ssize_t readv(SOCKET_FD_TYPE fd, const IOVEC* iovec, int num_iovec) PURE;

  /**
   * @see recv (man 2 recv)
   */
  virtual ssize_t recv(SOCKET_FD_TYPE socket, void* buffer, size_t length, int flags) PURE;

  /**
   * Release all resources allocated for fd.
   * @return zero on success, -1 returned otherwise.
   */
  virtual int close(int fd) PURE;

#if !defined(_WIN32)
  /**
   * @see shm_open (man 3 shm_open)
   */
  virtual int shmOpen(const char* name, int oflag, mode_t mode) PURE;

  /**
   * @see shm_unlink (man 3 shm_unlink)
   */
  virtual int shmUnlink(const char* name) PURE;
#endif

  /**
   * @see man 2 ftruncate
   */
  virtual int ftruncate(int fd, off_t length) PURE;

#if !defined(_WIN32)
  /**
   * @see man 2 mmap
   */
  virtual void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) PURE;
#endif

  /**
   * @see man 2 stat
   */
  virtual int stat(const char* pathname, struct stat* buf) PURE;

  /**
   * @see man 2 setsockopt
   */
  virtual int setsockopt(SOCKET_FD_TYPE sockfd, int level, int optname, const void* optval,
                         socklen_t optlen) PURE;

  /**
   * @see man 2 getsockopt
   */
  virtual int getsockopt(SOCKET_FD_TYPE sockfd, int level, int optname, void* optval, socklen_t* optlen) PURE;
};

typedef std::unique_ptr<OsSysCalls> OsSysCallsPtr;

} // namespace Api
} // namespace Envoy
