# Quadtree Collision Simulator (SDL2)

A real-time particle simulation that compares **Brute Force** vs **Quadtree** collision detection.

## Overview

At startup, the simulator spawns a large set of circles with randomized positions and velocities.  
Each frame updates the system by:

- Advancing particle motion
- Resolving circle–circle **elastic collisions**
- Rendering:
  - Current collision mode (**Brute Force** or **Quadtree**)
  - FPS counter (for performance comparison)
  - Quadtree partitions (subdividing rectangles in Quadtree mode)
  - Collision coloring (circles toggle color on impact)

### Collision Modes

- **Brute Force**:  
  Performs ~$\tfrac{n(n-1)}{2}$ pairwise checks per frame.  
  Becomes very costly with 1,000+ particles.

- **Quadtree**:  
  Partitions space into localized regions, reducing collision checks to **nearby neighbors only**.  
  At large $n$, this yields a **dramatic FPS boost** compared to brute force.

### Demo
📺 [Watch the demo on YouTube](https://www.youtube.com/watch?v=O1PLDiVBXmA)

---

---
