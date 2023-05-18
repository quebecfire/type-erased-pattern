//
// Created by vimorin on 2023-05-17.
//

#ifndef TYPE_ERASED_DRAW2_H
#define TYPE_ERASED_DRAW2_H

#include <iostream>

#include "../shape/Circle.h"
#include "../shape/Square.h"

void draw(Circle const &c) {
  std::cout << "Drawing Circle using draw2 library" << std::endl;
  std::cout << "Circle radius: " << c.getRadius() << std::endl << std::endl;
}

void draw(Square const &s) {
  std::cout << "Drawing Square using draw2 library" << std::endl;
  std::cout << "Square side: " << s.getSide() << std::endl << std::endl;
}

#endif  // TYPE_ERASED_DRAW2_H
