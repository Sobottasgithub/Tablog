#ifndef TABLOG_HELPERS_H
#define TABLOG_HELPERS_H

#include "tablog.h"

#include <toml++/toml.hpp>

namespace tablog {
  class TablogHelpers {
    public:
      static Tablog::LoglevelConfig getLoglevelConfigFromToml(std::string logLevel, toml::table table);
  };
}

#endif
