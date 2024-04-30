#include <FastLED.h>

/*	HC05 é o nome do módulo bluetooth em uso nesse projeto
	O Arduino DUE possui 3 portas seriais além da porta serial 0 e qualquer uma dessas poderia ser usada
	A Serial3 corresponde aos pinos 15(RX3) e 14(TX3) */
#define HC05 Serial3 
/*	Transmissão de bits por segundo do arduino */
#define BAUD_RATE 9600
/*	Pin do Arduino onde está conectado o fio que transmite os dados a Fita de Led */
#define LED_PIN	 5
/*	Número de leds da fita que devem ser controlados (sequencialmente) */
#define NUM_LEDS	23
/*	Brilho global/default dos leds */
#define DEFAULT_BRIGHTNESS  64
/*	Tipo da fita Led em uso
		Lista de chipsets de Led suportados: http://fastled.io/docs/index.html#autotoc_md2 */
#define LED_TYPE	WS2811
/*	Ordem do código de cores a ser usado pelos Leds
		GRB = Green Red Blue */
#define COLOR_ORDER GRB
/*	Frequencia global/default dos leds, usada em conjunto com UPDATES_PER_SECOND */
#define DEFAULT_FREQUENCY 1000
/*	TODO: REVISAR O USO DESSE MACRO */
#define UPDATES_PER_SECOND 100

/*	Os elementos desse array representam sequencialmente os leds da fita
	Conforme NUM_LEDS aumenta, mais leds da fita podem ser controlados */
CRGB leds[NUM_LEDS];

/*	CRGBPalette16 representa um array de 16 elementos (0, 1, 2, ... 15 indices),
	onde cada elemento desse array corresponde a uma cor do tipo CRGB ou CHSV
	
	O RGBPalette16 é um tipo de array muito prático, pois por mais que contenha
	apenas 16 cores, permite na prática acessar até 256 cores. 
	Isso é feito da seguinte forma: entre cada elemento desse array, são gerados
	15 cores que consistem na transição entre os elementos em questão.
	
	A biblioteca FastLED prove alguns 'preset' palettes:
	1. RainbowColors_p
	2. RainbowStripeColors_p,
	3. OceanColors_p
	4. CloudColors_p
	5. LavaColors_p
	6. ForestColors_p
	7. PartyColors_p
	
	Paletas customizadas também podem ser criadas. Existem diferentes formas de
	fazer isso (de uma olhada aqui: http://fastled.io/docs/_color_palette_8ino-example.html)
	
	Em geral, a função 'CRGBPalette16' parece ser uma boa forma de se criar uma paleta */
CRGBPalette16 currentPalette = RainbowColors_p;

/*	O TBlendType determina a forma como a interpolação entre os elementos de um
	'CRGBPalette16' é feita. Em geral, LINEARBLEND é o que o usuário quer.  */
TBlendType	currentBlending = LINEARBLEND;

/*	Index to be used for indexing 'currentPallete' */
uint8_t randomModeIndex;
uint8_t brightness = DEFAULT_BRIGHTNESS;
unsigned frequency = DEFAULT_FREQUENCY;

/*	Listando todos os modos de operação conhecidos e implementados da fita de LED*/
enum LED_MODE{
	RAINBOW_MODE = 1,
	SINGLE_COLOR_MODE = 2,
	PULSE_MODE = 3,
	BLINK_MODE = 4,
	RANDOM_MODE = 5,
};

/*	Variável que armazena o modo de operação atual do led. Operação inicial escolhida: RANDOM_MODE */
byte led_mode[1] = {RANDOM_MODE};
/*	Variável a ser usada pelo modo piscar para controlar o on/off dos leds */
uint8_t blink_bit;

void setup() {
	/*	Delay de segurança */
	delay(3000);

	/*	Inicializando objeto global FastLED
		Essa variável é uma instância global da classe CFastLED e que está definida na biblioteca FastLED.h
		Sobre a biblioteca FastLED.h: http://fastled.io/docs/_fast_l_e_d_8h.html
		Sobre a classe CFastLED: http://fastled.io/docs/class_c_fast_l_e_d.html */
	FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

	/*	Desligando todos os leds */
	FastLED.clear();
	FastLED.show();

	/*	Setando brilho global */
	FastLED.setBrightness(DEFAULT_BRIGHTNESS);

	/*	Iniciando as portas seriais do Monitor
		No Arduino DUE, a porta serial 0 corresponde aos pinos 0(RX) e 1(TX)
		https://www.arduino.cc/reference/en/language/functions/communication/serial/ */
  Serial.begin(BAUD_RATE);

	/*	Iniciando as portas seriais do módulo Bluetooth
		O Arduino DUE possui 3 portas seriais além da porta serial 0 e qualquer uma dessas poderia ser usada
		A Serial3 corresponde aos pinos 15(RX3) e 14(TX3) */
  HC05.begin(BAUD_RATE);  

	/* Configurando o modo inicial de operação do led (assumindo ser RANDOM_MODE) */
	ChangePalettePeriodically();
}

/* 	o loop desse programa pode ser entendido em 3 etapas:
		Etapa 1: 	apenas acender os LEDs, seja qual for a cor naquele momento de
		cada um deles e esperar um pequeno delay
		Etapa 2:	atualizar/mudar a cor dos LEDs a depender do estado em que a fita se
		encontra
		Etapa 3:	verificar se existem dados que foram enviados pelo modo
		Bluetooth. Caso sim, então ler os dados e mudar o estado da fita de led
		de acordo com os dados que foram enviados.  */	
void loop(){
	/*************** INICIO DA ETAPA 1 ******************/

	/*	Acendendo os LEDs */
	FastLED.show();

	/*	Delay em milisegundos */
	FastLED.delay(frequency / UPDATES_PER_SECOND);

	/*************** FIM DA ETAPA 1 ******************/

	/*************** INICIO DA ETAPA 2 ******************/

	/*	switch de atualização dos leds enquanto um modo de operação estiver selecionado */
	switch(led_mode[0]){
		case RANDOM_MODE:
			/*	Enquanto o modo for aleatório, selecionar uma paleta aleatória de cores */
			ChangePalettePeriodically();
		case RAINBOW_MODE:
			randomModeIndex = randomModeIndex + 1; //motion speed
			FillLEDsFromPaletteColors(randomModeIndex);
			break;
		case SINGLE_COLOR_MODE:
			/*	Simplesmente não há atualização dos leds nesse modo */
			break;
		case PULSE_MODE:
			if(brightness == 0){
				brightness = DEFAULT_BRIGHTNESS;
			}
			FastLED.setBrightness(brightness);
			brightness--;
			break;
		case BLINK_MODE:
			if(blink_bit){
				FastLED.setBrightness(100);
				blink_bit = 0;
			}
			else{
				FastLED.setBrightness(0);
				blink_bit = 1;
			}
			break;
		default:
			Serial.println("Erro ao atualizar modo: modo desconhecido");
			break;
	}

	/*************** FIM DA ETAPA 2 ******************/

	/*************** INICIO DA ETAPA 3 ******************/

	if(!HC05.available()){
		return;
	}

	//	Se chegar aqui, então temos dados disponíveis!

	/*	Dando inicio a rotina de leitura de: obtenção dos dados enviados do aplicativo,
		modificação das cores exibidas nos leds e de outras propriedades da fita,
		como frequencia e brilho, a depender do modo selecionado */
	Serial.println("HC05: dados disponiveis!");

	/*	O próximo modo do led é inicializado com o modo atual como medida de
		precaução para que, caso a operação de leitura do modo falhe, exista um modo
		válido de operação do led para ser usado */
	byte next_led_mode[1] = {led_mode[0]};

	/*	BYTES
		1) https://www.arduino.cc/reference/en/language/functions/communication/serial/readbytes/
		'readBytes' espera 1000 ms (ou o tempo configurado pelo método 'setTimeout')
		e guarda N bytes de dados disponíveis no buffer do HC05 em um array de char ou de bytes */
	if(HC05.readBytes(next_led_mode, 1) != 1){
		Serial.println("HC05: o tempo de espera dos dados na leitura do estado do led expirou");
		return;
	}

	Serial.println("HC05: 1 byte lido com sucesso!");

	/* Se o modo selecionado for o mesmo que o atual, então não há o que mudar */
	if(next_led_mode[0] == led_mode[0]){
		return;
	}

	/*	Variável que guarda o código de cor rgb a ser usado pelos modos de cor
		única, pulsar e piscar
		Declarada aqui, porque variáveis não podem ser declaradas dentro de switchs */
	byte rgb_color_code[3];
	CRGB color;

	/*	switch de mudança de modo de operação dos leds após leitura de dados */
	led_mode[0] = next_led_mode[0];
	switch(led_mode[0]){
		case RAINBOW_MODE:
			Serial.println("Modo Arco Iris selecionado");
			currentPalette = RainbowColors_p;
			frequency = DEFAULT_FREQUENCY;
			FastLED.setBrightness(DEFAULT_BRIGHTNESS);
			break;
		/*	A diferença entre esses estados está no switch que atualiza os leds, não nesse.
		Isso porque o switch de mudança de modo de operação (que consiste na leitura de 3
		bytes para compor a cor RGB) deve ser feito nesses três modos de operação distintos */
		case SINGLE_COLOR_MODE:
		case PULSE_MODE:
		case BLINK_MODE:
			if(led_mode[0] == SINGLE_COLOR_MODE){
				Serial.println("Modo Unicolor selecionado");
				frequency = DEFAULT_FREQUENCY;
			}
			else if(led_mode[0] == PULSE_MODE){
				Serial.println("Modo Pulsar selecionado");
				frequency = DEFAULT_FREQUENCY;
			}
			else{
				Serial.println("Modo Piscar selecionado");
				frequency = 10000;
			}
			while(HC05.readBytes(rgb_color_code, 3) != 3);
			Serial.println("HC05: 3 bytes lidos com sucesso!");
			Serial.print("HC05: Vermelho: ");
			Serial.println(rgb_color_code[0]);
			Serial.print("HC05: Verde: ");
			Serial.println(rgb_color_code[1]);
			Serial.print("HC05: Azul: ");
			Serial.println(rgb_color_code[2]);
			color = CRGB(rgb_color_code[0], rgb_color_code[1], rgb_color_code[2]);
			fill_solid(leds, NUM_LEDS, color);
			FastLED.setBrightness(DEFAULT_BRIGHTNESS);
			break;
		case RANDOM_MODE:
		default:
			if(led_mode[0] != RANDOM_MODE){
				Serial.print("Erro ao mudar de modo: modo desconhecido: ");
				Serial.println(led_mode[0]);
				Serial.println("Selecionando modo aleatorio");
			}
			else{
				Serial.println("Modo aleatorio selecionado");
			}
			led_mode[0] = RANDOM_MODE;
			ChangePalettePeriodically();
			FastLED.setBrightness(DEFAULT_BRIGHTNESS);
			break;
	}

	/*************** FIM DA ETAPA 3 ******************/
}

/*	Preenche cada um dos leds com uma das 256 cores disponíveis na paleta de cores em uso */
void FillLEDsFromPaletteColors(uint8_t colorIndex){
	uint8_t brightness = 255;
	
	for(int i = 0; i < NUM_LEDS; i++) {
		leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
		colorIndex += 3;
	}
}

void ChangePalettePeriodically(){
	uint8_t secondHand = (millis() / 1000) % 60;
	static uint8_t lastSecond = 99;
	
	if(lastSecond != secondHand) {
		lastSecond = secondHand;
		if(secondHand ==  0){
			currentPalette = RainbowColors_p;
			currentBlending = LINEARBLEND;
		}
		if(secondHand == 10){
			currentPalette = RainbowStripeColors_p;
			currentBlending = NOBLEND; 
		}
		if(secondHand == 15){
			currentPalette = RainbowStripeColors_p;
			currentBlending = LINEARBLEND;
		}
		if(secondHand == 20){
			SetupPurpleAndGreenPalette();
			currentBlending = LINEARBLEND;
		}
		if(secondHand == 25){
			SetupTotallyRandomPalette();
			currentBlending = LINEARBLEND;
		}
		if(secondHand == 30){
			SetupBlackAndWhiteStripedPalette();
			currentBlending = NOBLEND;
		}
		if(secondHand == 35){
			SetupBlackAndWhiteStripedPalette();
			currentBlending = LINEARBLEND;
		}
		if(secondHand == 40){
			currentPalette = CloudColors_p;
			currentBlending = LINEARBLEND;
		}
		if(secondHand == 45){
			currentPalette = PartyColors_p;
			currentBlending = LINEARBLEND;
		}
	}
}

void SetupTotallyRandomPalette(){
	for( int i = 0; i < 16; i++){
		currentPalette[i] = CHSV(random8(), 255, random8());
	}
}

void SetupBlackAndWhiteStripedPalette(){
	fill_solid(currentPalette, 16, CRGB::Black);
	currentPalette[0] = CRGB::White;
	currentPalette[4] = CRGB::White;
	currentPalette[8] = CRGB::White;
	currentPalette[12] = CRGB::White;
}

void SetupColorizedStripedPalette(byte R, byte G, byte B){
	fill_solid(currentPalette, 16, CRGB::Black);
	currentPalette[0] = CRGB(R, G, B);
	currentPalette[4] = CRGB(R, G, B);
	currentPalette[8] = CRGB(R, G, B);
	currentPalette[12] = CRGB(R, G, B);
}

void SetupPurpleAndGreenPalette(){
	CRGB purple = CHSV( HUE_PURPLE, 255, 255);
	CRGB green  = CHSV( HUE_GREEN, 255, 255);
	CRGB black  = CRGB::Black;
	
	currentPalette = CRGBPalette16(
								   green,  green,  black,  black,
								   purple, purple, black,  black,
								   green,  green,  black,  black,
								   purple, purple, black,  black );
}

//Não usado ainda (favor não apagar)
/*
void SetupCustomPallete(CRGB (*cores)[16]){
	currentPalette = CRGBPalette16(
								   *cores[0], *cores[1], *cores[2], *cores[3],
								   *cores[4], *cores[5], *cores[6], *cores[7],
								   *cores[8], *cores[9], *cores[10], *cores[11],
								   *cores[12], *cores[13], *cores[14], *cores[15]);
}
*/
