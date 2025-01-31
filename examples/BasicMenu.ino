/**
 * @file BasicMenu.ino
 * @brief Example usage of the MenuSystem library with an LCD and button navigation.
 *
 * This example demonstrates how to implement a multi-level menu system using
 * the MenuSystem library with an I2C LCD display and button controls. The menu
 * supports navigation, selection of actions, and returning to previous menus.
 *
 * @author Aditya Shandilya
 * @date 2025-01-31
 * @version 1.0
 * @license MIT
 *
 * @hardware
 * - Arduino Board (e.g., Uno, Mega, ESP32, etc.)
 * - 16x2 LCD with I2C interface or 20x4 LCD with I2C interface
 * - 4 push buttons for navigation
 *
 * @connections
 * - LCD I2C to Arduino (SDA -> A4, SCL -> A5 for Uno)
 * - SELECT_BUTTON to Digital Pin 2
 * - UP_BUTTON to Digital Pin 3
 * - DOWN_BUTTON to Digital Pin 4
 * - BACK_BUTTON to Digital Pin 5
 */
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MenuSystem.h>

// Define button pins
#define SELECT_BUTTON 2
#define UP_BUTTON 3
#define DOWN_BUTTON 4
#define BACK_BUTTON 5

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
// LiquidCrystal_I2C lcd(0x27, 20, 4);  // For 20x4 LCD

// Create main menu instance
Menu mainMenu(&lcd, 16, 2);
// Menu mainMenu(&lcd, 20, 4);  // For 20x4 LCD
Menu* currentMenu = &mainMenu;

// Example actions
void action1() {
    Serial.println("Action 1 executed");
}

void action2() {
    Serial.println("Action 2 executed");
}

void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();
    
    pinMode(UP_BUTTON, INPUT_PULLUP);
    pinMode(DOWN_BUTTON, INPUT_PULLUP);
    pinMode(SELECT_BUTTON, INPUT_PULLUP);
    pinMode(BACK_BUTTON, INPUT_PULLUP);

    // Setup menu structure
    mainMenu.addItem("Option 1", action1);
    mainMenu.addItem("Option 2", action2);
    Menu* subMenu1 = mainMenu.addSubmenu("Submenu 1");
    subMenu1->addItem("SubItem 1", action1);
    subMenu1->addItem("SubItem 2", action2);
    Menu* subMenu2 = mainMenu.addSubmenu("Submenu 2");
    subMenu2->addItem("SubItem 1", action1);
    subMenu2->addItem("SubItem 2", action2);
    
    currentMenu->show();
}

void loop() {
    static unsigned long lastButtonPress = 0;
    unsigned long currentMillis = millis();
    const unsigned long debounceDelay = 200;

    if (currentMillis - lastButtonPress > debounceDelay) {
        if (digitalRead(UP_BUTTON) == LOW) {
            currentMenu->up();
            lastButtonPress = currentMillis;
        }
        if (digitalRead(DOWN_BUTTON) == LOW) {
            currentMenu->down();
            lastButtonPress = currentMillis;
        }
        if (digitalRead(SELECT_BUTTON) == LOW) {
            currentMenu = currentMenu->select();
            lastButtonPress = currentMillis;
        }
        if (digitalRead(BACK_BUTTON) == LOW) {
            currentMenu = currentMenu->back();
            lastButtonPress = currentMillis;
        }
    }
}