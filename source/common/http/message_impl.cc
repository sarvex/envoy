#include "common/http/message_impl.h"

#include <cstdint>
#include <string>

namespace Envoy {
namespace Http {

std::string MessageImpl::bodyAsString() const {
  std::string ret;
  if (body_) {
    uint64_t num_slices = body_->getRawSlices(nullptr, 0);
#if !defined(WIN32)
    Buffer::RawSlice slices[num_slices];
#else
    Buffer::RawSlice* slices =
      reinterpret_cast<Buffer::RawSlice*>(_alloca(sizeof(Buffer::RawSlice) * num_slices));
#endif
    body_->getRawSlices(slices, num_slices);
    for (int i = 0; i < num_slices; i++) {
      Buffer::RawSlice& slice = slices[i];
      ret.append(reinterpret_cast<const char*>(slice.mem_), slice.len_);
    }
  }
  return ret;
}

} // namespace Http
} // namespace Envoy
