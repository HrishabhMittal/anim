#include "animations.cpp"
#include "drawing.cpp"
#include "textures.cpp"
#include <iostream>
#include <ostream>

int main() {
    Window::create("something",1000,800);
    ObjectGroup objg;
    Rectangle* rect=new Rectangle({100,100,100,100},{0,100,200,255});
    AnimationSequence as;
    AnimationGroup* ag=new AnimationGroup();
    as.push(ag);
    ag->push(new Translate(rect,1,1,200));
    objg.push(rect);
    while (Window::running) {
        Window::pollEvents();
        Window::clear({0,0,0,255});


        as.update();
        objg.render();
        Window::present();
        setFrameRate(60);
    }
}
