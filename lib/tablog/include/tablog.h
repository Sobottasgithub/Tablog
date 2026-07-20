#ifndef libtablog
#define libtablog

#include <string>
#include <mutex>

namespace tablog {
 enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

 class Tablog {
  public:
   Tablog() {};

   void configure(std::string name, bool displayTimestamp);
   void log(LogLevel loglevel, const std::string& message);
   
  private:
   static std::mutex loggerMutex;

   std::string name = "";
   bool displayTimestamp = true;

   std::string logLevelToString(LogLevel level);
 };
}

#endif
