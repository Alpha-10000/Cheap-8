#include <cstdlib>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include "cpu.h"
#include "input.h"

Chip::Chip(std::uint8_t size) : screen(size)
{
  pc = 0x200;
  I = 0;
  opcode = 0;
  for(int i = 0; i < 16; i++)
    V[i] = 0;
  for(int i = 0; i < 80; i++) //0x50=>80
    memory[i] = font[i];
  for(int i = 80; i < 4096; i++)
    memory[i] = 0;
  delay = 0;
  sound = 0;
}

void Chip::load(std::string name)
{
  std::ifstream file(name, std::ios::binary);
  if(file)
  {
    file.seekg(0, file.end);
    const int length = file.tellg();
    file.seekg(0, file.beg);

    std::string in;
    in.resize(length, ' ');
    char* begin = &(*in.begin());
    file.read(begin, length);

    for(int i = 0; i < length; i++)
      memory[i + 512] = in[i]; //0x200=>512

    file.close();
  }
  else
    std::cout << "Error: Impossible to load program." << std::endl;
}

void Chip::emulate()
{
  //Fetch
  opcode = memory[pc] << 8 | memory[pc + 1];
  //std::cout << "Debug: opcode = "<< std::hex << opcode << std::endl;

  std::uint8_t x, y, n, row;
  std::uint16_t xMask(0x0F00), yMask(0x00F0), kMask(0x00FF), nMask(0x0FFF);
  //Decode & Execute
  switch(opcode & 0xF000)
  {
  case 0x0000:
    switch(opcode & 0x000F)
    {
    case 0x0000: //00E0
      screen.clear();
      break;

    case 0x000E: //00EE
      pc = sp.top();
      sp.pop();
      break;

    default: //Unknown
      std::cout << "Unknown opcode: " << opcode << std::endl;
      break;
    }
    break;

  case 0x1000: //0x1NNN
    pc = opcode & nMask;
    break;

  case 0x2000: //0x2NNN
    sp.push(pc);
    pc = opcode & nMask;
    break;

  case 0x3000: //0x3XNN
    if(V[(opcode & xMask) >> 8] == (opcode & kMask))
      pc += 2;
    break;

  case 0x4000: //0x4XNN
    if(V[(opcode & xMask) >> 8] != (opcode & kMask))
      pc += 2;
    break;

  case 0x5000: //0x5XY0
    if(V[(opcode & xMask) >> 8] == V[(opcode & xMask) >> 4])
      pc += 2;
    break;

  case 0x6000: //0x6XNN
    V[(opcode & xMask) >> 8] = opcode & kMask;
    break;

  case 0x7000: //0x7XNN
    V[(opcode & xMask) >> 8] += opcode & kMask;
    break;

  case 0x8000:
    switch(opcode & 0x000F)
    {
    case 0x0000: //0x8XY0
      V[(opcode & xMask) >> 8] = V[(opcode & yMask) >> 4];
      break;

    case 0x0001: //0x8XY1
      V[(opcode & xMask) >> 8] |= V[(opcode & yMask) >> 4];
      break;

    case 0x0002: //0x8XY2
      V[(opcode & xMask) >> 8] &= V[(opcode & yMask) >> 4];
      break;

    case 0x0003: //0x8XY3
      V[(opcode & xMask) >> 8] ^= V[(opcode & yMask) >> 4];
      break;

    case 0x0004: //0x8XY4
      if((V[(opcode & xMask) >> 8] + V[(opcode & yMask) >> 4]) > 0xFF)
	V[0xF] = 1;
      else
	V[0xF] = 0;
      V[(opcode & xMask) >> 8] += V[(opcode & yMask) >> 4];
      break;

    case 0x0005: //0x8XY5
      if(V[(opcode & xMask) >> 8] > V[(opcode & yMask) >> 4])
	V[0xF] = 1;
      else
	V[0xF] = 0;
      V[(opcode & xMask) >> 8] -= V[(opcode & yMask) >> 4];
      break;

    case 0x0006: //0x8XY6
      V[0xF] = V[(opcode & xMask) >> 8] & 0x1;
      V[(opcode & xMask) >> 8] >>= 1;
      break;

    case 0x0007: //0x8XY7
      if(V[(opcode & xMask) >> 8] > V[(opcode & yMask) >> 4])
	V[0xF] = 1;
      else
	V[0xF] = 0;
      V[(opcode & xMask) >> 8]
	= V[(opcode & yMask) >> 4] - V[(opcode & xMask) >> 8];
      break;

    case 0x000E: //0x8XYE
      V[0xF] = (V[(opcode & xMask) >> 8] & 0x80) >> 7; //0x80 => 0x1 << 7
      V[(opcode & xMask) >> 8] <<= 1;
      break;

    default: //Unknown
      std::cout << "Unknown opcode: " << opcode << std::endl;
      break;
    }
  case 0x9000: //0x9XY0
    if(V[opcode & xMask >> 8] != V[opcode & yMask >> 4])
      pc += 2;
    break;

  case 0xA000: //0xANNN
    I = opcode & nMask;
    break;

  case 0xB000: //0xBNNN
    pc = (opcode & nMask) + V[0x0];
    break;

  case 0xC000: //0xCXNN
      V[opcode & xMask >> 8] = (std::rand() % 256) & (opcode & kMask);
    break;

  case 0xD000: //0xDXYN
    x = V[(opcode & xMask) >> 8];
    y = V[opcode & yMask >> 4];
    n = opcode & 0x000F;
    for(std::uint8_t i = 0; i < n; i++)
    {
      row = memory[I + i];
      for(std::uint8_t j = 0; j < 8; j++)
      {
	if((row & (0x80 >> j)) != 0) //test pixel value
	  V[0xF] = screen.update((x+j) % screen.getW(), (y+i) % screen.getL());
	//(pixel) ^ (screen) = (screen) => 0 ^ 1 = 1 ; 0 ^ 0 = 0
	// => if pixel == 0, the screen's pixel don't change
      }
    }
    break;

  case 0xE000:
    switch(opcode & 0x00FF)
    {
    case 0x009E: //0xEX9E
      if(Input::isDown(V[(opcode & xMask) >> 8]))
	pc += 2;
      break;

    case 0x00A1: //0xEXA1
      if(!Input::isDown(V[(opcode & xMask) >> 8]))
	pc += 2;
      break;

    default: //Unknown
      std::cout << "Unknown opcode: " << opcode << std::endl;
      break;
    }
    break;

  case 0xF000:
    switch(opcode & 0x00FF)
    {
    case 0x0007: //0xFX07
      V[(opcode & xMask) >> 8] = delay;
      break;

    case 0x000A: //0xFX0A
      while(true)
      {
	std::cout << "waiting..." << std::endl;
	for(std::uint8_t i = 0; i < 16; i++)
	{
	  //if(keys[i])
	  if(Input::isDown(i))
	  {
	    V[(opcode & xMask) >> 8] = i;
	    goto exit;
	  }
	}
      }
      exit: //Some men just want to watch the world burn :)
      break;

    case 0x0015: //0xFX15
      delay = V[(opcode & xMask) >> 8];
      break;

    case 0x0018: //0xFX18
      sound = V[(opcode & xMask) >> 8];
      break;

    case 0x001E: //0xFX1E
      I += V[(opcode & xMask) >> 8];
      break;

    case 0x0029: //0xFX29
      I = 5 * V[(opcode & xMask) >> 8];
      //Offset from start of memory
      break;

    case 0x0033: //0xFX33
      memory[I] = V[(opcode & xMask) >> 8] / 100;
      memory[I + 1] = V[(opcode & xMask) >> 8] % 100 / 10;
      memory[I + 2] = V[(opcode & xMask) >> 8] % 10;
      break;

    case 0x0055: //0xFX55
      for(int i = 0; i <= ((opcode & xMask) >> 8); i++)
	memory[I + i] = V[i];
      break;

    case 0x0065: //0xFX65
      for(int i = 0; i <= ((opcode & xMask) >> 8); i++)
	V[i] = memory[I + i];
      break;

    default: //Unknown
      std::cout << "Unknown opcode: " << opcode << std::endl;
      break;
    }
    break;

  default: //Unknown opcode
    std::cout << "Unknown opcode: " << opcode << std::endl;
    break;
  }
  pc += 2;

  //Timers
  if(delay > 0)
    delay--;
  if(sound > 0)
  {
    if(sound == 1)
      std::cout << "Biiiip!" << std::endl;
    sound--;
  }
}


void Chip::pause()
{
  std::chrono::milliseconds time(1 / 60);
  std::this_thread::sleep_for(time);
}

const Screen& Chip::getScreen() const
{
  return screen;
}
