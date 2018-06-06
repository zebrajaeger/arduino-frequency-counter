#include <SPI.h>
#include <Wire.h>

// https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_GFX.h>

// https://github.com/adafruit/Adafruit_SSD1306
#include <Adafruit_SSD1306.h>

// https: //github.com/PaulStoffregen/FreqCount
#include <FreqCount.h>

/* Wiring for Hardware SPI
    GND ------- ground
    VCC ------- 5V
    D0 -------- 13 hardware SCK
    D1 -------- 11 to hardware MOSI
    DC -------- 6 (user choice, defined below)
    DS -------- 7 (user choice, defined below)
    RST ------- 8 (user choice, defined below)
  Hardware MISO (12) unused

  // f_in is pin 5
*/

#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display( OLED_DC, OLED_RESET, OLED_CS );

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);

  FreqCount.begin(1000);
}

void loop() {
  if (FreqCount.available()) {
    unsigned long count = FreqCount.read();
    print_frequency(count);
    Serial.println(count);
  }
}

void print_frequency(unsigned long f) {
  display.clearDisplay();

  display.setCursor(0, 0);

  char buf[10];
  ltoa(f, buf, 10);
  uint8_t l = strlen((char*)&buf);

  uint8_t space = 10 - l;
  if (l > 3) --space;
  if (l > 6) --space;
  for (uint8_t i = 0; i < space; ++i) {
    display.write(' ');
  }

  char c = 1;
  for (uint8_t i = 0; c != 0; ++i, --l) {
    c = buf[i];
    if (i != 0 && (l == 6 || l == 3)) {
      display.write('.');
    }
    display.write(c);
  }
  display.setCursor(96, 16);
  display.write('H');
  display.write('z');
  display.display();
}


