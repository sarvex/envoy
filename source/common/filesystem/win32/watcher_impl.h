#pragma once

#include <cstdint>
#include <list>
#include <string>
#include <unordered_map>

#include "envoy/event/dispatcher.h"
#include "envoy/filesystem/filesystem.h"

#include "common/common/logger.h"

namespace Envoy {
namespace Filesystem {

/**
 * Implementation of Watcher that uses inotify. inotify is an awful API. In order to make this work
 * in a somewhat sane way we always watch the directory that owns the thing being watched, and then
 * filter for events that are relevant to the the thing being watched.
 */
  class WatcherImpl : public Watcher, Logger::Loggable<Logger::Id::file> {
  public:
    WatcherImpl(Event::Dispatcher& dispatcher);
    ~WatcherImpl();

    // Filesystem::Watcher
    void addWatch(const std::string& path, uint32_t events, OnChangedCb cb) override;
  };
} // namespace Filesystem
} // namespace Envoy
