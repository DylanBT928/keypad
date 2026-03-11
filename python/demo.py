import picokeypad
import utime

keypad = picokeypad.PicoKeypad()
keypad.set_brightness(1.0)

lit = 0
last_button_states = 0
color_index = 0

num_pads = keypad.get_num_pads()

while True:
    button_states = keypad.get_button_states()
    if last_button_states != button_states:
        last_button_states = button_states

        if button_states > 0:
            if lit == 0xffff:
                lit = 0
                color_index += 1

                if color_index >= 6:
                    color_index = 0
            else:
                button = 0

                for _ in range(num_pads):
                    if button_states & 0x01 > 0:
                        if not (button_states & (~0x01)) > 0:
                            lit = lit | (1 << button)
                        break

                    button_states >>= 1
                    button += 1

    for i in range(num_pads):
        if (lit >> i) & 0x01:
            if color_index == 0:
                keypad.illuminate(i, 0x00, 0x20, 0x00)
            elif color_index == 1:
                keypad.illuminate(i, 0x20, 0x20, 0x00)
            elif color_index == 2:
                keypad.illuminate(i, 0x20, 0x00, 0x00)
            elif color_index == 3:
                keypad.illuminate(i, 0x20, 0x00, 0x20)
            elif color_index == 4:
                keypad.illuminate(i, 0x00, 0x00, 0x20)
            elif color_index == 5:
                keypad.illuminate(i, 0x00, 0x20, 0x20)
        else:
            keypad.illuminate(i, 0x05, 0x05, 0x05)

    keypad.update()
    utime.sleep(0.1)

