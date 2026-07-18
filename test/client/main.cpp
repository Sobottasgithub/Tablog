#include <iostream>

#include "tablog.h"

int main() {
    tablog::Tablog* logger = &tablog::Tablog::getInstance();
    logger->configure("TestClient", true);
    logger->log(tablog::DEBUG, "Tablog");
    logger->log(tablog::INFO, "Tablog");
    logger->log(tablog::WARNING, "Tablog");
    logger->log(tablog::ERROR, "Tablog");
    logger->log(tablog::CRITICAL, "Tablog");
}
