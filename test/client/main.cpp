#include <iostream>

#include "tablog_registry.h"
#include "tablog.h"

#include <memory>

int main() {
    tablog::TablogRegistry* registry = &tablog::TablogRegistry::getInstance();
    std::shared_ptr<tablog::Tablog> logger = std::make_shared<tablog::Tablog>();
    logger->configure("TestClient", true);
    registry->registerLogger("TestClient", logger);

    std::shared_ptr<tablog::Tablog> logger2 = std::make_shared<tablog::Tablog>();
    logger2->configure("TestClient2", true);
    registry->registerLogger("TestClient2", logger2);


    std::shared_ptr<tablog::Tablog> loggerInstance1 = registry->get("TestClient");
    std::shared_ptr<tablog::Tablog> loggerInstance2 = registry->get("TestClient2");  
    loggerInstance1->log(tablog::DEBUG, "Tablog");
    loggerInstance2->log(tablog::INFO, "Tablog");
    loggerInstance1->log(tablog::WARNING, "Tablog");
    loggerInstance2->log(tablog::ERROR, "Tablog");
    loggerInstance1->log(tablog::CRITICAL, "Tablog");
}
