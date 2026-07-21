#include "../include/tablog.h"

#include <iostream>
#include <ctime>
#include <mutex>
#include <pthread.h>
#include <unordered_map>

namespace tablog {
  void Tablog::configure(std::string name, bool displayTimestamp) {
    std::lock_guard<std::mutex> lock(loggerMutex);
    this->config.name = name;
    this->config.displayTimestamp = displayTimestamp;

    LoglevelConfig debugConfig;
    debugConfig.color = "32m";

    LoglevelConfig infoConfig;
    infoConfig.color = "36m";

    LoglevelConfig warningConfig;
    warningConfig.color = "93m";

    LoglevelConfig errorConfig;
    errorConfig.color = "41;30m";

    LoglevelConfig criticalConfig;
    criticalConfig.color = "95m";

    this->config.loglevelConfigs = {
      {LogLevel::DEBUG, debugConfig},
      {LogLevel::INFO, infoConfig},
      {LogLevel::WARNING, warningConfig},
      {LogLevel::ERROR, errorConfig},
      {LogLevel::CRITICAL, criticalConfig}
    };
  }
   
  void Tablog::log(LogLevel loglevel, const std::string& message) {
     std::lock_guard<std::mutex> lock(loggerMutex);
     if (!this->config.loglevelConfigs[loglevel].visible)
       return;
     
     std::cout << "<\033[1;34m" << this->config.name << "\033[0m> " << "[" << logLevelToString(loglevel) << "] ";
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
}
