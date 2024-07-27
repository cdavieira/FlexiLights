# Processing
O processing corresponde a uma alternativa ao Prismatik no que tange tanto ao
Software de captação e processamento das cores na tela como ao programa
embarcado no Arduino para efetuar o controle da fita de LEDs

A pasta `Colorswirl` contem o código a ser embarcado no Arduino por meio do
software [Processing](https://processing.org/download)

A pasta `Prismatik` contem o código a ser executado por um host para captar as
cores da tela, processa-las e envia-las ao arduino.


## Configuração do ambiente de desenvolvimento

> Esse método é descrito na forma de um [tutorial online pela
Adafruit](https://learn.adafruit.com/adalight-diy-ambient-tv-lighting/download-and-install)

É necessário baixar ao todo 2 Softwares:
* [Arduino IDE](https://www.arduino.cc/en/software)
* [Processing](https://processing.org/download)
> O **Arduino IDE** é um requisito para o funcionamento do **Processing**

Uma vez baixados, é necessário abrir ambos uma vez. Isso fará com que 2 pastas
sejam criadas no diretório raiz do usuário (`$HOME/Documents` no linux e
`$USERPROFILE\Documents` no windows):
* `$HOME/Documents/Arduino`
* `$HOME/Documents/Processing`

O conteúdo das pastas `Arduino` e `Processing` presentes nesse diretório devem
ser então adicionadas nas respectivas pastas
> O código contido nessas pastas foi retirado respectivamente do [repositório
> da Adalight](https://github.com/adafruit/Adalight/tree/master)


## Sobre o Processing
O **Processing** corresponde ao Software a ser executado pelo computador/host.

O código utilizado pelo computador/host é escrito na linguagem de programação
*processing*, que é responsável por ler a tela do computador, identificar as
cores, codifica-las e envia-las ao arduino.

O software do computador envia para o Arduino um buffer, que é composto por:
	1. uma palavra chave (3 bytes)
	2. LED count high byte (1 byte)
	3. LED count low byte (1 byte)
	4. checksum (1 byte)
	5. Pacotes de 3 inteiros representando as cores dos leds (N * 3 bytes, onde N é o número de LEDs em uso)

o LED count high e low byte constroem um número de 2 bytes quando juntos, que
corresponde ao número de LEDs em uso.

O checksum feito está em função desses dois bytes, o que possibilita o código
cliente conferir o checksum reconstruindo-o usando o high e low byte do LED
count.


## Sobre o Arduino
O segundo é o programa que roda no arduino (escrito em processing), que é
responsável por decodificar o sinal proveniente do computador e ativar os
leds.

Seria necessário reescrever o código cliente que é usado no Arduino, uma vez que o
código disponibilizado no tutorial do Adalight é muito antigo e utiliza alguns
recursos não tão apropriados para o ambiente de desenvolvimento utilizado pelo
grupo.


## Vantagens
Uma vantagem do Processing sobre o Prismatik é que se teria mais controle
sobre as informações de RGB que serão enviadas para o Arduino, uma vez que é
possível editar o código fonte diretamente rodado pelo Computador/Host.


## Desvantagens
Uma desvantagem do Processing sobre o Prismatik seria ter que reescrever o
código cliente para o nosso setup do Arduino.


## Conclusão
Como o código cliente a ser embarcado no Arduino teria que ser reescrito
completamente utilizando a linguagem *Processing*, optou-se por seguir com o
Prismatik como o Software a ser usado no computador/host para captar,
processar e enviar as cores para o Arduino.
