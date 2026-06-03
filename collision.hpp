#pragma once

#include "entities.hpp"

bool wall_collision(circle& shape, int window_width, int window_height, float slop=0);

bool circle_collision(circle& shape1, circle& shape2, float slop=0);

void collision_resolve(circle& shape1, circle& shape2);

