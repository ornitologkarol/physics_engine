#pragma once

#include "basic_math.hpp"
#include "entities.hpp"

bool wall_broad(circle& shape, int window_width, int window_height);

bool circle_broad(circle& shape1, circle& shape2);

bool wall_collision(circle& shape, int window_width, int window_height, vector2d &mtv);

bool circle_collision(circle& shape1, circle& shape2, vector2d &mtv);

