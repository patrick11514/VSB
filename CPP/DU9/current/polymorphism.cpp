#include <memory>

/*
 * This file demonstrates the CRTP (curiously recurring template parameter) technique.
 * It can be used to implement static polymorphism without the overhead of virtual calls.
 *
 * CRTP happens when a class A derives from a base class, and provides itself as a template argument
 * for the parent class. With this approach, the parent knows the
 */

// Imagine that we want to provide shared drawing functionality for multiple image types
// (PNG, SVG, ...) and we want to share that implementation by inheriting from a base class.
// With virtual funtions, we would have unnecessary overhead.
class DrawerDynamic {
public:
    virtual ~DrawerDynamic() = default;

    virtual void draw_line(int x1, int y1, int x2, int y2) = 0;

    void draw_house(int x, int y, int size) {
        this->draw_line(x, y, x + size, y + size); // requires dynamic dispatch -> vtable call
    }

    // Here we cannot use covariance (cannot set the return type to std::unique_ptr<Child> in a child
    // class), because we return a unique ptr and not DrawerDynamic* directly!
    virtual std::unique_ptr<DrawerDynamic> copy() = 0;
};
class PngDrawerDynamic: public DrawerDynamic {
public:
    std::unique_ptr<DrawerDynamic> copy() override {
        return std::make_unique<PngDrawerDynamic>();
    }

    void draw_line(int x1, int y1, int x2, int y2) override {}
};
class SvgDrawerDynamic: public DrawerDynamic {
public:
    std::unique_ptr<DrawerDynamic> copy() override {
        return std::make_unique<SvgDrawerDynamic>();
    }

    void draw_line(int x1, int y1, int x2, int y2) override {}
};


// If you want to put Drawer in a collection (e.g. std::vector), you can introduce another parent
// class without a template. If you need a shared interface for this class, you have to use
// virtual methods again.
class DrawerBase {};

// We can achieve this effect without virtual call overhead by using CRTP.
template <typename Child>
class Drawer: public DrawerBase {
public:
    virtual ~Drawer() = default;
    void draw_house(int x, int y, int size) {
        auto child = static_cast<Child*>(this);
        // no dynamic dispatch, because we know the child type exactly!
        child->draw_line(x, y, x + size, y + size);
    }

    // Here we can specialize the return type in the child, because Child is a template here!
    virtual std::unique_ptr<Child> copy() = 0;
};

// CRTP in action - inherit from base and pass itself as the template argument.
class PngDrawer: public Drawer<PngDrawer> {
public:
    // Notice that there is no draw_line function in Drawer!
    // If the child doesn't provide it, the code will simply not compile.
    void draw_line(int x1, int y1, int x2, int y2) {}

    // Here we can use a more specific return type.
    std::unique_ptr<PngDrawer> copy() override {
        return std::make_unique<PngDrawer>();
    }
};
class SvgDrawer: public Drawer<SvgDrawer> {
public:
    void draw_line(int x1, int y1, int x2, int y2) {}

    std::unique_ptr<SvgDrawer> copy() override {
        return std::make_unique<SvgDrawer>();
    }
};


int main() {
    SvgDrawer a;
    a.draw_house(1, 1, 10);

    return 0;
}
