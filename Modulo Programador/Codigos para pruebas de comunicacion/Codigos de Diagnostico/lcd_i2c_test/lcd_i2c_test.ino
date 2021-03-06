#include <LiquidCrystal_I2C.h>
 
 
 
// Construct an LCD object and pass it the 
// I2C address, width (in characters) and
// height (in characters). Depending on the
// Actual device, the IC2 address may change.
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
void setup() {
 
 
  lcd.init();
 
  // Turn on the backlight.
  lcd.backlight();
 
  // Move the cursor 5 characters to the right and
  // zero characters down (line 1).
  lcd.setCursor(0, 0);
 
  // Print HELLO to the screen, starting at 5,0.
  lcd.print("Hugo");
 
  // Move the cursor to the next line and print
  // WORLD.
  lcd.setCursor(0, 1);      
  lcd.print("Gay");
}
 
void loop() {
}
