#include <cstdint>
#include <ctime>
#include <iostream>
#include "cpu.h"

int main(int argc, char* argv[])
{
  std::srand(std::time(0));
  std::string rom;
  if(argc > 1)
    rom = argv[1];
  else
  {
    std::cout << "Usage: ./Cheap-8 program" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  const std::uint8_t size = 14;
  sf::RenderWindow window(sf::VideoMode(64 * size, 32 * size), "Cheap-8");

  Chip chip8(size);
  chip8.load(rom);
  while(window.isOpen())
  {
    sf::Event event;
    while(window.pollEvent(event))
    {
      if(event.type == sf::Event::Closed)
	window.close();
    }

    chip8.emulate();
    chip8.pause();

    window.clear();
    window.draw(chip8.getScreen());

    window.display();
  }

  return EXIT_SUCCESS;
}
