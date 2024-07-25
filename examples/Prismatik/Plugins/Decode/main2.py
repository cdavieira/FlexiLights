import lightpack
import time
import datetime

debug_filename = 'debug.txt'
log_filename = 'log.txt'
err_filename = 'error.txt'
localhost = '127.0.0.1'
ipv4_addr = localhost
port = 3636
max_reads = 10
delay_seconds = 3
inicio = datetime.datetime.today()
debug_enable = True
log_enable = True


try:
    lp = lightpack.lightpack(ipv4_addr, port)
    lp.connect()

    with open(debug_filename, "w") as debug_file, open(log_filename, 'w') as log_file:
        if debug_enable: debug_file.write("LOG DAS LEITURAS FEITAS PELO SOFTWARE PRISMATIK\n")
        if debug_enable: debug_file.write(100*"#")
        if debug_enable: debug_file.write(f"\nData e hora de inicio do plugin: {inicio}\n")
        if debug_enable: debug_file.write(f"Numero maximo de leituras do plugin: {max_reads} leituras\n")
        if debug_enable: debug_file.write(f"Delay entre leituras consecutivas: {delay_seconds} segundos\n")

        lp.lock()
        count_leds = lp.getCountLeds()
        leds = lp.getLeds()
        lp.unlock()

        if debug_enable: debug_file.write(f"Numero de leds a nivel de software: {count_leds}\n")
        if debug_enable: debug_file.write(f"Leds: {leds}\n")
        if debug_enable: debug_file.write(100*"#")
        # if log_enable: log_file.write(f"{count_leds}\n")

        for i in range(count_leds):
            content = leds[i]
            content = content.replace('-', ',')
            # if log_enable: log_file.write(f"{content}\n");

        timestamp = datetime.datetime.now()
        if debug_enable: debug_file.write(f"\n\n{timestamp}: Começando a detectar as cores em cada led\n")
        if debug_enable: debug_file.write("Formato da saída de cada led: {Índice do LED}:{RED},{GREEN},{BLUE}\n\n")

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

            if debug_enable:
                timestamp = datetime.datetime.now()
                content = f"{timestamp}: leitura {i}: {colors}\n"
                debug_file.write(content)

            lp.unlock()
            # Aguardando um pouco enquanto o software controla as cores nos leds
            time.sleep(delay_seconds)

            if log_enable:
                #debug_line = f"{i};" + ";".join(colors)
                #log_file.write(debug_line)

                colors = colors[:-1]
                colors = list(map(lambda color:
                                  ";".join(color.split(':')[1].split(',')),
                                  colors))
                # colors = ['R;G;B', 'R;G;B', 'R;G;B', ...]
                for idx, color in enumerate(colors):
                    panel_colors = [color for _ in range(9)]
                    debug_line = f"{idx};" + ";".join(panel_colors) + "\n"
                    log_file.write(debug_line)

        timestamp = datetime.datetime.now()
        if debug_enable: debug_file.write(f"\n{timestamp}: Terminando de detectar as cores em cada led")

    lp.disconnect()
except Exception as e:
    with open(err_filename, "w") as error_file:
        error_file.write('Algo de errado ocorreu durante a reproducao do plugin!')
        error_file.write(e.__repr__())
