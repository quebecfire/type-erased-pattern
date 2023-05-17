//
// Created by vimorin on 2023-05-17.
//

#ifndef TYPE_ERASED_CIRCLE_H
#define TYPE_ERASED_CIRCLE_H

class Circle
{
public:
    explicit Circle(double rad) : radius{rad}{}

    [[nodiscard]] double getRadius() const noexcept { return radius;}

private:
    double radius;
};

#endif //TYPE_ERASED_CIRCLE_H
