#include <iostream>
#include <cmath>
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
#include "Game.h"

int main()
{
    srand((unsigned int) time(NULL));
    
    //===========================
    // create objects
    //===========================
    Surface surface(W, H);
    Event event;
    KeyPressed keypressed = get_keypressed();
    Game game(surface);
    game.build_circles(NUM_CIRCLES);
    game.play_gameplay_music();

    int key_delay = 0;
    int key_delay_max = 10;
    int frame_count = 0; 
    int fps_timer_start = getTicks(); 
    float fps = 0.0f;
    const int FPS_UPDATE_INTERVAL = 5000; // Update FPS every 5000 ms
    while (1)
    {
        int start = getTicks();

        //==================================
        // keyboard inputs
        //==================================
        if (event.poll() && event.type() == QUIT
            || keypressed[q] || keypressed[ESC])
        {
            break;
        }
        else if (keypressed[TAB]) // tab to pause
        {
            continue;
        }
        
        if (key_delay > 0)
            --key_delay;
 
        if (key_delay == 0)
        {
            if (keypressed[SPACE])
            {
                game.algorithm() = (game.algorithm() == BRUTE_FORCE
                                    ? QUADTREE : BRUTE_FORCE);
                key_delay = key_delay_max;
            }  
            if (keypressed[Z])
            {
                //std::cout << "vector field" << std::endl;
                game.state() = VECTOR_FIELD;
                key_delay = key_delay_max;
            }
            else if (keypressed[X])
            {
                game.state() = INIT_DEFAULT_FIELD;
                key_delay = key_delay_max;
            }

            if (keypressed[C])
            {
                game.should_draw_boxes() = true;
                key_delay = key_delay_max;
            }
            else if (keypressed[V])
            {
                game.should_draw_boxes() = false;
                key_delay = key_delay_max;
            }

            if (keypressed[UPARROW] && game.pselected_ball() != NULL)
            {
                // std::cout << "PLUS" << std::endl;
                if (game.pselected_ball()->r() < W / 2)
                    game.pselected_ball()->r() += 1;
            }
            else if (keypressed[DOWNARROW] && game.pselected_ball() != NULL)
            {
                if (game.pselected_ball()->r() > 1)
                    game.pselected_ball()->r() -= 1;
            }
            
            game.user_update(event);
        }

        //==================================
        // mouse inputs
        //==================================
        
        game.update_circles();
        game.draw_all();
        
        int end = getTicks();

        int frame_time = end - start;
        frame_count++;

        // update FPS
        if (end - fps_timer_start >= FPS_UPDATE_INTERVAL) 
        {
            fps = frame_count / ((end - fps_timer_start) / 1000.0f);
            std::cout << "FPS: " << fps << std::endl;

            // reset FPS calculation
            frame_count = 0;
            fps_timer_start = end;
        }

        int dt = RATE - end + start;
        //std::cout << dt << std::endl;
        if (dt > 0) delay(dt);
    }

    return 0;
}
