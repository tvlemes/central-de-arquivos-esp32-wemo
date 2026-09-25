#ifndef SD_MANAGER_H
#define SD_MANAGER_H

#include <Arduino.h>

// Estado do cartão SD
extern bool sdInitialized;

// Inicialização do SD
bool setupSD();

// Informações do SD
void printSDInfo();

#endif