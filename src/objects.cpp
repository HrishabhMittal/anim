#ifndef OBJECT
#define OBJECT
#include "drawing.cpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <iostream>
#include <ostream>
#include <vector>
class Object {
public:
    SDL_Color color;
    Object(SDL_Color c): color(c) {}
    virtual void move(double dx,double dy)=0;
    void changeColor(SDL_Color c) {
        color=c;
    }
    SDL_Color getColor() {
        return color;
    }
    virtual void render()=0;
    virtual ~Object() {};
};
class ObjectGroup: public Object {
    std::vector<Object*> obj;
public:
    ObjectGroup(): Object({0,0,0,0}) {}
    void move(double dx,double dy) override {
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
    std::vector<double> px;
    std::vector<double> py;
public:
    void move(double dx,double dy) override {
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
        std::vector<Sint16> spx(px.size(),0);
        std::vector<Sint16> spy(py.size(),0);
        for (int i=0;i<spx.size();i++) spx[i]=px[i];
        for (int i=0;i<spy.size();i++) spy[i]=py[i];
        Window::fillPoly(spx,spy);
    }
};
class Rectangle: public Object {
    double x,y,w,h;
public:
    Rectangle(SDL_Rect r,SDL_Color c): x(r.x),y(r.y),w(r.w),h(r.h),Object(c) {}
    void move(double dx,double dy) override {
        x+=dx;
        y+=dy;
    }
    void render() override {
        Window::setColor(color);
        Window::fillRect(x,y,w,h);
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
