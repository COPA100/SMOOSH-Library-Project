#include <LiquidCrystal.h>

// Initialize the LCD library with the numbers of the interface pins
// LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // Set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  
  // Print a message to the LCD
  lcd.print("Number: ");
  lcd.print(42);
}

void loop() {
  // You can update the number here if needed
  // For example, display a counting number:
  /*
  lcd.clear();
  lcd.print("Count: ");
  lcd.print(millis() / 1000); // Display seconds since start
  delay(1000);
  */
}
