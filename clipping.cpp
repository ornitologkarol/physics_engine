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

void clip(vector2d v1, vector2d v2, vector2d normal, float o, vector2d* cp, int &l) {
    l = 0;
    float d1 = dot(normal, v1) - o;
    float d2 = dot(normal, v2) - o;
    if (d1 >= 0) {
        cp[l++] = v1;
    }
    if (d2 >= 0) {
        cp[l++] = v2;
    }

    if (d1 * d2 < 0.0) {
        vector2d e = v2;
        e.subtract(v1);
        float a = d1 / (d1 - d2);
        e.multiply(a);
        e.add(v1);
        cp[l++] = e;
    }
}

vector2d contact_point(circle &shape1, circle &shape2, vector2d mtv) {
    //Sutherland–Hodgman algorithm from dyn4j website
    mtv.normalize();
    vector2d n = mtv;

    // mtv always points from B to A
    edge e1 = best(shape1, vector2d(-n.x, -n.y));
    edge e2 = best(shape2, n);
    vector2d e1v = vector2d(e1.second_point.x - e1.max.x, e1.second_point.y - e1.max.y);
    vector2d e2v = vector2d(e2.second_point.x - e2.max.x, e2.second_point.y - e2.max.y);

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
    //first clipping
    vector2d cp[2];
    int l;
    float o = dot(normal, ref.max);
    clip(inc.max, inc.second_point, normal, o, cp, l);
    if (l < 2) return vector2d();

    //second clipping
    o = dot(normal, ref.second_point);
    clip(cp[0], cp[1], vector2d(-normal.x, -normal.y), -o, cp, l);
    if (l < 2) return vector2d();

    //third clipping
    vector2d normalperp = normal.perpendicular();
    if (flip) normalperp.multiply(-1);
    o = dot(normalperp, ref.max);
    // normalperp points outwards so we clip those with positive value
    if (dot(normalperp, cp[0]) - o > 0.0) {
        cp[0] = cp[1];
        l--;
    }
    if (dot(normalperp, cp[1]) - o > 0.0) {
        l--;
    }

    //returning contact point
    if (l == 1) return cp[0];
    if (l == 2) return vector2d((cp[0].x + cp[1].x)/2, (cp[0].y + cp[1].y)/2);
    return vector2d();
}
