#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

// Estado da conexão Wi-Fi
extern bool wifiConnected;

// Inicialização do Wi-Fi
void setupWiFi();

// Inicialização do Access Point
void startAccessPoint();

#endif