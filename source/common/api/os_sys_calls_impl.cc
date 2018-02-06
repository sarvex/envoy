#include "common/api/os_sys_calls_impl.h"

#include <fcntl.h>
#include <sys/stat.h>
#if !defined(_WIN32)
#include <unistd.h>
#else
#include <WinSock2.h>
#undef X509_NAME
#undef DELETE
#undef ERROR
#undef TRUE
#include <io.h>
#endif

namespace Envoy {
namespace Api {

int OsSysCallsImpl::bind(SOCKET_FD_TYPE sockfd, const sockaddr* addr, socklen_t addrlen) {
  return ::bind(sockfd, addr, addrlen);
}

int OsSysCallsImpl::ioctl(SOCKET_FD_TYPE sockfd, unsigned long int request, void* argp) {
#if !defined(_WIN32)
  return ::ioctl(sockfd, request, argp);
#else
	return ::ioctlsocket(sockfd, request, reinterpret_cast<u_long*>(argp));
#endif
}

int OsSysCallsImpl::open(const std::string& full_path, int flags, int mode) {
  return ::open(full_path.c_str(), flags, mode);
}

int OsSysCallsImpl::close(int fd) { return ::close(fd); }

ssize_t OsSysCallsImpl::write(int fd, const void* buffer, size_t num_bytes) {
  return ::write(fd, buffer, num_bytes);
}

ssize_t OsSysCallsImpl::writev(SOCKET_FD_TYPE fd, const IOVEC* vec, int num_iovec) {
#if !defined(_WIN32)
  return ::writev(fd, vec, num_iovec);
#else
  DWORD bytesSent = 0;
  auto err = WSASend(fd, const_cast<IOVEC*>(vec), num_iovec, &bytesSent, 0, nullptr, nullptr);
  if (err == 0)
  {
	  WSASetLastError(0);
  }
  return bytesSent;
#endif
}

ssize_t OsSysCallsImpl::readv(SOCKET_FD_TYPE fd, const IOVEC* vec, int num_iovec) {
#if !defined(_WIN32)
  return ::readv(fd, iovec, num_iovec);
#else
  DWORD bytesRead = 0;
  auto err = WSARecv(fd, const_cast<IOVEC*>(vec), num_iovec, &bytesRead, 0, nullptr, nullptr);
  if (err == 0)
  {
	  WSASetLastError(0);
  }
  return bytesRead;
#endif
}

ssize_t OsSysCallsImpl::recv(SOCKET_FD_TYPE socket, void* buffer, size_t length, int flags) {
#if !defined(_WIN32)
  return ::recv(socket, buffer, length, flags);
#else
  return ::recv(socket, (char *)buffer, length, flags);
#endif
}

#if !defined(_WIN32)
int OsSysCallsImpl::shmOpen(const char* name, int oflag, mode_t mode) {
  return ::shm_open(name, oflag, mode);
}

int OsSysCallsImpl::shmUnlink(const char* name) { return ::shm_unlink(name); }

int OsSysCallsImpl::ftruncate(int fd, off_t length) { return ::ftruncate(fd, length); }
#else
int OsSysCallsImpl::ftruncate(int fd, off_t length) { return ::_chsize_s(fd, length); }
#endif

#if !defined(_WIN32)
void* OsSysCallsImpl::mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) {
  return ::mmap(addr, length, prot, flags, fd, offset);
}
#endif

int OsSysCallsImpl::stat(const char* pathname, struct stat* buf) { return ::stat(pathname, buf); }

int OsSysCallsImpl::setsockopt(SOCKET_FD_TYPE sockfd, int level, int optname, const void* optval,
                               socklen_t optlen) {
#if !defined(_WIN32)
  return ::setsockopt(sockfd, level, optname, optval, optlen);
#else
  return ::setsockopt(sockfd, level, optname, (const char *)optval, optlen);
#endif
}

int OsSysCallsImpl::getsockopt(SOCKET_FD_TYPE sockfd, int level, int optname, void* optval,
                               socklen_t* optlen) {
#if !defined(_WIN32)
  return ::getsockopt(sockfd, level, optname, optval, optlen);
#else
  return ::getsockopt(sockfd, level, optname, (char *)optval, optlen);
#endif
}

} // namespace Api
} // namespace Envoy
