#ifndef ANIME 
#define ANIME
#include "objects.cpp"
#include "timer.cpp"
#include <SDL2/SDL_pixels.h>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>
#include <cmath>
double clamp(double x) {
  if (x<0.0) return 0.0;
  if (x>1.0) return 1.0;
  return x;
}
double smootherstep(double x) {
  x = clamp(x);
  return x*x*x*(x*(6.0f*x-15.0f)+10.0f);
}
double springstep(double x, double edge0=0.0, double edge1=1.0, double damping=0.3, double frequency=2.0) {
    x = (x - edge0) / (edge1 - edge0);
    if (x < 0.0) x = 0.0;
    if (x > 1.0) x = 1.0;

    double zeta = damping;
    double omega_n = frequency * M_PI;
    double t = x;

    double omega_d = omega_n * std::sqrt(1.0 - zeta * zeta);
    double phase = std::acos(zeta);

    double value = 1.0 - (1.0 / std::sqrt(1.0 - zeta * zeta)) *
        std::exp(-zeta * omega_n * t) *
        std::sin(omega_d * t + phase);

    return value;
}
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
    double time,start=0.0;
    std::function<double(double)> step;
    double dx,dy;
public:
    Translate(Object* obj,double x,double y,double time,std::function<double(double)> func): time(time),Animation(obj),step(func),dx(x),dy(y) {}
    void update() {
        double st=step(start/time);
        start+=Timer::checkFrameTime();
        st=step(start/time)-st;
        if (start>=time) {
            done=true;
            return;
        }
        obj->move(dx*st,dy*st);
    }
};
class ColorChange: public Animation {
    double time,start=0.0;
    std::function<double(double)> step;
    SDL_Color initial;
    double dr,dg,db;
public:
    ColorChange(Object* obj,SDL_Color final,double time,std::function<double(double)> func): time(time),Animation(obj),step(func) {
        initial=obj->getColor();
        dr=final.r-initial.r;
        dg=final.g-initial.g;
        db=final.b-initial.b;
    }
    void update() {
        start+=Timer::checkFrameTime();
        if (start>=time) {
            done=true;
            return;
        }
        double st=step(start/time);
        SDL_Color set=initial;
        set.r+=dr*st;
        set.g+=dg*st;
        set.b+=db*st;
        obj->changeColor(set);
    }
};
class ChangeValue: public Animation {
    double time,start=0.0;
    std::function<double(double)> step;
    double dx;
    double& value;
public:
    ChangeValue(double& value,double x,double time,std::function<double(double)> func): time(time),step(func),value(value),dx(x-value),Animation(nullptr) {}
    void update() {
        double st=step(start/time);
        start+=Timer::checkFrameTime();
        st=step(start/time)-st;
        if (start>=time) {
            done=true;
            return;
        }
        value+=dx*st;
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
