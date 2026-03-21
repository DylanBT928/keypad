#include "pico/stdlib.h"
#include "pico_rgb_keypad.hpp"

using namespace pimoroni;

PicoRGBKeypad keypad;

uint16_t maps[5]{ 0xbeb9, 0xe661, 0xb62c, 0x8c1f, 0xa26d };
uint8_t level{ 0 };

void invert_lights(int key)
{
    maps[level] ^= (1 << key);

    // top
    if (key > 3)
    {
        maps[level] ^= 1 << (key - 4);
    }

    // bottom
    if (key < 12)
    {
        maps[level] ^= 1 << (key + 4);
    }

    // left
    if (key % 4 != 0)
    {
        maps[level] ^= 1 << (key - 1);
    }

    // right
    if (key % 4 != 3)
    {
        maps[level] ^= 1 << (key + 1);
    }
}

void win_lights()
{
    for (size_t i{ 0 }; i < 3; ++i)
    {
        for (int key{ 0 }; key < PicoRGBKeypad::NUM_PADS; ++key)
        {
            keypad.illuminate(key, 0x00, 0x55, 0x00);
        }

        keypad.update();
        sleep_ms(200);

        for (int key{ 0 }; key < PicoRGBKeypad::NUM_PADS; ++key)
        {
            keypad.illuminate(key, 0x00, 0x00, 0x00);
        }

        keypad.update();
        sleep_ms(200);
    }
}

int main()
{
    keypad.init();
    keypad.set_brightness(1.0f);

    uint16_t last_button_states{ 0 };

    while (true)
    {
        uint16_t curr_button_states{ keypad.get_button_states() };
        uint16_t new_button_states{ curr_button_states & ~last_button_states };

        last_button_states = curr_button_states;

        if (new_button_states > 0)
        {
            for (int key{ 0 }; key < PicoRGBKeypad::NUM_PADS; ++key)
            {
                // button pressed
                if ((new_button_states >> key) & 0x01)
                {
                    invert_lights(key);

                    // completed level
                    if (maps[level] == 0xffff)
                    {
                        win_lights();
                        ++level;

                        // completed all levels
                        if (level == sizeof(maps) / sizeof(maps[0]))
                        {
                            return 0;
                        }
                    }

                    break;
                }
            }
        }

        for (int key{ 0 }; key < PicoRGBKeypad::NUM_PADS; ++key)
        {
            if ((maps[level] >> key) & 0x01)
            {
                keypad.illuminate(key, 0x55, 0x22, 0x22);
            }
            else
            {
                keypad.illuminate(key, 0x00, 0x00, 0x00);
            }
        }

        keypad.update();
        sleep_ms(100);
    }
}
