#include <iostream>

#include "tablog_registry.h"
#include "tablog.h"

int main() {
    tablog::TablogRegistry* registry = &tablog::TablogRegistry::getInstance();
    
    tablog::Tablog* logger = &tablog::Tablog::getInstance();
    logger->configure("TestClient", true);
    logger->log(tablog::DEBUG, "Tablog");
    logger->log(tablog::INFO, "Tablog");
    logger->log(tablog::WARNING, "Tablog");
    logger->log(tablog::ERROR, "Tablog");
    logger->log(tablog::CRITICAL, "Tablog");
}
