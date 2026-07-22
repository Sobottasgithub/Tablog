#include <iostream>

#include "tablog_registry.h"
#include "tablog.h"

#include <memory>
#include <optional>
#include <filesystem>

int main() {
    tablog::TablogRegistry* registry = &tablog::TablogRegistry::getInstance();
    std::shared_ptr<tablog::Tablog> logger = std::make_shared<tablog::Tablog>();
    logger->configure("TestClient", true);
    registry->registerLogger("TestClient", logger);

    std::shared_ptr<tablog::Tablog> logger2 = std::make_shared<tablog::Tablog>();
    tablog::Tablog::LoglevelConfig loglevelConfig = {"32m", true};
    
    logger2->configure("TestClient2", false, false, std::nullopt, loglevelConfig, std::nullopt, loglevelConfig);
    registry->registerLogger("TestClient2", logger2);

    std::shared_ptr<tablog::Tablog> logger3 = std::make_shared<tablog::Tablog>();
    logger3->configureWithToml(std::filesystem::current_path().string() + "/test/client/defaultConfig.toml");
    registry->registerLogger("TestClient3", logger3);

    std::shared_ptr<tablog::Tablog> loggerInstance1 = registry->get("TestClient");
    loggerInstance1->log(tablog::DEBUG, "Tablog");
    loggerInstance1->log(tablog::INFO, "Tablog");
    loggerInstance1->log(tablog::WARNING, "Tablog");
    loggerInstance1->log(tablog::ERROR, "Tablog");
    loggerInstance1->log(tablog::CRITICAL, "Tablog");

    std::shared_ptr<tablog::Tablog> loggerInstance2 = registry->get("TestClient2");  
    loggerInstance2->log(tablog::DEBUG, "Tablog");
    loggerInstance2->log(tablog::INFO, "Tablog");
    loggerInstance2->log(tablog::WARNING, "Tablog");
    loggerInstance2->log(tablog::ERROR, "Tablog");
    loggerInstance2->log(tablog::CRITICAL, "Tablog");

    std::shared_ptr<tablog::Tablog> loggerInstance3 = registry->get("TestClient3");  
    loggerInstance3->log(tablog::DEBUG, "Tablog");
    loggerInstance3->log(tablog::INFO, "Tablog");
    loggerInstance3->log(tablog::WARNING, "Tablog");
    loggerInstance3->log(tablog::ERROR, "Tablog");
    loggerInstance3->log(tablog::CRITICAL, "Tablog");
}
