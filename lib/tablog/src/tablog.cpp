#include "../include/tablog.h"

#include "../include/tablog_helpers.h"

#include <iostream>
#include <chrono>
#include <format>
#include <mutex>
#include <pthread.h>
#include <toml++/impl/parse_result.hpp>
#include <unordered_map>
#include <optional>
#include <fstream>
#include <filesystem>

#include <toml++/toml.hpp>

namespace tablog {
  void Tablog::configureWithToml(std::string configPath) {
    try {
      auto tomlTable = toml::parse_file(configPath);

      std::string name = std::string(tomlTable["default"]["name"].value_or(""));
      bool displayName = bool(tomlTable["default"]["displayName"].value_or(true));
      bool displayTimestamp = bool(tomlTable["default"]["displayTimestamp"].value_or(true));
      bool displayColor = bool(tomlTable["default"]["displayColor"].value_or(true));
      
      bool storeLogs = bool(tomlTable["default"]["storeLogs"].value_or(false));
      std::string logsDirectory = std::string(tomlTable["default"]["logsDirectory"].value_or(""));

      Tablog::LoglevelConfig debugConfig = TablogHelpers::getLoglevelConfigFromToml("debug", tomlTable);
      Tablog::LoglevelConfig infoConfig = TablogHelpers::getLoglevelConfigFromToml("info", tomlTable);
      Tablog::LoglevelConfig warningConfig = TablogHelpers::getLoglevelConfigFromToml("warning", tomlTable);
      Tablog::LoglevelConfig errorConfig = TablogHelpers::getLoglevelConfigFromToml("error", tomlTable);
      Tablog::LoglevelConfig criticalConfig = TablogHelpers::getLoglevelConfigFromToml("critical", tomlTable);

      configure(name, displayName, displayTimestamp, displayColor, storeLogs, logsDirectory, debugConfig, infoConfig, warningConfig, errorConfig, criticalConfig);
    } catch (const toml::parse_error& err) {
      std::cout << "Unable to open config toml file: " << err << std::endl;
      configure("", false, false);
    }
  }
  
  void Tablog::configure(std::string name,
                         std::optional<bool> displayName,
                         std::optional<bool> displayTimestamp,
                         std::optional<bool> displayColor,
                         std::optional<bool> storeLogs,
                         std::optional<std::string> logsDirectory,
                         std::optional<LoglevelConfig> debug,
                         std::optional<LoglevelConfig> info,
                         std::optional<LoglevelConfig> warning,
                         std::optional<LoglevelConfig> error,
                         std::optional<LoglevelConfig> critical) {
    std::lock_guard<std::mutex> lock(loggerMutex);
    this->config.name = name;

    this->config.displayName = extractConfig(displayName, true);
    this->config.displayTimestamp = extractConfig(displayTimestamp, true);
    this->config.displayColor = extractConfig(displayColor, true);
    
    this->config.storeLogs = extractConfig(storeLogs, false);
    this->config.logsDirectory = extractConfig(logsDirectory, std::string(""));
    if (this->config.storeLogs) {
      initStoreLogs(this->config.logsDirectory);
    }

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

     std::ostream* logStream = &std::cout;
     if (loglevel == LogLevel::ERROR || loglevel == LogLevel::CRITICAL) {
       logStream = &std::cerr;
     }
       
     if (this->config.displayName) {
       if (this->config.displayColor)
         *logStream << "<\033[1;34m" << this->config.name << "\033[0m> ";
       else
         *logStream << "<" << this->config.name << ">";
        
       if (this->config.storeLogs)
         *(this->logFile) << "<" + this->config.name + "> ";
     }
     
     *logStream << "[" << logLevelToString(loglevel) << "] ";
     if (this->config.storeLogs)
         *(this->logFile) << "[" + logLevelToString(loglevel) + "] ";

     if (this->config.displayTimestamp) {
      auto timeNow = std::chrono::system_clock::now();
      std::chrono::zoned_time localTime{std::chrono::current_zone(), timeNow};
      auto displayTimeNow = std::chrono::floor<std::chrono::seconds>(localTime.get_local_time());

      *logStream << std::format("{:%H:%M:%S}", displayTimeNow) << " ";

      if (this->config.storeLogs)
        *(this->logFile) << std::format("{:%H:%M:%S}", localTime.get_local_time()) + " ";
     }
     *logStream << message.c_str() << std::endl;
     if (this->config.storeLogs)
         *(this->logFile) << message + "\n" << std::flush;
  }

  bool Tablog::initStoreLogs(std::string filePath) {
    std::filesystem::path realFilePath = filePath;
    if (!std::filesystem::exists(realFilePath.parent_path())) {
      std::filesystem::create_directories(realFilePath.parent_path());
    }

    this->logFile = new std::ofstream(realFilePath);
    if (!this->logFile->is_open()) {
      // throw std::runtime_error("Error: Could not create the file");
      return false;
    }
        
    return true;
  }

  void Tablog::closeLogs() {
    if (!this->logFile->is_open()) {
      this->logFile->close();
    }
  }

  std::string Tablog::logLevelToString(LogLevel level) {
    switch (level) {
      // https://en.wikipedia.org/wiki/ANSI_escape_code
      case DEBUG:
          if (this->config.displayColor)
            return "\033[" + this->config.loglevelConfigs[DEBUG].color + "DEBUG\033[0m";
          else
            return "DEBUG";
      case INFO:
          if (this->config.displayColor)
            return "\033[" + this->config.loglevelConfigs[INFO].color + "INFO\033[0m";
          else
            return "INFO";
      case WARNING:
          if (this->config.displayColor)
            return "\033[" + this->config.loglevelConfigs[WARNING].color + "WARNING\033[0m";
          else
            return "WARNING";
      case ERROR:
          if (this->config.displayColor)
            return "\033[" + this->config.loglevelConfigs[ERROR].color + "ERROR\033[0m";
          else
            return "ERROR";
      case CRITICAL:
          if (this->config.displayColor)
            return "\033[" + this->config.loglevelConfigs[CRITICAL].color + "CRITICAL\033[0m";
          else
            return "CRITICAL";
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

  template <typename T>
  T Tablog::extractConfig(const std::optional<T>& optional, const T& defaultConfig) {
      return optional.value_or(defaultConfig);
  }
}
