#ifndef GRAPHIC
#define GRAPHIC

#include <cstdint>
#include <SFML/Graphics.hpp>

class Pixel : sf::RectangleShape
{
private:
  Pixel();
  Pixel(sf::Vector2f position, std::uint8_t size, sf::Color color);

  friend class Screen;
};

class Screen : public sf::Drawable
{
public:
  Screen(std::uint8_t pSize);
  std::uint8_t update(std::uint8_t x, std::uint8_t y);
  std::uint8_t getW();
  std::uint8_t getL();
  void clear();
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
  std::uint8_t pSize;
  static const std::uint8_t width = 64;
  static const std::uint8_t length = 32;
  Pixel pixels[width][length];
};
#endif
