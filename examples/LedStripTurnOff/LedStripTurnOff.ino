#include <FastLED.h>

/*	Transmissão de bits por segundo do arduino */
#define BAUD_RATE 9600
/*	Pin do Arduino onde está conectado o fio que transmite os dados a Fita de Led */
#define LED_PIN	 5
/*	Número de leds da fita que devem ser controlados (sequencialmente) */
#define NUM_LEDS	25
/*	Tipo da fita Led em uso
		Lista de chipsets de Led suportados: http://fastled.io/docs/index.html#autotoc_md2 */
#define LED_TYPE WS2811
/*	Ordem do código de cores a ser usado pelos Leds
		GRB = Green Red Blue
    RGB = Red Green Blue */
#define COLOR_ORDER RGB

/*	Os elementos desse array representam sequencialmente os leds da fita
	Conforme NUM_LEDS aumenta, mais leds da fita podem ser controlados */
CRGB leds[NUM_LEDS];

void setup() {
	/*	Delay de segurança */
	delay(1000);

	/*	Inicializando objeto global FastLED
		Essa variável é uma instância global da classe CFastLED e que está definida na biblioteca FastLED.h
		Sobre a biblioteca FastLED.h: http://fastled.io/docs/_fast_l_e_d_8h.html
		Sobre a classe CFastLED: http://fastled.io/docs/class_c_fast_l_e_d.html */
	FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);

	/*	Desligando todos os leds */
	FastLED.clear(true);

	/*	Iniciando as portas seriais do Monitor
		No Arduino DUE, a porta serial 0 corresponde aos pinos 0(RX) e 1(TX)
		https://www.arduino.cc/reference/en/language/functions/communication/serial/ */
  Serial.begin(BAUD_RATE);
}

void loop(){

}
