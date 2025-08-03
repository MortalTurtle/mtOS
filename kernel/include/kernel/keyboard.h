#pragma once
#include <stdint.h>

char inline scancode_to_ascii(uint8_t scancode) {
  static constexpr char code_to_ascii[] = {
      0,    0,   '1',  '2',  '3',  '4', '5', '6', '7', '8', '9',  '0',
      '-',  '=', '\b', '\t', 'q',  'w', 'e', 'r', 't', 'y', 'u',  'i',
      'o',  'p', '[',  ']',  '\n',
      0,  // Control
      'a',  's', 'd',  'f',  'g',  'h', 'j', 'k', 'l', ';', '\'', '`',
      0,  // Left Shift
      '\\', 'z', 'x',  'c',  'v',  'b', 'n', 'm', ',', '.', '/',
      0,  // Right Shift
      '*',
      0,     // Alt
      ' '};  // Space
  return code_to_ascii[scancode];
}
