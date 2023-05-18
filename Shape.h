//
// Created by vimorin on 2023-05-17.
//

#ifndef TYPE_ERASED_SHAPE_H
#define TYPE_ERASED_SHAPE_H

#include <memory>
// Wrapper over the external polymorphism pattern
template <size_t buffersize = 128UL, size_t alignment = 16UL>
class Shape {
 private:
  struct ShapeConcept {
    virtual ~ShapeConcept() = default;

    virtual void do_draw() const = 0;
    virtual void clone(
        ShapeConcept *memory) const = 0;  // Still prototype design pattern
    virtual void move(ShapeConcept *memory) const = 0;
  };

  template <typename GeomShape>
  struct ShapeModel : ShapeConcept {
    explicit ShapeModel(GeomShape shape) : shape_{std::move(shape)} {}

    void do_draw() const override { draw(shape_); }

    // Note the explicit use of the global placement new operator.
    // This is recommended to avoid overloads of placement new.
    void clone(ShapeConcept *memory)
        const override {                 // Still prototype design pattern
      ::new (memory) ShapeModel(*this);  // Global placement new call
    }

    void move(ShapeConcept *memory) const override {
      ::new (memory) ShapeModel(std::move(*this));
    }

    GeomShape shape_;
  };

  template <typename GeomShape, typename DrawStrategy>
  struct ExtendedShapeModel : ShapeConcept {
    explicit ExtendedShapeModel(GeomShape shape, DrawStrategy drawer)
        : shape_{std::move(shape)}, drawer_{std::move(drawer)} {}

    void do_draw() const override { drawer_(shape_); }

    // Note the explicit use of the global placement new operator.
    // This is recommended to avoid overloads of placement new.
    void clone(ShapeConcept *memory)
        const override {  // Still prototype design pattern
      ::new (memory) ExtendedShapeModel(*this);  // Global placement new call
    }

    void move(ShapeConcept *memory) const override {
      ::new (memory) ExtendedShapeModel(std::move(*this));
    }

    GeomShape shape_;
    DrawStrategy drawer_;
  };

  // Free function and injected into the surrounding namespace
  // Not part of the class
  // This friend fct trigger the polymorphic behavior
  // It goes to the pimpl and call the do_draw fct.
  // Doing so, trigger the "real" polymorphic behavior
  friend void draw(Shape const &shape) { shape.pimpl()->do_draw(); }

  [[nodiscard]] ShapeConcept *pimpl() noexcept  // Bridge design pattern
  {
    return reinterpret_cast<ShapeConcept *>(buffer.data());
  }

  [[nodiscard]] const ShapeConcept *pimpl()
      const noexcept  // Bridge design pattern
  {
    return reinterpret_cast<const ShapeConcept *>(buffer.data());
  }

  // constexpr specifier declares that it is possible to evaluate the value of
  // the function or variable at compile time static specify the lifetime of
  // the variable
  //    static constexpr size_t buffersize = 128UL;
  //    static constexpr size_t alignment = 16UL;

  alignas(alignment)
      std::array<std::byte, buffersize> buffer;  // Bridge design pattern

 public:
  // Templated constructor that take any type of shape and deduce the type
  // This is creating a bridge to some implementation details that the compiler
  // generate

  template <typename GeomShape>
  explicit Shape(GeomShape shape) {
    using M = ShapeModel<GeomShape>;
    static_assert(sizeof(M) <= buffersize, "Given type is too large");
    static_assert(alignof(M) <= alignment, "Given type is overaligned");
    ::new (pimpl())
        M(std::move(shape));  // Construct a `ShapeModel<GeomShape>` object,
                              // placing it directly into your pre-allocated
                              // storage at memory address `buffer.data()`
  }

  template <typename GeomShape, typename DrawStrategy>
  explicit Shape(
      GeomShape shape,
      DrawStrategy drawer)  // Here we created a point of dependency injection
  {
    using M = ExtendedShapeModel<GeomShape, DrawStrategy>;
    static_assert(sizeof(M) <= buffersize, "Given type is too large");
    static_assert(alignof(M) <= alignment, "Given type is overaligned");
    ::new (pimpl())
        M(std::move(shape),
          std::move(drawer));  // Construct a
                               // `ExtendedShapeModel<GeomShape,DrawStrategy>`
                               // object, placing it directly into your
    // pre-allocated storage at memory address `buffer.data()`
  }

  // We are now required to perform the destruction of shapes manually,
  // since the compiler only sees an array of bytes
  ~Shape() { pimpl()->~ShapeConcept(); }

  // Special member functions;
  // Copy
  Shape(const Shape &s) { s.pimpl()->clone(pimpl()); }

  Shape &operator=(Shape const &s) {
    // Copy-and-swap idiom
    Shape copy(s);
    buffer.swap(copy.buffer);
    return *this;
  }

  // Default are ok due to the unique_ptr
  // Move will leave an empty shape, remember
  Shape(Shape &&s) = default;
  Shape &operator=(Shape &&s) = default;
};

#endif  // TYPE_ERASED_SHAPE_H
