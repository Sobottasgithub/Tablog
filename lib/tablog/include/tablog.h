#ifndef libtablog
#define libtablog

#include <string>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace tablog {
 enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

 class Tablog {
  public:
   Tablog() {};

   void configure(std::string name, bool displayTimestamp);
   void log(LogLevel loglevel, const std::string& message);
   
  private:
   std::mutex loggerMutex;

   struct LoglevelConfig {
    std::string color = "";
    bool visible = true;
   };

   struct Config {
    std::string name = "";
    bool displayTimestamp = true;
    std::unordered_map<LogLevel, LoglevelConfig> loglevelConfigs;
   };

   Config config;

   std::string logLevelToString(LogLevel level);
 };
}

#endif
