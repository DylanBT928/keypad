cmake_minimum_required(VERSION 3.17)

include (pico_sdk_import.cmake)

project(keypad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico_w)
pico_sdk_init()

add_executable(keypad main.c)

pico_enable_stdio_usb(keypad 1)
pico_enable_stdio_uart(keypad 1)

pico_add_extra_outputs(keypad)

target_link_libraries(keypad pico_stdlib)

