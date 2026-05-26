#pragma once

#include "entities.hpp"

bool wall_broad(circle& shape, int window_width, int window_height);

bool circle_broad(circle& shape1, circle& shape2);

bool wall_collision(circle& shape, int window_width, int window_height, float slop=0, float percentage=1);

bool circle_collision(circle& shape1, circle& shape2, float slop=0, float percentage=1);

