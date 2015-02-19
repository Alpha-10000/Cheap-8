#ifndef INPUT
#define INPUT

#include <cstdint>
#include <SFML/Window.hpp>

class Input : sf::Keyboard
{
public:
  static bool isDown(std::uint8_t key);
};

#endif
