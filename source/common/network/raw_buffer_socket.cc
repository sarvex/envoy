#include "common/network/raw_buffer_socket.h"
#include "common/network/errormap.h"

#include "common/common/assert.h"
#include "common/common/empty_string.h"
#include "common/http/headers.h"

namespace Envoy {
namespace Network {

void RawBufferSocket::setTransportSocketCallbacks(TransportSocketCallbacks& callbacks) {
  callbacks_ = &callbacks;
}

IoResult RawBufferSocket::doRead(Buffer::Instance& buffer) {
  PostIoAction action = PostIoAction::KeepOpen;
  uint64_t bytes_read = 0;
  bool end_stream = false;
  do {
    // 16K read is arbitrary. IIRC, libevent will currently clamp this to 4K. libevent will also
    // use an ioctl() before every read to figure out how much data there is to read.
    //
    // TODO(mattklein123) PERF: Tune the read size and figure out a way of getting rid of the
    // ioctl(). The extra syscall is not worth it.
    int rc = buffer.read(callbacks_->fd(), 16384);
    ENVOY_CONN_LOG(trace, "read returns: {}", callbacks_->connection(), rc);

    if (rc == 0) {
      // Remote close.
      end_stream = true;
      break;
    } else if (rc == -1) {
      // Remote error (might be no data).
      int errorno = get_socket_error();
      ENVOY_CONN_LOG(trace, "read error: {}", callbacks_->connection(), errorno);
      if (errorno != EAGAIN && errorno != EWOULDBLOCK) {
        action = PostIoAction::Close;
      }

      break;
    } else {
      bytes_read += rc;
      if (callbacks_->shouldDrainReadBuffer()) {
        callbacks_->setReadBufferReady();
        break;
      }
    }
  } while (true);

  return {action, bytes_read, end_stream};
}

IoResult RawBufferSocket::doWrite(Buffer::Instance& buffer, bool end_stream) {
  PostIoAction action;
  uint64_t bytes_written = 0;
  ASSERT(!shutdown_ || buffer.length() == 0);
  do {
    if (buffer.length() == 0) {
      if (end_stream && !shutdown_) {
      // Ignore the result. This can only fail if the connection failed. In that case, the
      // error will be detected on the next read, and dealt with appropriately.
#if !defined(WIN32)
        ::shutdown(callbacks_->fd(), SHUT_WR);
#else
        ::shutdown(callbacks_->fd(), SD_SEND);
#endif
        shutdown_ = true;
      }
      action = PostIoAction::KeepOpen;
      break;
    }
    int rc = buffer.write(callbacks_->fd());
    ENVOY_CONN_LOG(trace, "write returns: {}", callbacks_->connection(), rc);
    if (rc == -1) {
      int errorno = get_socket_error();
      ENVOY_CONN_LOG(trace, "write error: {} ({})", callbacks_->connection(), errorno,
                     strerror(errorno));
      if (errorno == EAGAIN || errorno == EWOULDBLOCK) {
        action = PostIoAction::KeepOpen;
      } else {
        action = PostIoAction::Close;
      }

      break;
    } else {
      bytes_written += rc;
    }
  } while (true);

  return {action, bytes_written, false};
}

std::string RawBufferSocket::protocol() const { return EMPTY_STRING; }

void RawBufferSocket::onConnected() { callbacks_->raiseEvent(ConnectionEvent::Connected); }

TransportSocketPtr RawBufferSocketFactory::createTransportSocket() const {
  return std::make_unique<RawBufferSocket>();
}

bool RawBufferSocketFactory::implementsSecureTransport() const { return false; }
} // namespace Network
} // namespace Envoy
