#pragma once

#include "basic_math.hpp"
#include "entities.hpp"

bool wall_collision(circle& shape, int window_width, int window_height, float slop=0);

void collision_resolve(circle& shape1, circle& shape2, vector2d mtv, vector2d contact);

bool final_collision(circle &shape1, circle &shape2, vector2d &res, vector2d &contact, float slop=0);
