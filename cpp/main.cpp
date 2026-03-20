#include "pico/stdlib.h"
#include "pico_rgb_keypad.hpp"

using namespace pimoroni;

PicoRGBKeypad keypad;

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

constexpr uint8_t num_colors{ 6 };

Color colors[num_colors]{
    { 0x00, 0x20, 0x00 },  // green
    { 0x20, 0x20, 0x00 },  // yellow
    { 0x20, 0x00, 0x00 },  // red
    { 0x20, 0x00, 0x20 },  // pink
    { 0x00, 0x00, 0x20 },  // blue
    { 0x00, 0x20, 0x20 }   // teal
};

int main()
{
    keypad.init();
    keypad.set_brightness(1.0f);

    uint16_t lit{ 0 };
    uint8_t color_index{ 0 };
    Color curr_color{ colors[color_index] };

    while (true)
    {
        lit |= keypad.get_button_states();

        for (size_t i{ 0 }; i < PicoRGBKeypad::NUM_PADS; ++i)
        {
            if (lit & (0x1 << i))
            {
                keypad.illuminate(i, curr_color.r, curr_color.g, curr_color.b);
            }
            else
            {
                keypad.illuminate(i, 0x05, 0x05, 0x05);
            }
        }

        keypad.update();
        sleep_ms(100);

        if (lit == 0xffff)
        {
            sleep_ms(500);
            lit = 0;
            ++color_index;

            if (color_index == num_colors)
            {
                color_index = 0;
            }

            curr_color = colors[color_index];
        }
    }
}
