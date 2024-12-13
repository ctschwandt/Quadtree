#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

#include "vec2.h"
#include "GameObject.h"
#include "Circle.h"
#include "QTNode.h"

enum CollisionAlgorithm
{
    BRUTE_FORCE,
    QUADTREE,
};
enum GameState
{
    DEFAULT,
    INIT_DEFAULT_FIELD,
    VECTOR_FIELD,
};

inline
void handle_collisions(int i, int j, std::vector< Circle * > circles, std::vector< bool > & collided, Surface & surface)
{
    Circle * circle0 = circles[i];
    Circle * circle1 = circles[j];
    if (i != j && circle0->collides_with(*circle1))
    {
        // collided[i] = true; // I don't need collided?
        // collided[j] = true;
        // char bleh;
        // std::cout << "COLLISION: "
        //           << *circle0 << std::endl
        //           << *circle1 << std::endl;

        // SHOW COLLISION       
        // surface.lock();

        // surface.put_line(circle0->x() - 20, circle0->y(),
        //                  circle1->x() + 40, circle1->y(),
        //                  PINK);
        
        // surface.unlock();
        // surface.flip();

        // static collisions
        float x_diff = circle0->x() - circle1->x();
        float y_diff = circle0->y() - circle1->y();
        float distance = vec2f(x_diff, y_diff).len();
        float overlap = 0.5f * (distance - circle0->r() - circle1->r());

        if (distance != 0)
        {
            // Normal vector: unit vector from circle0 to circle1
            float nx = x_diff / distance;
            float ny = y_diff / distance;

            // Displace circles to resolve overlap
            float x_displacement = overlap * nx;
            float y_displacement = overlap * ny;

            circle0->move(-x_displacement, -y_displacement);
            circle1->move(x_displacement, y_displacement);

            // Reflection for circle0 (velocity of circle0)
            float vx0 = circle0->dx();
            float vy0 = circle0->dy();

            // Dot product between velocity and normal for circle0
            float dotProduct0 = vx0 * nx + vy0 * ny;

            // Calculate the reflected velocity vector for circle0
            float reflected_vx0 = vx0 - 2 * dotProduct0 * nx;
            float reflected_vy0 = vy0 - 2 * dotProduct0 * ny;

            // Update the velocity for circle0
            circle0->dx() = reflected_vx0;
            circle0->dy() = reflected_vy0;

            // Reflection for circle1 (velocity of circle1)
            float vx1 = circle1->dx();
            float vy1 = circle1->dy();

            // Dot product between velocity and normal for circle1
            float dotProduct1 = vx1 * nx + vy1 * ny;

            // Calculate the reflected velocity vector for circle1
            float reflected_vx1 = vx1 - 2 * dotProduct1 * nx;
            float reflected_vy1 = vy1 - 2 * dotProduct1 * ny;

            // Update the velocity for circle1
            circle1->dx() = reflected_vx1;
            circle1->dy() = reflected_vy1;
            
            // float x_displacement = (overlap * x_diff) / distance;
            // float y_displacement = (overlap * y_diff) / distance;

            // circle0->move(-x_displacement, -y_displacement);
            // circle1->move(x_displacement, y_displacement);
        }
        else
        {
            // std::cout << *circle0 << std::endl;
            // std::cout << *circle1 << std::endl;
            float c0r = circle0->r();
            float c1r = circle1->r();
            circle0->move(-c0r, -c0r);
            circle1->move(c1r, c1r);
        }
 
        // // reverse velocities
        // circle0->vel() = {-circle0->dx(), -circle0->dy()};
        // circle1->vel() = {-circle1->dx(), -circle1->dy()};        
                    
        // change colors
        int random_color_i = rand() % NUM_STAR_TYPES;
        int random_color_j = rand() % NUM_STAR_TYPES;
        circle0->color() = get_color(random_color_i);
        circle1->color() = get_color(random_color_j);
                    
        // Color t = circle0->color();
        // circle0->color() = circle1->color();
        // circle1->color() = t;

        // bounce away
        // while (circle0->collides_with(*circle1))
        // {
        //     circle0->move();
        //     circle1->move();
        // }
        // circle0->move();
        // circle1->move();

        // perhaps some circles are colliding twice, so their
        // velocity goes back to normal

        // if (circle0->collides_with(*circle1))
        // {
        //     std::cout << "still colliding "
        //               << std::endl
        //               << *circle0 << std::endl
        //               << *circle1 << std::endl;
        // }
    }
}

class Game
{
public:
    //================== ...structors ====================//
    Game(Surface & surface)
        : surface_(surface),
          algorithm_(QUADTREE), state_(DEFAULT),
          should_draw_boxes_(true),
          pselected_ball_(NULL), quadtree_(surface)
    {}
    ~Game()
    {
        for (auto * p : circles())
        {
            delete p;
        }
    } 
    //====================================================//
    
    //================ getters and setters ================//
    CollisionAlgorithm & algorithm() { return algorithm_; }
    CollisionAlgorithm algorithm() const { return algorithm_; }
    GameState & state() { return state_; }
    GameState state() const { return state_; }
    Surface & surface() { return surface_; }
    Surface surface() const { return surface_; }
    Circle *& pselected_ball() { return pselected_ball_; }
    Circle * pselected_ball() const { return pselected_ball_; }
    std::vector < Circle * > & circles() { return circles_; }
    std::vector < Circle * > circles() const { return circles_; }
    QuadTree & quadtree() { return quadtree_; }
    QuadTree quadtree() const { return quadtree_; }
    bool & should_draw_boxes() { return should_draw_boxes_; }
    bool should_draw_boxes() const { return should_draw_boxes_; }
    static Music & gameplay_music() { return gameplay_music_; }
    //=====================================================//

    //====================== methods ======================//
    void play_gameplay_music() { gameplay_music().play(); }

    void user_update(Event & event)
    {
        auto is_point_in_circle = [](int x, int y, Circle * c)
        {
            return ((x - c->x()) * (x - c->x()) + (y - c->y()) * (y - c->y()) < c->r() * c->r());
        };

        if (event.type() == MOUSEBUTTONDOWN)
        {
            // std::cout << "MOUSEBUTTONDOWN" << std::endl;
            for (auto * circle : circles())
            {
                if (is_point_in_circle(mouse_x(), mouse_y(), circle))
                {
                    // std::cout << "selected ball " << std::endl;
                    pselected_ball_ = circle;
                }
            }
        }
        else if (event.type() == MOUSEBUTTONUP)
        {
            // std::cout << "MOUSEBUTTONUP" << std::endl;
            pselected_ball_ = NULL;
        } 

        if (pselected_ball_ != NULL)
        {
            // std::cout << "selected " << *pselected_ball_ << std::endl;
            pselected_ball_->x() = mouse_x();
            pselected_ball_->y() = mouse_y();
        }     
    }
    
    void build_circles(int n)
    {
        // Creates n circles each with a
        // random position and colors
        for (int i = 0; i < n; ++i)
        {
            int random_color_index = rand() % NUM_STAR_TYPES;
            int random_x = 2 * RADIUS + rand() % (W - 3 * RADIUS);
            int random_y = 2 * RADIUS + rand() % (H - 3 * RADIUS);
            int random_speed = rand() % SPEED_LIM + SPEED;
            int random_dx = (rand() % 2 == 0 ? random_speed : -random_speed);
            int random_dy = (rand() % 2 == 0 ? random_speed : -random_speed);
            int random_radius = rand() % RADIUS_LIM + RADIUS;

            circles().push_back(new Circle(surface(),
                                           vec2f(random_x, random_y),
                                           vec2f(random_dx, random_dy),
                                           get_color(random_color_index),
                                           MOVING,
                                           random_radius));
        }
    }
    
    void draw_all()
    {
        surface().lock();
        surface().fill(BLACK);

        if (algorithm() == QUADTREE && should_draw_boxes_)
        {
            for (auto * node : quadtree().leaves())
            {
                node->draw();
            }
        }
           
        // circles
        for (auto * circle : circles())
        {
            // std::cout << *circle << std::endl;
            circle->draw();
        }
        
        surface().unlock();
        surface().flip();
    }

    void brute_force_collision()
    {
        std::vector< bool > collided(circles().size());
        for (int i = 0; i < circles().size(); ++i)
        {
            for (int j = 0; j < circles().size(); ++j)
            {
                handle_collisions(i, j, circles(), collided, surface());
            }
        }
    }

    void build_quadtree()
    {
        quadtree().delete_children();

        // insert circles into quadtree
        for (auto * circle : circles())
        {
            quadtree().insert(circle);
        }

        // get leaves to check for
        // collisions within them
        quadtree().get_leaves();
    }

    void quadtree_collision()
    {
        // build quadtree
        build_quadtree();

        // brute force search
        // on circles/bounds of each leaf        
        for (auto * node : quadtree().leaves())
        {
            std::vector< bool > collided(node->circles().size());
            for (int i = 0; i < node->circles().size(); ++i)
            {
                for (int j = 0; j < node->circles().size(); ++j)
                {
                    handle_collisions(i, j, node->circles(), collided, surface());
                }
            }
        }
        
    }

    void update_circles()
    {
        // determine which algorithm
        // to use
        if (algorithm() == BRUTE_FORCE)
        {
            brute_force_collision();
        }
        else if (algorithm() == QUADTREE)
        {
            quadtree_collision();
        }

        // determine whether or not to apply vector field
        if (state() == INIT_DEFAULT_FIELD)
        {
            // set all circles to their default velocity
            for (auto * p : circles())
            {
                p->reinit_velocity();
            }
            state() = DEFAULT;
        }
        else if (state() == VECTOR_FIELD)
        {
            // apply vector field to velocity
            // for all circles
            for (auto * p : circles())
            {
                p->vector_field_velocity();
            }
        }

        // update circles
        for (auto * p : circles())
        {
            p->update();
        }
    }
    //=====================================================//
    
private:
    CollisionAlgorithm algorithm_;
    GameState state_;
    Circle * pselected_ball_;
    Surface & surface_;
    std::vector < Circle * > circles_;
    QuadTree quadtree_;
    bool should_draw_boxes_;

    static Music gameplay_music_;
};

#endif
