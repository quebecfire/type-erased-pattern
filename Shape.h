//
// Created by vimorin on 2023-05-17.
//

#ifndef TYPE_ERASED_SHAPE_H
#define TYPE_ERASED_SHAPE_H


#include <memory>
// Wrapper over the external polymorphism pattern
class Shape {
private:
    struct ShapeConcept {
        virtual ~ShapeConcept() = default;

        virtual void do_draw() const = 0;

        // Usage of the prototype design pattern to allow copy
        [[nodiscard]] virtual std::unique_ptr<ShapeConcept> clone() const = 0;
    };

    template<typename GeomShape>
    struct ShapeModel : ShapeConcept {
        ShapeModel(GeomShape shape)
        : shape_{std::move(shape)} {
        }


        void do_draw() const override {
            draw(shape_);
        }

        // Usage of the prototype design pattern to allow copy
        [[nodiscard]] std::unique_ptr<ShapeConcept> clone() const override {
            // Note the use of the copy constructor of the ShapeModel.
            // This will always do the right thing. even if other code changes.
            return std::make_unique<ShapeModel>(*this);
        }

        GeomShape shape_;
    };

    // Free function and injected into the surrounding namespace
    // Not part of the class
    // This friend fct trigger the polymorphic behavior
    // It goes to the pimpl and call the do_draw fct.
    // Doing so, trigger the "real" polymorphic behavior
    friend void draw( Shape const& shape)
    {
        shape.pimpl->do_draw();
    }

    std::unique_ptr<ShapeConcept> pimpl; // Used for the bridge design pattern

public:
    // Templated constructor that take any type of shape and deduce the type
    // This is creating a bridge to some implementation details that the compiler generate

    template< typename GeomShape >
    Shape(GeomShape shape) :
    pimpl{std::make_unique<ShapeModel<GeomShape>>( std::move(shape) )}
    {}

    // Special member functions;
    // Copy
    Shape(Shape const &s)
    : pimpl(s.pimpl->clone())
    {}

    Shape& operator=(Shape const &s)
    {
        // Copy-and-swap idiom
        s.pimpl->clone().swap( pimpl);
        return *this;
    }

    // Default are ok due to the unique_ptr
    // Move will leave an empty shape, remember
    Shape(Shape &&s) = default;
    Shape& operator=(Shape &&s) = default;
};


#endif //TYPE_ERASED_SHAPE_H
