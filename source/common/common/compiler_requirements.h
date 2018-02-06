#pragma once

namespace Envoy {

#if __cplusplus < 201402L && !defined(_MSC_VER)
#error "Your compiler does not support C++14. GCC 5+ or Clang is required."
#endif
} // Envoy
