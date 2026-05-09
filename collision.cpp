#include "basic_math.hpp"
#include "entities.hpp"
#include "collision.hpp"

bool wall_collision(circle& shape, int window_width, int window_height) {
    vector2d mtv;
    
    if (shape.position.x - shape.radius < 0)
        mtv.x = shape.radius - shape.position.x;
    else if(shape.position.x + shape.radius > window_width)
        mtv.x = (window_width - shape.position.x) - shape.radius;
    
    if (shape.position.y - shape.radius < 0)
        mtv.y = shape.radius - shape.position.y;
    else if(shape.position.y + shape.radius > window_height)
        mtv.y = (window_height - shape.position.y) - shape.radius;

    shape.position.add(mtv);

    return (mtv.x != 0 || mtv.y != 0);
}

bool circle_collision(circle &shape1, circle &shape2) {
    ;
}
