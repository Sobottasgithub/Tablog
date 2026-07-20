#include <iostream>

#include "tablog_registry.h"
#include "tablog.h"

#include <memory>

int main() {
    tablog::TablogRegistry* registry = &tablog::TablogRegistry::getInstance();
    std::shared_ptr<tablog::Tablog> logger = std::make_shared<tablog::Tablog>();
    logger->configure("TestClient", true);
    registry->registerLogger("TestClient", logger);

    std::shared_ptr<tablog::Tablog> loggerInstance = registry->get("TestClient"); 
    loggerInstance->log(tablog::DEBUG, "Tablog");
    loggerInstance->log(tablog::INFO, "Tablog");
    loggerInstance->log(tablog::WARNING, "Tablog");
    loggerInstance->log(tablog::ERROR, "Tablog");
    loggerInstance->log(tablog::CRITICAL, "Tablog");
}
