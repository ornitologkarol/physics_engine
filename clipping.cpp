#include <cmath>
#include "basic_math.hpp"
#include "entities.hpp"

typedef struct {
    vector2d max;
    vector2d second_point;
} edge;

edge best(circle &shape, vector2d n) {
    // this function finds the edge that takes part in the collision

    // first we look for the furthest point in the other shape
    vector2d points[shape.sides];
    shape.get_points_edges(points, NULL);
    float maxi = dot(points[0], n);
    int index = 0;
    for (int i = 1; i < shape.sides; i++) {
        float projection = dot(points[i], n);
        if (projection > maxi) {
            maxi = projection;
            index = i;
        }
    }

    // now we check which of the two edges containing furthest point is more perpendicular to normal
    vector2d v = points[index];
    vector2d v0 = points[(index-1 >= 0 ? index-1 : shape.sides-1)];
    vector2d v1 = points[(index+1 < shape.sides ? index+1 : 0)];
    vector2d left = vector2d(v.x-v0.x, v.y - v0.y);
    vector2d right = vector2d(v.x-v1.x, v.y-v1.y);
    left.normalize();
    right.normalize();
    // the edge that has the smaller projection is more perpendicular
    if (dot(left, n) <= dot(right, n)) {
        return edge{v, v0};
    } else {
        return edge{v, v1};
    }
}

vector2d contact_point(circle &shape1, circle &shape2, vector2d mtv) {
    //Sutherland–Hodgman algorithm from dyn4j website
    mtv.normalize();
    vector2d n = mtv;

    // mtv always points from B to A
    edge e1 = best(shape1, vector2d(-n.x, -n.y));
    edge e2 = best(shape2, n);
    vector2d e1v = vector2d(e1.max.x - e1.second_point.x, e1.max.y - e1.second_point.y);
    vector2d e2v = vector2d(e2.max.x - e2.second_point.x, e2.max.y - e2.second_point.y);

    //now we look for reference and incident edge
    edge ref, inc;
    bool flip = false;
    vector2d normal;
    if (fabs(dot(e1v, n)) <= fabs(dot(e2v, n))) {
        ref = e1;
        inc = e2;
        e1v.normalize();
        normal = e1v;
    } else {
        ref = e2;
        inc = e1;
        flip = true;
        e2v.normalize();
        normal = e2v;
    }

}
