#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class Menu;  // Forward declaration

// Function pointer for menu actions
typedef void (*MenuActionCallback)();

// Menu item class - users don't need to interact with this directly
class MenuItem {
    friend class Menu;
private:
    const char* label;
    MenuActionCallback action;
    Menu* submenu;
    
    MenuItem(const char* label, MenuActionCallback action = nullptr);
    MenuItem(const char* label, Menu* submenu);
    ~MenuItem();
};

class Menu {
public:
    // Constructor - just needs the LCD object and screen dimensions
    Menu(LiquidCrystal_I2C* lcd, uint8_t lcdCols, uint8_t lcdRows, Menu* parent = nullptr);
    ~Menu();

    // Simple interface for adding menu items
    bool addItem(const char* label, MenuActionCallback action = nullptr);
    Menu* addSubmenu(const char* label);

    // Navigation methods - these handle all display updates automatically
    void up();      // Move cursor up
    void down();    // Move cursor down
    Menu* select(); // Select current item - returns new menu if submenu selected
    Menu* back();   // Go back to parent menu

    // Call this once at the start to show the menu
    void show();

    // Optional: Check if last operation was successful
    bool isLastOperationSuccessful() const;

private:
    static const uint8_t MAX_ITEMS = 50;
    static const uint8_t MAX_DEPTH = 5;
    
    MenuItem** items;
    uint8_t itemCount;
    uint8_t currentSelection;
    LiquidCrystal_I2C* lcd;
    uint8_t lcdCols;
    uint8_t lcdRows;
    Menu* parentMenu;
    uint8_t depth;
    bool lastOperationSuccess;
    
    const byte CURSOR_CHAR = 0;
    byte cursorPattern[8] = {
        B11000, B11100, B11110, B11111,
        B11111, B11110, B11100, B11000
    };
    const char* submenuIndicator = ">";

    void updateDisplay();
    bool isValidLCD() const;
};

#endif