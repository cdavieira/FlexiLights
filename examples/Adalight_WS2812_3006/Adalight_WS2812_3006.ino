/*
 * Arduino interface for the use of WS2812 strip LEDs
 * Uses Adalight protocol and is compatible with Boblight, Prismatik etc...
 * "Magic Word" for synchronisation is 'Ada' followed by LED High, Low and Checksum
 * @author: Wifsimster <wifsimster@gmail.com> 
 * @library: FastLED v3.001
 * @date: 11/22/2015
 */
#include "FastLED.h"
#define NUM_LEDS 27
#define LEDS_PER_PANEL 9
#define NUM_PANELS NUM_LEDS/LEDS_PER_PANEL
#define DATA_PIN 5
#define DEFAULT_BRIGHTNESS 20

// Baudrate, higher rate allows faster refresh rate and more LEDs (defined in /etc/boblight.conf)
#define serialRate 9600

// Adalight sends a "Magic Word" (defined in /etc/boblight.conf) before sending the pixel data
uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;

// Initialise LED-array
CRGB leds[NUM_LEDS];

void setup() {
  // Use NEOPIXEL to keep true colors
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
  
  // Initial RGB flash
  LEDS.showColor(CRGB(255, 0, 0));
  delay(500);
  LEDS.showColor(CRGB(0, 255, 0));
  delay(500);
  LEDS.showColor(CRGB(0, 0, 255));
  delay(500);
  LEDS.showColor(CRGB(0, 0, 0));
  
  Serial.begin(serialRate);
  Serial3.begin(serialRate);  
  // Send "Magic Word" string to host
  Serial.print("Ada\n");
}

void loop() { 
  // Wait for first byte of Magic Word

  for(i = 0; i < sizeof prefix; ++i) {
    waitLoop: while (!Serial.available()) ;;
    // Check next byte in Magic Word
    if(prefix[i] == Serial.read()) continue;
    // otherwise, start over
    i = 0;
    goto waitLoop;
  }

  // Hi, Lo, Checksum  
  while (!Serial.available()) ;;
  hi=Serial.read();
  while (!Serial.available()) ;;
  lo=Serial.read();
  while (!Serial.available()) ;;
  chk=Serial.read();
  
  // If checksum does not match go back to wait
  if (chk != (hi ^ lo ^ 0x55)) {
    i=0;
    goto waitLoop;
  }

  memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));

  /*
  // Read the transmission data and set LED values
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  */

  for (uint8_t i = 0; i < NUM_PANELS; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();

    Serial3.print("RGB:");
    Serial3.print(r);
    Serial3.print(g);
    Serial3.println(b);

    for(uint8_t j = 0; j < LEDS_PER_PANEL; j++){
      leds[i*LEDS_PER_PANEL + j].r = r;
      leds[i*LEDS_PER_PANEL + j].g = g;
      leds[i*LEDS_PER_PANEL + j].b = b;
    }
  }
  
  // Shows new values
  FastLED.show();
}
