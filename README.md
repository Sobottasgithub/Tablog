# Tablog
<img width="650" height="135" alt="image" src="https://github.com/user-attachments/assets/c4bccc3e-1dad-41cc-b7b7-824e15f5ee43" />

## Use
### init
```cpp
#include <tablog.h>
std::shared_ptr<tablog::Tablog> logger = std::make_shared<tablog::Tablog>();
```

### configure
The simplest and recomended way to use Tablog is to configure it via a toml file, which can look something like this:
```toml
[default]
name = "TablogTomlConfig"
displayName = true
displayTimestamp = true
displayColor = true
storeLogs = false

[loglevel]
  [[debug]]
  color = "32m"
  visible = true

  [[info]]
  color = "36m"
  visible = true

  [[warning]]
  color = "93m"
  visible = true

  [[error]]
  color = "41;30m"
  visible = true

  [[critical]]
  color = "95m"
  visible = true
```
You must specify the name in the default section, but the remaining details are optional. If you do not specify a configuration option, the default configuration shown above is used. Now you can pass the filepath to:
```cpp
void Tablog::configureWithToml(std::string filepath);
```
If you prefere to configure Tablog with a function you can also use the following function:
```cpp
void Tablog::configure(std::string name,
  std::optional<bool> displayName = true,
  std::optional<bool> displayTimestamp = true,
  std::optional<bool> displayColor = true,
  std::optional<bool> storeLogs = false,
  std::optional<LoglevelConfig> debug = std::nullopt,
  std::optional<LoglevelConfig> info = std::nullopt,
  std::optional<LoglevelConfig> warning = std::nullopt,
  std::optional<LoglevelConfig> error = std::nullopt,
  std::optional<LoglevelConfig> critical = std::nullopt);
```
The LoglevelConfig is a simple struct to define a color and the visibility of a loglevel:
```cpp
struct LoglevelConfig {
  std::string color = "";
  bool visible = true;
};
```

### functions
 - void log(LogLevel loglevel, const std::string& message);
 - bool storeLogs(std::string filePath);
   
### loglevel
 - enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

### registry
If you want to use the logger in other threads without passing an instance as an argument you can register it in the registry:
```cpp
#include <tablog_registry.h>
tablog::TablogRegistry* registry = &tablog::TablogRegistry::getInstance();
registry->registerLogger("logger", loggerInstance);
```
Then in the other thread:
```cpp
tablog::TablogRegistry* registry = &tablog::TablogRegistry::getInstance();
std::shared_ptr<tablog::Tablog> loggerInstance = registry->get("logger");
```

## Test
To test Tablog, you must run the following in the shell:
```
nix run .#client
```
