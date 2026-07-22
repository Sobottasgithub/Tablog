#include "../include/tablog.h"

#include <iostream>
#include <ctime>
#include <mutex>
#include <pthread.h>
#include <unordered_map>
#include <optional>

namespace tablog {
  void Tablog::configure(std::string name,
                         std::optional<bool> displayName,
                         std::optional<bool> displayTimestamp,
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
     if (this->config.displayName)
       std::cout << "<\033[1;34m" << this->config.name << "\033[0m> ";
     
     std::cout << "[" << logLevelToString(loglevel) << "] ";
     if (this->config.displayTimestamp) {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp),
                 "%H:%M:%S", timeinfo);
        std::cout << timestamp << " ";
     }
     std::cout << message.c_str() << std::endl;
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
      loglevelConfig.color = option.value().color;
      loglevelConfig.visible = option.value().visible;
    } else
      loglevelConfig.color = replacement;

    return loglevelConfig;
  }
}
