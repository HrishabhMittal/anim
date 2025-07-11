
#include "../src/animations.cpp"
#include "../src/drawing.cpp"
#include "../src/textures.cpp"
int main() {
    Window::create("something",1000,800);

    ObjectGroup objg;
        Rectangle* rect=new Rectangle({100,100,100,100},{0,100,200,255});
        objg.push(rect);
        Rectangle* rect2=new Rectangle({100,400,200,100},{200,100,200,255});
        objg.push(rect2);
    

    AnimationSequence as;
    
    AnimationGroup* ag=new AnimationGroup();
        as.push(ag);
        ag->push(new Translate(rect,100,100,1,smootherstep));
        ag->push(new ColorChange(rect,{0,200,100},1,smootherstep));
        ag->push(new Translate(rect2,0,100,1,smootherstep));
        ag->push(new ColorChange(rect2,{200,200,100},1,smootherstep));
    Timer::startTimer();
    while (Window::running) {
        Timer::updateFrameTime();
        Window::pollEvents();
        Window::clear({255,255,255,255});
        as.update();
        objg.render();
        Window::present();
        setFrameRate(100);
    }
}
