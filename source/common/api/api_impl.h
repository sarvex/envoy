#pragma once

#include <chrono>
#include <string>

#include "envoy/api/api.h"
#include "envoy/filesystem/filesystem.h"

namespace Envoy {
namespace Api {

/**
 * Implementation of Api::Api
 */
#if !defined(_MSC_VER)
class Impl : public Api::Api {
#else
class Impl : public Api {
#endif
  public:
  Impl(std::chrono::milliseconds file_flush_interval_msec);

  // Api::Api
  Event::DispatcherPtr allocateDispatcher() override;
  Filesystem::FileSharedPtr createFile(const std::string& path, Event::Dispatcher& dispatcher,
                                       Thread::BasicLockable& lock,
                                       Stats::Store& stats_store) override;
  bool fileExists(const std::string& path) override;
  std::string fileReadToEnd(const std::string& path) override;

private:
  std::chrono::milliseconds file_flush_interval_msec_;
};

} // namespace Api
} // namespace Envoy
