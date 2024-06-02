#include <Arduino.h>
#include <LittleFS.h>
#include <I2CScanner.h>
#include <LiquidCrystal_I2C.h>
// #include <CommandHelpers.h>
#include "DeviceConfig.h"
#include "network.h"
#include "webui.h"

DeviceConfig cfg;

I2CScanner scanner;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set address 0x20-0x27 for 16x2 display

void updateDisplayText(const String& line1, const String& line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}

void onSerialCommand(const String& input) {
    if (cfg.exec(input)) {
    }
    else if (input == "show-status") {
        Serial.println("Device MAC Address: " + WiFi.macAddress());
        Serial.println("Connection state: " + (WiFi.status() == WL_CONNECTED
            ? ("connected (" + WiFi.localIP().toString())
            : ("disconnected (" + String(WiFi.status()))) + ')');
    }
    else if (input == "list-networks") {
        const int8_t count = WiFi.scanNetworks();
        for (int8_t n = 0; n < count; n++) {
            Serial.println(WiFi.SSID(n));
        }
    }
    else if (input == "restart") {
        Serial.println("Restarting...");
        delay(2000);
        ESP.restart();
    }
    // else if (CommandHelpers::printOrUpdateValue(input, "webui", webui)){
    // }
    else Serial.println(
        "Commands: show-status, restart, list-networks,\n        show-config, save-config, option?, option=VALUE");
    Serial.println();
}

void onConnectionUpdated(bool connected) {
    if(connected) {
        updateDisplayText("http://" + cfg.hostname, "IP:" + cfg.address.toString());
    }
    else {
        updateDisplayText("http://" + cfg.hostname, "Connection LOST");
    }
    logConnectionStatus(connected);
}

void initPeripherials() {
    scanner.Init(); 
	scanner.Scan();

    lcd.init();
	lcd.backlight();
    updateDisplayText("LCD1602-I2C Test", "5/29/2024");
    delay(1000);
}

void setup(void) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.begin(SERIAL_SPEED);
    Serial.print("Serial initialized, speed: ");
    Serial.println(SERIAL_SPEED);

    initPeripherials(); // I2C-scanner, LCD1602 

    Serial.println("\n\n\n\n\nLCD1602-I2C Test\nInitializing:");
    Serial.print("  filesystem... ");
    if(LittleFS.begin()) {
        Serial.println("OK");
    }
    else {
        Serial.println("FAILED, SYSTEM HALTED");
        while(true);
    }

    Serial.print("  configuration... ");
    Serial.println(cfg.load()
        ? "OK"
        : "FAILED, defaults loaded");
    
    Serial.print("  network... ");
    if (initNetwork(cfg)) {
        Serial.println("OK");
    }
    else {
        Serial.println("FAILED");
        return;
    }

    Serial.print("  connecting... ");
    if (beginConnect(cfg.ssid, cfg.password)) {
        Serial.println("OK");
    }
    else {
        Serial.println("FAILED");
        return;
    }

    initWebUI();
}

void loop(void) {
    if (watchConnection()) {
        loopWebUI();
    }
} 