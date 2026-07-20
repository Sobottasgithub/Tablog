#ifndef TABLOG_REGISTRY_H
#define TABLOG_REGISTRY_H

#include "tablog.h"

#include <string>
#include <mutex>
#include <unordered_map>
#include <memory>

namespace tablog {
    class TablogRegistry {
      public:
        TablogRegistry(const TablogRegistry& object) = delete;
        TablogRegistry& operator=(const TablogRegistry& object) = delete;
   
        static TablogRegistry& getInstance();

        void registerLogger(const std::string& key, std::shared_ptr<Tablog> tablogInstance);
        std::shared_ptr<Tablog> get(std::string key);

      private:
        static std::mutex loggerMutex;
        TablogRegistry();

        std::unordered_map<std::string, std::shared_ptr<Tablog>> registeredLogger;
  };
}

#endif
