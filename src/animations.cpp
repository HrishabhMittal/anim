#ifndef ANIME 
#define ANIME
#include "objects.cpp"
#include <iostream>
#include <ostream>
#include <vector>
class Animation {
public:
    bool done=false;
    Object* obj;
    Animation(Object* obj): obj(obj) {}
    bool isdone() {
        return done;
    }
    virtual void update()=0;
    virtual ~Animation() {};
};
class Translate: public Animation {
    int dx,dy;
    int frames;
public:
    Translate(Object* obj,int dx,int dy,int frames): frames(frames),dx(dx),dy(dy),Animation(obj) {}
    void update() {
        if (frames==0) {
            done=true;
            return;
        }
        frames--;
        obj->move(dx,dy);
    }
};
class AnimationGroup {
public:
    std::vector<Animation*> anims;
    bool done=false;
    AnimationGroup() {}
    void push(Animation* an) {
        anims.push_back(an);
    }
    void update() {
        if (done) return;
        int x=0;
        for (auto i:anims) {
            i->update();
            x+=i->isdone();
        }
        if (x==anims.size()) done=true;
    }
    ~AnimationGroup() {
        for (auto i:anims) {
            delete i;
        }
    }
};
class AnimationSequence {
public:
    bool done=false;
    std::vector<AnimationGroup*> ags;
    void push(AnimationGroup* ag) {
        ags.push_back(ag);
    }
    void update() {
        for (auto i:ags) {
            if (!i->done) {
                i->update();
                return;
            }
        }
        done=true;
    }
    ~AnimationSequence() {
        for (auto i:ags) {
            delete i;
        }
    }
};
#endif
