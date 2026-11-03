#ifndef libtablog
#define libtablog

#include <string>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <optional>

namespace tablog {
 enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

 class Tablog {
  public:
   struct LoglevelConfig {
    std::string color = "";
    bool visible = true;
   };

   Tablog() {};

   void configureWithToml(std::string configPath);
   void configure(std::string name,
                  std::optional<bool> displayName = true,
                  std::optional<bool> displayTimestamp = true,
                  std::optional<bool> displayColor = true,
                  std::optional<bool> storeLogs = false,
                  std::optional<std::string> logsDirectory = "",
                  std::optional<LoglevelConfig> debug = std::nullopt,
                  std::optional<LoglevelConfig> info = std::nullopt,
                  std::optional<LoglevelConfig> warning = std::nullopt,
                  std::optional<LoglevelConfig> error = std::nullopt,
                  std::optional<LoglevelConfig> critical = std::nullopt);
  
   void log(LogLevel loglevel, const std::string& message);
   
  private:
   inline static std::mutex loggerMutex;

   struct Config {
    std::string name = "";
    bool displayName = true;
    bool displayTimestamp = true;
    bool displayColor = true;
    bool storeLogs = false;
    std::string logsDirectory = "";
    std::unordered_map<LogLevel, LoglevelConfig> loglevelConfigs;
   };

   Config config;

   std::string logs = "";

   std::string logLevelToString(LogLevel level);

   LoglevelConfig setOptional(std::optional<LoglevelConfig> option, std::string replacement);

   template <typename T>
   T extractConfig(const std::optional<T>& opt, const T& defaultConfig);

   bool initStoreLogs(std::string filePath);
 };
}

#endif
