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
    this->registeredLogger.insert_or_assign(key, tablogInstance);
  }

  std::shared_ptr<Tablog> TablogRegistry::get(std::string key) {
    return this->registeredLogger.at(key);
  }
}
