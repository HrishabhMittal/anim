#include "animations.cpp"
#include "drawing.cpp"
#include "textures.cpp"
int main() {
    Window::create("something",1000,800);

    ObjectGroup objg;
        Rectangle* rect=new Rectangle({100,100,100,100},{0,100,200,255});
        Rectangle* rect2=new Rectangle({100,400,200,100},{200,100,200,255});
    objg.push(rect);
    objg.push(rect2);
    

    AnimationSequence as;
    
    AnimationGroup* ag=new AnimationGroup();
        ag->push(new Translate(rect,100,100,1,smootherstep));
        ag->push(new ColorChange(rect,{0,200,100},1,smootherstep));
        ag->push(new Translate(rect2,0,100,1,smootherstep));
        ag->push(new ColorChange(rect2,{200,200,100},1,smootherstep));
    as.push(ag);
    
    Timer::startTimer();
    while (Window::running) {
        Timer::updateFrameTime();
        Window::pollEvents();
        Window::clear({0,0,0,255});
        as.update();
        objg.render();
        Window::present();
        setFrameRate(100);
    }
}
