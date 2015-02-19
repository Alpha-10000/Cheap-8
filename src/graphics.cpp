#include "graphics.h"

Pixel::Pixel(){}

Pixel::Pixel(sf::Vector2f position, std::uint8_t size, sf::Color color)
{
  setPosition(position);
  setSize(sf::Vector2f(size, size));
  setFillColor(color);
}

Screen::Screen(std::uint8_t pSize)
{
  for(std::uint8_t x = 0; x < width; x++)
    for(std::uint8_t y = 0; y < length; y++)
      pixels[x][y]
	= Pixel(sf::Vector2f(x*pSize, y*pSize), pSize, sf::Color::Black);
  this->pSize = pSize;
}

std::uint8_t Screen::update(std::uint8_t x, std::uint8_t y)
{
  if(pixels[x][y].getFillColor() == sf::Color::Black)
  {
    pixels[x][y].setFillColor(sf::Color::White); //1
    return 0;
  }
  else
  {
    pixels[x][y].setFillColor(sf::Color::Black); //0
    return 1; //From white to black, pixel cleared
  }
}

std::uint8_t Screen::getW()
{
  return width;
}

std::uint8_t Screen::getL()
{
  return length;
}

void Screen::clear()
{
  for(std::uint8_t x = 0; x < width; x++)
    for(std::uint8_t y = 0; y < length; y++)
      pixels[x][y]
	= Pixel(sf::Vector2f(x*pSize, y*pSize), pSize, sf::Color::Black);
}

void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  for(std::uint8_t x = 0; x < width; x++)
    for(std::uint8_t y = 0; y < length; y++)
      target.draw(pixels[x][y], states);
}
