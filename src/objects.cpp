#ifndef OBJECT
#define OBJECT
#include "drawing.cpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <vector>
class Object {
public:
    SDL_Color color;
    Object(SDL_Color c): color(c) {}
    virtual void move(int dx,int dy)=0;
    void changeColor(SDL_Color c) {
        color=c;
    }
    virtual void render()=0;
    virtual ~Object() {};
};
class ObjectGroup: public Object {
    std::vector<Object*> obj;
public:
    ObjectGroup(): Object({0,0,0,0}) {}
    void move(int dx,int dy) override {
        for (auto i:obj) {
            i->move(dx,dy);
        }
    }
    void push(Object* ob) {
        obj.push_back(ob);
    }
    void render() override {
        for (auto i:obj) {
            i->render();
        }
    }
    ~ObjectGroup() {
        for (auto& i:obj) {
            delete i;
        }
    }
};
class Polygon: public Object {
    std::vector<Sint16> px;
    std::vector<Sint16> py;
public:
    void move(int dx,int dy) override {
        for (auto& i:px) i+=dx;
        for (auto& i:py) i+=dy;
    }
    Polygon(std::vector<SDL_Point> points,SDL_Color color): Object(color) {
        for (auto& i:points) {
            px.push_back(i.x);
            py.push_back(i.y);
        }
    }
    void render() override {
        Window::setColor(color);
        Window::fillPoly(px,py);
    }
};
class Rectangle: public Object {
    SDL_Rect r;
public:
    Rectangle(SDL_Rect r,SDL_Color c): r(r),Object(c) {}
    void move(int dx,int dy) override {
        r.x+=dx;
        r.x+=dy;
    }
    void render() override {
        Window::setColor(color);
        Window::fillRect(r);
    }
};
class RoundedRect: public Object {

};
class Line: public Object {

};
class Image: public Object {

};
class Text: public Object {

};
#endif
