#include "common/filesystem/win32/watcher_impl.h"

namespace Envoy {
namespace Filesystem {
WatcherImpl::WatcherImpl(Event::Dispatcher& dispatcher) {}
WatcherImpl::~WatcherImpl() {}

// Filesystem::Watcher
void WatcherImpl::addWatch(const std::string& path, uint32_t events, OnChangedCb cb) {}

} // namespace Filesystem
} // namespace Envoy
