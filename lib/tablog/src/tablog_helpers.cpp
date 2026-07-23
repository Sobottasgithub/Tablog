#include "../include/tablog_helpers.h"

#include <toml++/toml.hpp>

namespace tablog {
  Tablog::LoglevelConfig TablogHelpers::getLoglevelConfigFromToml(std::string logLevel, toml::table table) {
    std::string color = std::string(table["loglevel"][logLevel]["color"].value_or(""));
    bool visible = bool(table["loglevel"][logLevel]["visible"].value_or(true));

    Tablog::LoglevelConfig config;
    config.color = color;
    config.visible = visible;

    return config;
  }
}
