/*
 * Arduino interface for the use of WS2812 strip LEDs
 * Uses Adalight protocol and is compatible with Boblight, Prismatik etc...
 * "Magic Word" for synchronisation is 'Ada' followed by LED High, Low and Checksum
 * @author: Wifsimster <wifsimster@gmail.com> 
 * @library: FastLED v3.001
 * @date: 11/22/2015
 */
#include "FastLED.h"
#define NUM_LEDS 25
#define DATA_PIN 5

// Baudrate, higher rate allows faster refresh rate and more LEDs (defined in /etc/boblight.conf)
#define serialRate 9600

#define MAX_READS 100

#define HC05 Serial3

// Adalight sends a "Magic Word" (defined in /etc/boblight.conf) before sending the pixel data
uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;

// Initialise LED-array
CRGB leds[NUM_LEDS];

size_t read_count = 0;
CRGB led_reads[MAX_READS][NUM_LEDS];

void setup() {
  // Use NEOPIXEL to keep true colors
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  
  // Initial RGB flash
  LEDS.showColor(CRGB(255, 0, 0));
  delay(500);
  LEDS.showColor(CRGB(0, 255, 0));
  delay(500);
  LEDS.showColor(CRGB(0, 0, 255));
  delay(500);
  LEDS.showColor(CRGB(0, 0, 0));
  
  Serial.begin(serialRate);
  // Send "Magic Word" string to host
  Serial.print("Ada\n");

	/*	Iniciando as portas seriais do módulo Bluetooth
		O Arduino DUE possui 3 portas seriais além da porta serial 0 e qualquer uma dessas poderia ser usada
		A Serial3 corresponde aos pinos 15(RX3) e 14(TX3) */
  HC05.begin(serialRate);  
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
  
  //memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
  // Read the transmission data and set LED values
  
  /*
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
  if(read_count >= MAX_READS){
    HC05.println("Maximum reads reached! Restarting count...");
    read_count = 0;
    return ;
  }

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    led_reads[read_count][i].r = r;
    led_reads[read_count][i].g = g;
    led_reads[read_count][i].b = b;
  }

  HC05.println("");
  HC05.println(read_count);
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    HC05.print(i);
    HC05.print(", R: ");
    HC05.print(led_reads[read_count][i].r);
    HC05.print(", G: ");
    HC05.print(led_reads[read_count][i].g);
    HC05.print(", B: ");
    HC05.println(led_reads[read_count][i].b);
  }

  read_count++;

  // Shows new values
  //FastLED.show();
}
