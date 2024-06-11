import lightpack
import time
import datetime

try:
    log_filename = 'log.txt'
    localhost = '127.0.0.1'
    ipv4_addr = localhost
    port = 3636
    max_reads = 50
    delay_seconds = 3
    inicio = datetime.datetime.today()

    lp = lightpack.lightpack(ipv4_addr, port)
    lp.connect()

    with open(log_filename, "w") as output_file, open('replay.txt', 'w') as debug_file:
        output_file.write("LOG DAS LEITURAS FEITAS PELO SOFTWARE PRISMATIK\n")
        output_file.write(100*"#")
        output_file.write(f"\nData e hora de inicio do plugin: {inicio}\n")
        output_file.write(f"Numero maximo de leituras do plugin: {max_reads} leituras\n")
        output_file.write(f"Delay entre leituras consecutivas: {delay_seconds} segundos\n")

        lp.lock()
        count_leds = lp.getCountLeds()
        leds = lp.getLeds()
        lp.unlock()

        output_file.write(f"Numero de leds a nivel de software: {count_leds}\n")
        output_file.write(f"Leds: {leds}\n")
        output_file.write(100*"#")
        debug_file.write(f"{count_leds}\n")
        for i in range(count_leds):
            content = leds[i]
            content = content.replace('-', ',')
            debug_file.write(f"{content}\n");

        timestamp = datetime.datetime.now()
        output_file.write(f"\n\n{timestamp}: Começando a detectar as cores em cada led\n")
        output_file.write("Formato da saída de cada led: {Índice do LED}:{RED},{GREEN},{BLUE}\n\n")

        for i in range(max_reads):
            # Interrompendo brevemente o software, impedindo que envie informações para o Arduino
            lp.lock()
            colors = lp.getColors()
            # colors = 'colors:0-R,G,B;1-R,G,B;2-R,G,B;...'
            # now, removing 'colors:' from string
            colors = colors.split(':')[1]
            # colors = '0-R,G,B;1-R,G,B;2-R,G,B;...'
            # now, obtaining a list of each individual led RGB value
            colors = colors.split(';')
            # colors = ['0-R,G,B', '1-R,G,B', '2-R,G,B', ...]
            colors = list(map(lambda color: color.replace('-', ':'), colors))
            # colors = ['0:R,G,B', '1:R,G,B', '2:R,G,B', ...]
            timestamp = datetime.datetime.now()
            content = f"{timestamp}: leitura {i}: {colors}\n"
            output_file.write(content)
            lp.unlock()
            # Aguardando um pouco enquanto o software controla as cores nos leds
            time.sleep(delay_seconds)
            debug_line = f"{i};" + ";".join(colors)
            debug_file.write(debug_line)

        timestamp = datetime.datetime.now()
        output_file.write(f"\n{timestamp}: Terminando de detectar as cores em cada led")

    lp.disconnect()
except Exception as e:
    with open('error.txt', "w") as error_file:
        error_file.write('Algo de errado ocorreu durante a reprodução do plugin!')
        error_file.write(e)
