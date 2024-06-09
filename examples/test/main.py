import lightpack
import time

lp = lightpack.lightpack('127.0.0.1', 3636)
lp.connect()
lp.lock()

count_leds = lp.getCountLeds()
leds = lp.getLeds()
with open("data.txt", "w") as output_file:
    content = "{}\n{}".format(count_leds, leds)
    output_file.write(content)
time.sleep(3)

lp.disconnect()
