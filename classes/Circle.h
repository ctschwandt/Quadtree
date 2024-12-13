#ifndef CIRCLE_H
#define CIRCLE_H

#include "GameObject.h"
#include <cmath>

enum CIRCLE_STATE
{
    MOVING,
    BOUNCING_OFF_LEFT,
    BOUNCING_OFF_RIGHT,
    BOUNCING_OFF_TOP,
    BOUNCING_OFF_BOTTOM,
    BOUNCING_OFF_BALL
};

//==============================================================
// Circle Class
//==============================================================
class Circle : public GameObject
{
public:
    //===================== structors =====================//
    Circle(Surface & surface,
           const vec2f & position, const vec2f & velocity,
           const Color & color, CIRCLE_STATE state, float radius)
        : GameObject(surface, position, velocity, color),
          state_(state), r_(radius), init_vel_(velocity)
    {}
    Circle(Surface & surface)
        : GameObject(surface), r_(0)
    {}
    Circle(const Circle & other)
        : GameObject(other), r_(other.r())
    {}
    ~Circle()
    {}
    //====================================================//
    
    //================ getters and setters ================//
    CIRCLE_STATE & state() { return state_; }
    CIRCLE_STATE state() const { return state_; }
    vec2f & init_vel() { return init_vel_; }
    vec2f init_vel() const { return init_vel_; }
    float & r() { return r_; }
    float r() const { return r_; }
    //=====================================================//
    
    //====================== methods =====================//
    void draw() { surface().put_circle(int(x()), int(y()), int(r()), color()); }
    
    bool hit_left()
    {
        return (x() < r());
    }

    bool hit_right()
    {
        return (x() + r() > W - 1);
    }

    bool hit_top()
    {
        return (y() < r());
    }

    bool hit_bottom()
    {
        return (y() + r() > H - 1);
    }

    bool collides_with_wall() // collides with left or right boundary
    {
        return (hit_left() || hit_right());
    }

    bool collides_with_ceiling() // collides with top or bottom boundary
    {
        return (hit_top() || hit_bottom());
    }
    
    void reinit_velocity()
    {
        vel() = init_vel();
    }

    void vector_field_velocity()
    {
        vec2f new_vel = {-(y() - H / 2) * 1.5f, (x() - W / 2) * 0.8f};
        vel() = norm(new_vel);
        dx() *= SPEED;
        dy() *= SPEED;
    }

    void bounce_off_left()
    {
        dx() = -dx();
        x() = r();
    }

    void bounce_off_right()
    {
        dx() = -dx();
        x() = W - 1 - r();
    }

    void bounce_off_top()
    {
        dy() = -dy();
        y() = r();
    }

    void bounce_off_bottom()
    {
        dy() = -dy();
        y() = H - 1 - r();
    }
    
    bool collides_with(const Circle & other)
    {
        float dx = x() - other.x();
        float dy = y() - other.y();
        float dr = r() + other.r();

        return dx * dx + dy * dy <= dr * dr;
    }
    
    void update()
    {
        // switch(state())
        // {
        //     case MOVING:
        //         moving();
        //         break;
        //     case BOUNCING_OFF_LEFT:
        //         bounce_off_left();
        //         break;
        //     case BOUNCING_OFF_RIGHT:
        //         bounce_off_right();
        //         break;
        //     case BOUNCING_OFF_TOP:
        //         bounce_off_top();
        //         break;
        //     case BOUNCING_OFF_BOTTOM:
        //         bounce_off_bottom();
        //         break;
        //     // case BOUNCING_OFF_BALL:
        //     //     bounce_off_ball();
        //     //     break;
        //     default:
        //         break;
        // }

        if (hit_left())
            bounce_off_left();
        if (hit_right())
            bounce_off_right();
        if (hit_top())
            bounce_off_top();
        if (hit_bottom())
            bounce_off_bottom();

        move();
        
        // where to place these change state_checks?
        // in update_state() function?
        // if (hit_left())
        //     state() = BOUNCING_OFF_LEFT;
        // else if (hit_right())
        //     state() = BOUNCING_OFF_RIGHT;
        //  else if (hit_top())
        //     state() = BOUNCING_OFF_TOP;
        //  else if (hit_bottom())
        //     state() = BOUNCING_OFF_BOTTOM;
        //  else
        //      state() = MOVING;
    }
    //=====================================================//
    
private:
    CIRCLE_STATE state_;
    vec2f init_vel_;
    float r_;
};

//==============================================================
// Print
//==============================================================
inline
std::ostream & operator<<(std::ostream & cout, const Circle & circle)
{
    cout << "<Circle: " << &circle
         << " pos: " << circle.pos()
         << " vel: " << circle.vel()
         << '>';
 
    return cout;
}

#endif
