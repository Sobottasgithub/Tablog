#include <iostream>

#include "tablog.h"

int main() {
    tablog::Tablog* logger = &tablog::Tablog::getInstance();
    logger->configure("TestClient", true);
    logger->log(tablog::DEBUG, "Tablog");
}
