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
        ObjectGroup* cat=new ObjectGroup();
            Polygon* body=new Polygon({{100,800},{120,400},{140,500},{240,500},{260,400},{280,800}},{0,0,0,255});
            cat->push(body);
            
            ObjectGroup* glasses=new ObjectGroup();
                glasses->push(new Circle({150,570},30,{255,255,255,255}));
                glasses->push(new Circle({230,570},30,{255,255,255,255}));
                glasses->push(new Rectangle({150,567,80,5},{255,255,255,255}));
                glasses->push(new Circle({150,570},25,{0,0,0,255}));
                glasses->push(new Circle({230,570},25,{0,0,0,255}));
            cat->push(glasses);
            
            Polygon* nose=new Polygon({{185,580},{190,590},{195,580}},{255,255,255,255});
            cat->push(nose);

        objg.push(cat);
    

    AnimationSequence as;
    
    AnimationGroup* ag=new AnimationGroup();
        as.push(ag);
        ag->push(new Translate(rect,100,100,1,smootherstep));
        ag->push(new ColorChange(rect,{0,200,100},1,smootherstep));
        ag->push(new Translate(rect2,0,100,1,smootherstep));
        ag->push(new ColorChange(rect2,{200,200,100},1,smootherstep));
        ag->push(new Translate(cat,500,0,2,smootherstep));
        AnimationSequence* fullanim=new AnimationSequence();
            ag->push(fullanim);
            for (int i=0;i<5;i++) {
                AnimationGroup* agg=new AnimationGroup();
                    fullanim->push(agg);
                    AnimationSequence* glassesanim=new AnimationSequence();
                        agg->push(glassesanim);
                        glassesanim->push(new Translate(glasses,0,10,0.2,smootherstep));
                        glassesanim->push(new Translate(glasses,0,-10,0.2,smootherstep));
                    AnimationSequence* noseanim=new AnimationSequence();
                        agg->push(noseanim);
                        noseanim->push(new Translate(nose,0,5,0.2,smootherstep));
                        noseanim->push(new Translate(nose,0,-5,0.2,smootherstep));
            }
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
