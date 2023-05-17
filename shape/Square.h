//
// Created by vimorin on 2023-05-17.
//

#ifndef TYPE_ERASED_SQUARE_H
#define TYPE_ERASED_SQUARE_H

class Square
{
public:
    explicit Square(double s) : side{s}{}

    [[nodiscard]] double getSide() const noexcept { return side;}

private:
    double side;
};

#endif //TYPE_ERASED_SQUARE_H
