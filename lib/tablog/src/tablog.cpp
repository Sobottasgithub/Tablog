#include "../include/tablog.h"

#include <iostream>
#include <ctime>
#include <mutex>
#include <pthread.h>
#include <toml++/impl/parse_result.hpp>
#include <unordered_map>
#include <optional>

#include <toml++/toml.hpp>

namespace tablog {
  void Tablog::configureWithToml(std::string configPath) {
    try {
      auto tomlTable = toml::parse_file(configPath);

      std::string name = std::string(tomlTable["default"]["name"].value_or(""));
      bool displayName = bool(tomlTable["default"]["displayName"].value_or(true));
      bool displayTimestamp = bool(tomlTable["default"]["displayTimestamp"].value_or(true));
      bool storeLogs = bool(tomlTable["default"]["storeLogs"].value_or(false));

      Tablog::LoglevelConfig debugConfig = getLoglevelConfigFromToml("debug", tomlTable);
      Tablog::LoglevelConfig infoConfig = getLoglevelConfigFromToml("info", tomlTable);
      Tablog::LoglevelConfig warningConfig = getLoglevelConfigFromToml("warning", tomlTable);
      Tablog::LoglevelConfig errorConfig = getLoglevelConfigFromToml("error", tomlTable);
      Tablog::LoglevelConfig criticalConfig = getLoglevelConfigFromToml("critical", tomlTable);

      configure(name, displayName, displayTimestamp, storeLogs, debugConfig, infoConfig, warningConfig, errorConfig, criticalConfig);
    } catch (const toml::parse_error& err) {
      std::cout << "Unable to open config toml file: " << err << std::endl;
      configure("", false, false);
    }
  }
  
  void Tablog::configure(std::string name,
                         std::optional<bool> displayName,
                         std::optional<bool> displayTimestamp,
                         std::optional<bool> storeLogs,
                         std::optional<LoglevelConfig> debug,
                         std::optional<LoglevelConfig> info,
                         std::optional<LoglevelConfig> warning,
                         std::optional<LoglevelConfig> error,
                         std::optional<LoglevelConfig> critical) {
    std::lock_guard<std::mutex> lock(loggerMutex);
    this->config.name = name;
    if (displayName.has_value())
      this->config.displayName = displayName.value();
    else
      this->config.displayName = true;
    
    if (displayTimestamp.has_value())
      this->config.displayTimestamp = displayTimestamp.value();
    else
      this->config.displayTimestamp = true;

    if (storeLogs.has_value())
      this->config.storeLogs = storeLogs.value();
    else
      this->config.storeLogs = false;

    this->config.loglevelConfigs = {
      {LogLevel::DEBUG, setOptional(debug, "32m")},
      {LogLevel::INFO, setOptional(info, "36m")},
      {LogLevel::WARNING, setOptional(warning, "93m")},
      {LogLevel::ERROR, setOptional(error, "41;30m")},
      {LogLevel::CRITICAL, setOptional(critical, "95m")}
    };
  }
   
  void Tablog::log(LogLevel loglevel, const std::string& message) {
     std::lock_guard<std::mutex> lock(loggerMutex);
     if (!this->config.loglevelConfigs[loglevel].visible)
       return;
     if (this->config.displayName) {
       std::cout << "<\033[1;34m" << this->config.name << "\033[0m> ";

       if (this->config.storeLogs)
         this->logs += "<" + this->config.name + "> ";
     }
     
     std::cout << "[" << logLevelToString(loglevel) << "] ";
     if (this->config.storeLogs)
         this->logs += "[" + logLevelToString(loglevel) + "] ";

     if (this->config.displayTimestamp) {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp),
                 "%H:%M:%S", timeinfo);
        std::cout << timestamp << " ";

        if (this->config.storeLogs)
          this->logs = this->logs + timestamp + " ";
     }
     std::cout << message.c_str() << std::endl;
     if (this->config.storeLogs)
         this->logs += message + "\n";
  }

  void Tablog::storeLogs(std::string filePath) {
    std::cout << this->logs;
  }

  std::string Tablog::logLevelToString(LogLevel level) {
    switch (level) {
      // https://en.wikipedia.org/wiki/ANSI_escape_code
      case DEBUG:
          return "\033[" + this->config.loglevelConfigs[DEBUG].color + "DEBUG\033[0m";
      case INFO:
          return "\033[" + this->config.loglevelConfigs[INFO].color + "INFO\033[0m";
      case WARNING:
          return "\033[" + this->config.loglevelConfigs[WARNING].color + "WARNING\033[0m";
      case ERROR:
          return "\033[" + this->config.loglevelConfigs[ERROR].color + "ERROR\033[0m";
      case CRITICAL:
          return "\033[" + this->config.loglevelConfigs[CRITICAL].color + "CRITICAL\033[0m";
      default:
          return "UNKNOWN";
      }
  }

  Tablog::LoglevelConfig Tablog::setOptional(std::optional<LoglevelConfig> option, std::string replacement) {
    LoglevelConfig loglevelConfig;
    if (option.has_value()) {
      if (option.value().color.size() != 0)
        loglevelConfig.color = option.value().color;
      else
        loglevelConfig.color = replacement;
      loglevelConfig.visible = option.value().visible;
    } else
      loglevelConfig.color = replacement;

    return loglevelConfig;
  }

  Tablog::LoglevelConfig Tablog::getLoglevelConfigFromToml(std::string logLevel, toml::table table) {
    std::string color = std::string(table["loglevel"][logLevel]["color"].value_or(""));
    bool visible = bool(table["loglevel"][logLevel]["visible"].value_or(true));

    Tablog::LoglevelConfig config;
    config.color = color;
    config.visible = visible;

    return config;
  }
}
