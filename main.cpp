#include <array>
#include <vector>

#include "Shape.h"

#define USE_LIB_1 true

#if USE_LIB_1
#include "drawing-strategy/draw1.h"
#else
#include "drawing-strategy/draw2.h"
#endif

void drawAllShapes(std::vector<Shape<> > const &shapes) {
  for (auto const &shape : shapes) {
    draw(shape);
  }
}

/**
 * Here you should enjoy the output of the Type-Erased pattern:
 *  1 - No pointers for the user
 *  2 - No manual dynamic allocation
 *  3 - No manual life-time management
 *  4 - Value semantics used
 *  5 - Very simple code ( KISS )
 **/
int main() {
  using Shapes = std::vector<Shape<> >;

  // Creating some shape.
  Shapes shapes{};

  shapes.emplace_back(Circle{2.0}, [](const Circle &c) {
    std::cout << "Drawing Circle using an injected strategy" << std::endl;
    std::cout << "Circle radius: " << c.getRadius() << std::endl << std::endl;
  });

  shapes.emplace_back(Square(5));
  shapes.emplace_back(Circle{4});
  shapes.emplace_back(Square(10));

  // Drawing all shapes
  drawAllShapes(shapes);
  return 0;
}
