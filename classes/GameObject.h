#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

#include "vec2.h"

// I should use my star colors from Galaxia.
// when ever stars hit each other, they turn into
// a random one of the star colors (but you have to
// make sure they don't choose the same color)

// balls with different sizes and speeds?
// acceleration?

enum STAR_TYPE
{
    O5_V,
    B1_V,
    B3_V,
    B5_V,
    B8_V,
    A1_V,
    A3_V,
    A5_V,
    F0_V,
    F2_V,
    F5_V,
    F8_V,
    G2_V,
    G5_V,
    G8_V,
    K0_V,
    K4_V,
    K7_V,
    M2_V,
    M4_V,
    M6_V,
    NUM_STAR_TYPES
};

inline
Color get_color(int i)
{
    static const Color star_colors[] =
        {
            {157, 180, 255}, // O5(V)
            {162, 185, 255}, // B1(V)
            {167, 188, 255}, // B3(V)
            {170, 191, 255}, // B5(V)
            {175, 195, 255}, // B8(V)
            {186, 204, 255}, // A1(V)
            {192, 209, 255}, // A3(V)
            {202, 216, 255}, // A5(V)
            {228, 232, 255}, // F0(V)
            {237, 238, 255}, // F2(V)
            {251, 248, 255}, // F5(V)
            {255, 249, 249}, // F8(V)
            {255, 245, 236}, // G2(V)
            {255, 244, 232}, // G5(V)
            {255, 241, 223}, // G8(V)
            {255, 235, 209}, // K0(V)
            {255, 215, 174}, // K4(V)
            {255, 198, 144}, // K7(V)
            {255, 190, 127}, // M2(V)
            {255, 187, 123}, // M4(V)
            {255, 187, 123}  // M6(V)
        };

    return star_colors[i];
}


//==============================================================
// GameObject
//==============================================================
class GameObject
{
public:
    //============ ...structors ============//
    GameObject(Surface & surface,
               const vec2f & position, const vec2f & velocity,
               const Color & color)
        : surface_(surface),
          pos_(position), vel_(velocity),
          color_(color)
    {}
    GameObject(Surface & surface)
        : surface_(surface), pos_(vec2f(-100, -100)),
          vel_(vec2f(0, 0))
    {}
    virtual ~GameObject() {}
    //======================================//
               
    //================ getters and setters ================//
    Surface & surface() { return surface_; }
    Surface surface() const { return surface_; }
    vec2f & pos() { return pos_; }
    vec2f pos() const { return pos_; }
    vec2f & vel() { return vel_; }
    vec2f vel() const { return vel_; }
    Color & color() { return color_; }
    Color color() const { return color_; }

    float & x() { return pos_.get_x(); }
    float x() const { return pos_.get_x(); }
    float & y() { return pos_.get_y(); }
    float y() const { return pos_.get_y(); }
    float & dx() { return vel_.get_x(); }
    float dx() const { return vel_.get_x(); }
    float & dy() { return vel_.get_y(); }
    float dy() const { return vel_.get_y(); }
    //=====================================================//

    //====================== methods ======================//
    void move();
    void move(float d_x, float d_y = 0);
    void move_to(int, int);
    virtual void draw() = 0;
    virtual void update() = 0;
    //=====================================================//

    //===================== operators =====================//
    GameObject & operator=(const GameObject &);
    //=====================================================//
    
protected:
    Surface & surface_;
    vec2f pos_;
    vec2f vel_;
    Color color_;
};

inline
void GameObject::move()
{
    x() += dx();
    y() += dy();
}

inline
void GameObject::move(float d_x, float d_y)
{
    x() += d_x;
    y() += d_y;
}

inline
void GameObject::move_to(int x_, int y_)
{
    x() = x_;
    y() = y_;
}

#endif
