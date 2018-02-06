#if defined(WIN32)
#include <WinSock2.h>
#undef X509_NAME
#undef DELETE
#undef ERROR
#undef TRUE
#endif

#define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
#define EPFNOSUPPORT            WSAEPFNOSUPPORT
#define ESHUTDOWN               WSAESHUTDOWN
#define ETOOMANYREFS            WSAETOOMANYREFS
#define EHOSTDOWN               WSAEHOSTDOWN
#define EPROCLIM                WSAEPROCLIM
#define EUSERS                  WSAEUSERS
#define EDQUOT                  WSAEDQUOT
#define ESTALE                  WSAESTALE
#define EREMOTE                 WSAEREMOTE

inline int get_socket_error() {
#if !defined(WIN32)
  return errno;
#else
  auto err = WSAGetLastError();
  switch (err) {
  case WSAEINTR:
    return EINTR;
  case WSAEBADF:
    return EBADF;
  case WSAEACCES:
    return EACCES;
  case WSAEFAULT:
    return EFAULT;
  case WSAEINVAL:
    return EINVAL;
  case WSAEMFILE:
    return EMFILE;
  case WSAEWOULDBLOCK:
    return EWOULDBLOCK;
  case WSAEINPROGRESS:
    return EINPROGRESS;
  case WSAEALREADY:
    return EALREADY;
  case WSAENOTSOCK:
    return ENOTSOCK;
  case WSAEDESTADDRREQ:
    return EDESTADDRREQ;
  case WSAEMSGSIZE:
    return EMSGSIZE;
  case WSAEPROTOTYPE:
    return EPROTOTYPE;
  case WSAENOPROTOOPT:
    return ENOPROTOOPT;
  case WSAEPROTONOSUPPORT:
    return EPROTONOSUPPORT;
  case WSAEOPNOTSUPP:
    return EOPNOTSUPP;
  case WSAEPFNOSUPPORT:
    return EPFNOSUPPORT;
  case WSAEAFNOSUPPORT:
    return EAFNOSUPPORT;
  case WSAEADDRINUSE:
    return EADDRINUSE;
  case WSAEADDRNOTAVAIL:
    return EADDRNOTAVAIL;
  case WSAENETDOWN:
    return ENETDOWN;
  case WSAENETUNREACH:
    return ENETUNREACH;
  case WSAENETRESET:
    return ENETRESET;
  case WSAECONNABORTED:
    return ECONNABORTED;
  case WSAECONNRESET:
    return ECONNRESET;
  case WSAENOBUFS:
    return ENOBUFS;
  case WSAEISCONN:
    return EISCONN;
  case WSAENOTCONN:
    return ENOTCONN;
  case WSAESHUTDOWN:
    return ESHUTDOWN;
  case WSAETOOMANYREFS:
    return ETOOMANYREFS;
  case WSAETIMEDOUT:
    return ETIMEDOUT;
  case WSAECONNREFUSED:
    return ECONNREFUSED;
  case WSAELOOP:
    return ELOOP;
  case WSAENAMETOOLONG:
    return ENAMETOOLONG;
  case WSAEHOSTDOWN:
    return EHOSTDOWN;
  case WSAEHOSTUNREACH:
    return EHOSTUNREACH;
  case WSAENOTEMPTY:
    return ENOTEMPTY;
  case WSAEPROCLIM:
    return EPROCLIM;
  case WSAEUSERS:
    return EUSERS;
  case WSAEDQUOT:
    return EDQUOT;
  case WSAESTALE:
    return ESTALE;
  case WSAEREMOTE:
    return EREMOTE;
  }
  return err;
#endif
}
