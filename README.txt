# Task do trello associada
* https://trello.com/c/W7gBppC0/5-estudar-o-c%C3%B3digo-do-processing

Para essa tarefa, é solicitado criar um programa que possibilite testar o encoding
feito por um software no computador para acender os leds da fita, de forma a
sincronizar as cores da tela do PC (por exemplo) com as luzes que piscam no led.

A tarefa demanda que o teste feito deve ser independente do Arduino. Portanto, será necessário 
entender que tipo de informação está codificada no sinal enviado pelo software para o Arduino, buscando
entender como usar essa informação para sincronizar as cores mostradas na tela do PC em outro dispositivo.

Essa tarefa tem um ponto importante:
1. Existem 2 alternativas de software que foram encontradas para interpretar as cores da tela do PC
e enviar o sinal necessário para acender os LEDs no Arduino.



# Método 1: Prismatik
Esse é o metodo demonstrado nesse vídeo https://www.youtube.com/watch?v=nzDnr76fZCc

O vídeo é bem explicativo.

O software em si pode ser encontrado aqui: 
https://github.com/psieg/Lightpack/releases

O código usado pelo autor para fazer com que o arduino respondesse ao Lightpack pode ser encontrado aqui:
https://github.com/Wifsimster/adalight_ws2812

Uma explicação breve mas extensiva sobre o Lightpack pode ser encontrada aqui:
https://github.com/Atarity/Lightpack-docs/blob/master/EN/Lightpack_basics.md

Foi necessário fazer algumas modificações quanto a configuração do Lightpack:
* A porta USB conectada ao arduino
* O baudrate (para 9600)

Foi necessário fazer algumas modificações quanto ao código arduino fornecido pelo autor:
* o DATAPIN (de 6 para 5)
* o baudrate (para 9600)
* o número de leds (para 25)

Uma vez subido o código do autor com as devidas alterações no Arduino, espera-se ver a fita de led piscando
vermelho, verde e azul e depois desligando.
Esse é o teste inicial de que o código funciona.
A partir desse ponto, o arduino passa a esperar pelo recebimento de uma palavra chave especial na linha de dados.
O software Prismatik, uma vez executado no computador, passa a enviar dados para o Arduino usando a porta USB configurada.
Esse software roda em background e faz o seguinte:
	1. capta as cores presentes nas bordas da tela do computador
	2. codifica essa informação na forma de um sinal
	3. envia o sinal para o arduino pela linha de dados, que se liga a um pino do arduino
Quando o sinal é recebido pelo arduino, esse inicialmente:
	1. checa se a palavra chave está presente no sinal (em caso afirmativo, segue para 2.)
	2. le os proximos dados presentes e realiza um checksum básico (caso falhe, volta a 1.)
	3. passa a ler e decodificar a informação recebida na forma de cor RGB e acende todos LEDs conforme necessário.

Observação: a fita de LED acende um número N de LEDs corretamente quando controlada pelo código do autor,
no entanto o código feito por nós (que se encontra em https://github.com/paisdegales/ledstrip) acende apenas
até 23 LEDs da fita. Talvez isso possa ocorrer porque o código do autor trata a fita como do tipo NEOPIXEL,
enquanto nosso código trata a fita como sendo WS2811. Ou talvez por alguma questão ligada a corrente.

Para criar um programa que fosse capaz de testar os dados recebidos do programa Prismatik, foi necessário criar um programa que:
	1. recebe os dados provenientes do Software
	2. guarda as informações enviadas aos LEDs em um buffer
	3. printa as informações armazenadas na porta serial ligada ao módulo HC05 (bluetooth)
	4. com um celular, é possível se conectar ao modulo HC05 por bluetooth e receber as informações transmitidas por esse
	Programa baixado da PlayStore para isso: 'Serial Bluetooth Terminal'
O código desse projeto se encontra na pasta Arduino com o nome 'Adalight_WS2812_Decoded'

> Também tentei conectar o meu computador por Bluetooth ao módulo HC05 do Arduino, com intuito receber as informações diretamente no PC,
mas não consegui encontrar o dispositivo pela pesquisa do Bluetooth.

Essa abordagem permitiu entender como os dados são transmitidos do computador para o Arduino, no entanto para construir um programa
que permita armazenar os dados enviados pelo computador e os manipular será necessário interagir com a API do Prismatik e provavelmente
usar plugins.
Esse processo é descrito no README presente no repositorio https://github.com/psieg/Lightpack, no ponto 'Network accessible API (documentation)'
O link presente nesse README redireciona para a página https://github.com/Atarity/Lightpack-docs/blob/master/EN/Prismatik_API.md, onde é explicado
como se conectar em tempo real ao programa do Prismatik enquanto roda em background e tira screenshots da tela.
Por meio dessa API, é possível interromper a captura de tela pelo programa e configurar manualmente os LEDs, bem como também obter os valores atuais
que estão sendo capturados na tela do computador.
Para se conectar ao Prismatik em tempo real, é necessário:
	1. criar um perfil novo no software que faça uso de um LED virtual
	2. na seção 'Expert', habilitar a opção 'Enable Server (API via sockets)'
	3. baixar o cliente telnet no Windows (https://learn.microsoft.com/pt-br/previous-versions/windows/it-pro/windows-server-2008-R2-and-2008/cc771275(v=ws.10))
	4. abrir o terminal e se conectar ao programa via telnet: telnet 127.0.0.1 3636
Programas que fazem uso da API podem ser incorporados ao Prismatik enquanto plugins, que podem ser adicionados na seção 'Plugins' do Prismatik
Os plugins podem ser escritos em diferentes linguagens de programação e muitos exemplos de APIs para linguagens diferentes se encontram disponíveis em:
https://github.com/psieg/Lightpack/tree/master/Software/apiexamples





# Método 2: Processing

Esse método é descrito na forma de um tutorial online que pode ser encontrado em:
https://learn.adafruit.com/adalight-diy-ambient-tv-lighting/overview

As pastas 'Arduino' e 'Processing' devem ser colocadas na pasta $HOME/Documents, logo:
* $HOME/Documents/Arduino
* $HOME/Documents/Processing

A pasta 'Arduino' é automaticamente gerada quando o Arduino IDE é baixado e aberto pela primeira vez.
> lembre-se de baixar a biblioteca FastLED logo após baixa-lo

A pasta 'Processing' é automaticamente gerada quando o Processing é aberto pela primeira vez

Ambas as pastas presentes na Nuvem contém algumas das pastas provenientes desse repositório:
https://github.com/adafruit/Adalight/tree/master

O passo a passo do que copiar e onde colar pode ser encontrado aqui:
https://learn.adafruit.com/adalight-diy-ambient-tv-lighting/download-and-install