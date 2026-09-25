/*
====================================================================
                        SD MANAGER
====================================================================
*/

#include "sd_manager.h"

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

#include "config.h"

// ==================================================================
// ESTADO DO SD
// ==================================================================

bool sdInitialized = false;

// ==================================================================
// INICIALIZAÇÃO DO SD
// ==================================================================

bool setupSD()
{
    Serial.println();
    Serial.println("========================================");
    Serial.println("             INICIANDO MICROSD");
    Serial.println("========================================");

    // Configura SPI
    SPI.begin(
        SD_SCK_PIN,
        SD_MISO_PIN,
        SD_MOSI_PIN,
        SD_CS_PIN
    );

    // Inicializa SD
    if (!SD.begin(SD_CS_PIN, SPI))
    {
        Serial.println("ERRO: Não foi possível inicializar o MicroSD!");

        sdInitialized = false;

        return false;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("ERRO: Nenhum cartão SD detectado!");

        sdInitialized = false;

        return false;
    }

    sdInitialized = true;

    Serial.println("MicroSD inicializado com sucesso!");

    printSDInfo();

    return true;
}

// ==================================================================
// INFORMAÇÕES DO SD
// ==================================================================

void printSDInfo()
{
    if (!sdInitialized)
    {
        Serial.println("SD não inicializado.");
        return;
    }

    uint8_t cardType = SD.cardType();

    Serial.print("Tipo do cartão: ");

    if (cardType == CARD_MMC)
    {
        Serial.println("MMC");
    }
    else if (cardType == CARD_SD)
    {
        Serial.println("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    }
    else
    {
        Serial.println("Desconhecido");
    }

    uint64_t cardSize = SD.cardSize();
    uint64_t totalBytes = SD.totalBytes();
    uint64_t usedBytes = SD.usedBytes();

    Serial.print("Capacidade: ");
    Serial.print((double)cardSize / (1024.0 * 1024.0));
    Serial.println(" MB");

    Serial.print("Espaço total: ");
    Serial.print((double)totalBytes / (1024.0 * 1024.0));
    Serial.println(" MB");

    Serial.print("Espaço utilizado: ");
    Serial.print((double)usedBytes / (1024.0 * 1024.0));
    Serial.println(" MB");

    Serial.print("Espaço livre: ");
    Serial.print(
        (double)(totalBytes - usedBytes) /
        (1024.0 * 1024.0)
    );

    Serial.println(" MB");
}