# Arduino

## Códigos cliente
Os códigos embarcados no Arduino estão contidos nas pastas presentes nesse
diretório.

Cada um desses teve um propósito de uso durante a fase de desenvolvimento da
solução de software.

Alguns racionais por trás de cada um desses códigos é apresentado a seguir:

* `FlexiLights`

Código final que efetua o controle das cores nos painéis de led

* `Adalight_WS2812`

Código que efetua um controle simplificado da fita de led, a qual não estaria
ainda posicionada nos painéis de LED.

* `Adalight_WS2812_Decoded`

Código utilizado para conferir os valores RGB lidos/recebidos pelo controlador
Arduino durante a fase de desenvolvimento desse programa.

* `LedStripTurnOff`

Código utilizado para desligar completamente os leds da fita caso esses
permaneçam ligados após o código de controle do Arduino ter sido
substituido por outro.
