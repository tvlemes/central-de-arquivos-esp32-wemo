/*
====================================================================
                        WIFI MANAGER
====================================================================
*/

#include "wifi_manager.h"

#include <Arduino.h>
#include <WiFi.h>

#include "config.h"

// ==================================================================
// ESTADO DO WI-FI
// ==================================================================

bool wifiConnected = false;

// ==================================================================
// ACCESS POINT
// ==================================================================

void startAccessPoint()
{
    Serial.println();
    Serial.println("========================================");
    Serial.println("       INICIANDO ACCESS POINT");
    Serial.println("========================================");

    WiFi.mode(WIFI_AP);

    bool result = WiFi.softAP(
        AP_SSID,
        AP_PASSWORD
    );

    if (result)
    {
        wifiConnected = false;

        Serial.println("Access Point iniciado!");
        Serial.print("SSID: ");
        Serial.println(AP_SSID);

        Serial.print("Senha: ");
        Serial.println(AP_PASSWORD);

        Serial.print("IP: ");
        Serial.println(WiFi.softAPIP());
    }
    else
    {
        Serial.println("ERRO ao iniciar Access Point!");
    }
}

// ==================================================================
// INICIALIZAÇÃO WI-FI
// ==================================================================

void setupWiFi()
{
    Serial.println();
    Serial.println("========================================");
    Serial.println("             INICIANDO WI-FI");
    Serial.println("========================================");

    WiFi.mode(WIFI_STA);

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );

    Serial.print("Conectando");

    unsigned long inicio = millis();

    while (
        WiFi.status() != WL_CONNECTED &&
        millis() - inicio < 15000
    )
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();

    // --------------------------------------------------------------
    // CONECTADO
    // --------------------------------------------------------------

    if (WiFi.status() == WL_CONNECTED)
    {
        wifiConnected = true;

        Serial.println("Wi-Fi conectado!");

        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());

        Serial.print("IP: ");
        Serial.println(WiFi.localIP());

        Serial.print("Gateway: ");
        Serial.println(WiFi.gatewayIP());

        Serial.print("RSSI: ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
    }

    // --------------------------------------------------------------
    // FALHA
    // --------------------------------------------------------------

    else
    {
        Serial.println("Falha ao conectar ao Wi-Fi.");

        startAccessPoint();
    }
}