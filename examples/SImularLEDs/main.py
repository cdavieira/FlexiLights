"""
formato do arquivo de entrada:
numero de leds
indice led,coordenada x,coordenada y,largura,altura
...
indice leitura;0:R,G,B;1:R,G,B;...
...
"""

from pygame.display import set_mode, update
from pygame.surface import Surface
from pygame.rect import Rect
from pygame.sprite import Sprite, Group
from pygame.color import Color
from pygame.event import get as get_event_queue
from pygame.constants import QUIT, KEYDOWN, K_LEFT, K_RIGHT, TEXTINPUT

class LedPanel(Sprite):
    def __init__(self, x, y, w, h, *groups):
        super().__init__(*groups)
        self.image = Surface([w, h])
        self.rect = self.image.get_rect().topleft = (x, y)

class LedStripDebugger:
    def __init__(self, number_leds: int):
        self.number_leds = number_leds
        self.leds_info: list[int,int,int,int,int] = list()
        self.reads: list[tuple[int, list[int,int,int,int]]] = list()
        self.sprites: list[LedPanel] = list()
        self.group: Group = Group()

    def position_squares(self) -> None:
        for led_info in self.leds_info:
            idx, x, y, w, h = led_info
            sprite = LedPanel(x, y, w, h, self.group)
            self.sprites.append(sprite)

    def set_state(self, read_idx: int) -> None:
        read_info = self.reads[read_idx]
        idx, leds_state = read_info
        for led_state in leds_state:
            led_idx, r, g, b = led_state
            sprite = self.sprites[led_idx]
            rgb = Color(r, g, b)
            sprite.image.fill(rgb)

    def list_states(self) -> None:
        for rread in self.reads:
            idx, leds_state = rread
            print("read", idx)
            for led_state in leds_state:
                led_idx, r, g, b = led_state
                print("led", led_idx, ":", r, g, b)

    def draw_window(self, window: Surface) -> None:
        rects = self.group.draw(window)
        update()


def read_input_data(filename: str) -> LedStripDebugger:
    try:
        with open(filename, "r") as file:
            # reading first line: the number os leds used
            led_count = file.readline()
            led_count = int(led_count)

            # instantiating our debugger class
            debugger = LedStripDebugger(led_count)

            # reading following <led_count> lines
            for i in range(led_count):
                # read information about led position on screen
                led_info = file.readline()
                led_info = led_info.replace('\n', '')
                led_info = led_info.replace('\r', '')
                # led_info = 'index,x,y,width,height'
                led_info = led_info.split(',')
                # led_info = ['index','x','y','width','height']
                led_info = list(map(lambda string_value: int(string_value), led_info))
                # led_info = [index,x,y,width,height]
                debugger.leds_info.append(led_info)

            # reading all remaining lines
            # these lines are thought to be reads about the color state of each led
            remaining_lines = file.readlines()
            remaining_lines = remaining_lines[:-1]
            for line in remaining_lines:
                if line.isspace():
                    continue
                # line format: read index;0:R,G,B;1:R,G,B;...
                line = line.split(';')
                line = line[:-1]
                # line = ['read_index', '0:R,G,B', '1:R,G,B', ...]
                read_index = int(line[0])
                leds_state = line[1:]
                # leds_state = ['0:R,G,B', '1:R,G,B', ...]
                states: list[int,int,int,int] = []
                for led_state in leds_state:
                    led_state = led_state.split(':')
                    # led_state = ['0', 'R,G,B']
                    led_index = int(led_state[0])
                    led_colors = led_state[1]
                    # led_colors = 'R,G,B'
                    led_colors = led_colors.split(',')
                    # led_colors = ['R', 'G', 'B']
                    r, g, b = int(led_colors[0]), int(led_colors[1]), int(led_colors[2])
                    state = [led_index, r, g, b]
                    states.append(state)
                read: tuple[int, list[int, int, int, int]] = (read_index, states)
                debugger.reads.append(read)
        return debugger
    except Exception as e:
        print(e)
        raise e

debugger = read_input_data('replay3.txt')
debugger.position_squares()
debugger.list_states()
window = set_mode()
state_idx = 0
debugger.set_state(state_idx)
debugger.draw_window(window)
while True:
    for event in get_event_queue():
        if event.type == QUIT:
            exit()
        elif event.type == KEYDOWN:
            if event.key not in (K_LEFT, K_RIGHT):
                continue
            if event.key == K_RIGHT and state_idx < len(debugger.reads):
                state_idx += 1
            elif event.key == K_LEFT and state_idx > 0:
                state_idx -= 1
            print("Current state:", state_idx)
            debugger.set_state(state_idx)
            debugger.draw_window(window)
