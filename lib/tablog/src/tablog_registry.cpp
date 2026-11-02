#include "../include/tablog_registry.h"

#include <unordered_map>
#include <memory>

namespace tablog {
  TablogRegistry::TablogRegistry() {
    this->registeredLogger = {};
  }
  
  TablogRegistry& TablogRegistry::getInstance() {
    static TablogRegistry instance; 
    return instance;
  }

  void TablogRegistry::registerLogger(const std::string& key, std::shared_ptr<Tablog> tablogInstance) {
    std::lock_guard<std::mutex> lock(loggerMutex);
    this->registeredLogger.insert_or_assign(key, tablogInstance);
  }

  std::shared_ptr<Tablog> TablogRegistry::get(std::string key) {
    std::lock_guard<std::mutex> lock(loggerMutex);
    if (this->registeredLogger.size() > 0) {
      return this->registeredLogger.at(key);
    }
    return nullptr;
  }
}
