# Arduino LCD Menu System

A lightweight, flexible menu system for Arduino projects using I2C LCD displays. This library makes it easy to create nested menus and handle user navigation with minimal setup required.

## Features

- Simple API for creating menu items and submenus
- Supports up to 50 menu items and 5 levels of depth
- Automatic cursor navigation and display handling
- Custom arrow cursor character for current selection
- Memory-efficient design with dynamic allocation
- Support for callback functions on menu item selection
- Compatible with standard LiquidCrystal_I2C library

## Installation

1. Download this repository as a ZIP file
2. In the Arduino IDE, go to Sketch > Include Library > Add .ZIP Library
3. Select the downloaded ZIP file
4. Restart the Arduino IDE

## Dependencies

Ensure you have the [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) library installed in the Arduino IDE. You can install it via Library Manager.

## Usage

### Basic Setup

```cpp
#include <LiquidCrystal_I2C.h>
#include <MenuSystem.h>

// Initialize LCD (address, columns, rows)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Create root menu
Menu mainMenu(&lcd, 20, 4);

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Build your menu structure
  mainMenu.addItem("Option 1", option1Function);
  mainMenu.addItem("Option 2", option2Function);
  
  // Add a submenu
  Menu* settings = mainMenu.addSubmenu("Settings");
  settings->addItem("Setting 1", setting1Function);
  settings->addItem("Setting 2", setting2Function);
  
  // Display the menu
  mainMenu.show();
}
```

### Navigation

```cpp
// Handle button inputs
void loop() {
  if (upButton.pressed()) {
    currentMenu = currentMenu->up();
  }
  if (downButton.pressed()) {
    currentMenu = currentMenu->down();
  }
  if (selectButton.pressed()) {
    currentMenu = currentMenu->select();
  }
  if (backButton.pressed()) {
    currentMenu = currentMenu->back();
  }
}
```

### Callback Functions

```cpp
void option1Function() {
  // Handle option 1 selection
  Serial.println("Option 1 executed");

}

void setting1Function() {
  // Handle setting 1 selection
  Serial.println("Setting 1 selected!");
}
```

## API Reference

### Menu Class

- `Menu(LiquidCrystal_I2C* lcd, uint8_t lcdCols, uint8_t lcdRows, Menu* parent = nullptr)`
  - Creates a new menu instance
  - Parameters:
    - `lcd`: Pointer to LiquidCrystal_I2C object
    - `lcdCols`: Number of LCD columns
    - `lcdRows`: Number of LCD rows
    - `parent`: Parent menu (optional)

- `bool addItem(const char* label, MenuActionCallback action = nullptr)`
  - Adds a menu item with optional callback function
  - Returns: true if successful, false if menu is full

- `Menu* addSubmenu(const char* label)`
  - Creates and adds a submenu
  - Returns: Pointer to new submenu or nullptr if failed

- `void up()`, `void down()`
  - Navigate through menu items

- `Menu* select()`
  - Select current item
  - Returns: New menu if submenu selected, otherwise current menu

- `Menu* back()`
  - Return to parent menu
  - Returns: Parent menu if exists, otherwise current menu

- `void show()`
  - Display the menu

## Limitations

- Maximum 50 items per menu
- Maximum 5 levels of nested menus
- Menu labels must be string literals or have static lifetime

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This library is released under the MIT License. See the LICENSE file for details.
