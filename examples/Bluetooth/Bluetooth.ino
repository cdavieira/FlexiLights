#define HC05 Serial3 
#define BAUD_RATE 9600

/*
	A comunicação bluetooth é feita por meio de um módulo aparte do arduino,
	chamado HC05 FC-114.

	No caso desse projeto, está sendo utilizado o arduino DUE, que possui pinos
	RX e TX expostos pelo Serial3

	https://www.arduino.cc/reference/en/language/functions/communication/serial/
	https://forum.arduino.cc/t/arduino-due-bluetooth-connection/450560
	https://www.martyncurrey.com/hc-05-fc-114-and-hc-06-fc-114-part-2-basic-at-commands/
	http://arduino-er.blogspot.com/2015/07/connect-arduino-due-with-hc-06.html
*/

void setup() {
  Serial.begin(BAUD_RATE);
  HC05.begin(BAUD_RATE);
  Serial.println("Ready!");
}

void loop() {
  if(Serial.available()){
    Serial.println("Data incoming from HC05 to Arduino!");
  }

  if(HC05.available()){
    Serial.println("HC05 has data available!");
		char data;

    while(HC05.available()){
      data = HC05.read();
      Serial.write(data);
      HC05.write(data);
    }
  }
}
