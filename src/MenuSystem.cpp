#include "MenuSystem.h"

// MenuItem implementations
MenuItem::MenuItem(const char* label, MenuActionCallback action)
    : label(label), action(action), submenu(nullptr) {}

MenuItem::MenuItem(const char* label, Menu* submenu)
    : label(label), action(nullptr), submenu(submenu) {}

MenuItem::~MenuItem() {
    if (submenu) delete submenu;
}

// Menu implementations
Menu::Menu(LiquidCrystal_I2C* lcd, uint8_t lcdCols, uint8_t lcdRows, Menu* parent)
    : items(nullptr), itemCount(0), currentSelection(0),
      lcd(lcd), lcdCols(lcdCols), lcdRows(lcdRows), 
      parentMenu(parent), depth(parent ? parent->depth + 1 : 0),
      lastOperationSuccess(true) {
    
    if (lcd && lcdCols > 0 && lcdRows > 0) {
        lcd->createChar(CURSOR_CHAR, cursorPattern);
        lastOperationSuccess = true;
    } else {
        lastOperationSuccess = false;
    }
}

Menu::~Menu() {
    for (uint8_t i = 0; i < itemCount; i++) {
        delete items[i];
    }
    delete[] items;
}

bool Menu::addItem(const char* label, MenuActionCallback action) {
    if (!label || itemCount >= MAX_ITEMS) {
        lastOperationSuccess = false;
        return false;
    }

    MenuItem** newItems = new MenuItem*[itemCount + 1];
    if (!newItems) {
        lastOperationSuccess = false;
        return false;
    }

    for (uint8_t i = 0; i < itemCount; i++) {
        newItems[i] = items[i];
    }
    
    newItems[itemCount] = new MenuItem(label, action);
    delete[] items;
    items = newItems;
    itemCount++;
    
    lastOperationSuccess = true;
    return true;
}

Menu* Menu::addSubmenu(const char* label) {
    if (!label || itemCount >= MAX_ITEMS || depth >= MAX_DEPTH - 1) {
        lastOperationSuccess = false;
        return nullptr;
    }

    MenuItem** newItems = new MenuItem*[itemCount + 1];
    if (!newItems) {
        lastOperationSuccess = false;
        return nullptr;
    }

    for (uint8_t i = 0; i < itemCount; i++) {
        newItems[i] = items[i];
    }

    Menu* submenu = new Menu(lcd, lcdCols, lcdRows, this);
    newItems[itemCount] = new MenuItem(label, submenu);
    delete[] items;
    items = newItems;
    itemCount++;
    
    lastOperationSuccess = true;
    return submenu;
}

void Menu::up() {
    if (currentSelection > 0) {
        currentSelection--;
        updateDisplay();
    }
}

void Menu::down() {
    if (currentSelection < itemCount - 1) {
        currentSelection++;
        updateDisplay();
    }
}

Menu* Menu::select() {
    if (!items || currentSelection >= itemCount) return this;
    
    MenuItem* selectedItem = items[currentSelection];
    if (!selectedItem) return this;

    if (selectedItem->submenu) {
        lcd->clear();  // Clear screen only when switching to a submenu
        selectedItem->submenu->updateDisplay();
        return selectedItem->submenu;
    } else if (selectedItem->action) {
        selectedItem->action();
    }
    return this;
}


Menu* Menu::back() {
    if (parentMenu) {
        parentMenu->updateDisplay();
        return parentMenu;
    }
    return this;
}

void Menu::show() {
    updateDisplay();
}

void Menu::updateDisplay() {
    if (!isValidLCD()) return;

    uint8_t visibleItems = (itemCount < lcdRows) ? itemCount : lcdRows;
    uint8_t startIdx;
    uint8_t cursorPos;

    if (currentSelection < lcdRows) {
        startIdx = 0;
        cursorPos = currentSelection;
    }
    else {
        uint8_t remainder = currentSelection % lcdRows;
        
        if (remainder == 0) {
            startIdx = currentSelection;
            cursorPos = 0;
        } else {
            startIdx = currentSelection - remainder;
            cursorPos = remainder;
        }
    }

    // Display the visible items
    for (uint8_t i = 0; i < lcdRows; i++) {  // Loop through all LCD rows
        uint8_t itemIndex = startIdx + i;
        
        lcd->setCursor(0, i);
        
        if (itemIndex < itemCount) {
            // Draw cursor
            if (i == cursorPos) {
                lcd->write(CURSOR_CHAR);
            } else {
                lcd->print(" ");
            }

            // Display menu item
            lcd->setCursor(1, i);
            lcd->print(items[itemIndex]->label);
            
            // Clear remaining characters on this line
            for (uint8_t j = strlen(items[itemIndex]->label) + 1; j < lcdCols; j++) {
                lcd->print(" ");
            }
        } else {
            // If no menu item to display, just clear this row
            for (uint8_t j = 0; j < lcdCols; j++) {
                lcd->print(" ");
            }
        }
    }
}
bool Menu::isValidLCD() const {
    return (lcd && lcdCols > 0 && lcdRows > 0);
}

bool Menu::isLastOperationSuccessful() const {
    return lastOperationSuccess;
}