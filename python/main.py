import picokeypad
import utime
import sys

keypad = picokeypad.PicoKeypad()
keypad.set_brightness(1.0)
keys = keypad.get_num_pads()

maps = [0xbeb9, 0xe661]
level = 0

last_button_states = 0;


def invert_lights(key):
    maps[level] = maps[level] ^ (1 << key)
    # top
    if key > 3:
        maps[level] = maps[level] ^ (1 << (key - 4))

    # bottom
    if key < 12:
        maps[level] = maps[level] ^ (1 << (key + 4))

    # left
    if key % 4 != 0:
        maps[level] = maps[level] ^ (1 << (key - 1))

    # right
    if key % 4 != 3:
        maps[level] = maps[level] ^ (1 << (key + 1))


def win_lights():
    for _ in range(3):
        for key in range(keys):
            keypad.illuminate(key, 0x00, 0x55, 0x00)

        keypad.update()
        utime.sleep(0.2)

        for key in range(keys):
            keypad.illuminate(key, 0x00, 0x00, 0x00)

        keypad.update()
        utime.sleep(0.2)


while True:
    curr_button_states = keypad.get_button_states()

    if curr_button_states != last_button_states:
        last_button_states = curr_button_states

        if curr_button_states > 0:
            for key in range(keys):

                # button pressed
                if curr_button_states & 0x01 > 0:
                    invert_lights(key)

                    # completed level
                    if maps[level] == 0xffff:
                        win_lights()
                        level += 1

                        # completed all levels
                        if level == len(maps):
                            sys.exit()

                    break

                curr_button_states >>= 1

    # light matrix onto keypad
    for key in range(keys):
        if (maps[level] >> key) & 0x01:
            keypad.illuminate(key, 0x55, 0x00, 0x22)
        else:
            keypad.illuminate(key, 0x00, 0x00, 0x00)

    keypad.update()
    utime.sleep(0.1)
