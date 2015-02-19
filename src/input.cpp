#include "input.h"

bool Input::isDown(std::uint8_t key)
{
  switch(key)
  {
  case 0x1:
    return isKeyPressed(sf::Keyboard::Numpad7);
  case 0x2:
    return isKeyPressed(sf::Keyboard::Numpad8);
  case 0x3:
    return isKeyPressed(sf::Keyboard::Numpad9);
  case 0xC:
    return isKeyPressed(sf::Keyboard::Multiply);
  case 0x4:
    return isKeyPressed(sf::Keyboard::Numpad4);
  case 0x5:
    return isKeyPressed(sf::Keyboard::Numpad5);
  case 0x6:
    return isKeyPressed(sf::Keyboard::Numpad6);
  case 0xD:
    return isKeyPressed(sf::Keyboard::Subtract);
  case 0x7:
    return isKeyPressed(sf::Keyboard::Numpad1);
  case 0x8:
    return isKeyPressed(sf::Keyboard::Numpad2);
  case 0x9:
    return isKeyPressed(sf::Keyboard::Numpad3);
  case 0xE:
    return isKeyPressed(sf::Keyboard::Add);
  case 0xA:
    return isKeyPressed(sf::Keyboard::Right);
  case 0x0:
    return isKeyPressed(sf::Keyboard::Numpad0);
  case 0xB:
    return isKeyPressed(sf::Keyboard::Delete);
  case 0xF:
    return isKeyPressed(sf::Keyboard::Return);
  default:
    return false;
  }
}
