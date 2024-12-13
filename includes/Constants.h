#ifndef CONSTANTS_H
#define CONSTANTS_H

// Game
const int FRAME_RATE = 60;
const int RATE = 1000 / FRAME_RATE;
const int NUM_CIRCLES = 1000;

// Whole surface
const int FACTOR = 2;
const int W = 640 * FACTOR;
const int H = 350 * FACTOR;

// Circles
const int SPEED = 1;
const int SPEED_LIM = 2;
const int RADIUS = 3; // 1;
const int RADIUS_LIM = 4;

// Quadtree
const int THRESHOLD = 8;
const int MAX_DEPTH = 5;

#endif
