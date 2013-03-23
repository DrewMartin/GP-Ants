#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <qmath.h>

#define QSP QSharedPointer

typedef enum {
    North,
    NE,
    East,
    SE,
    South,
    SW,
    West,
    NW
} Direction;

inline Direction directionAfterLeftTurn(Direction source) {
    switch(source) {
    case North: return NW;
    case East: return NE;
    case South: return SE;
    case West: return SW;
    case NE: return North;
    case SE: return East;
    case SW: return South;
    default: return West;
    }
}

inline Direction directionAfterRightTurn(Direction source) {
    switch(source) {
    case North: return NE;
    case East: return SE;
    case South: return SW;
    case West: return NW;
    case NE: return East;
    case SE: return South;
    case SW: return West;
    default: return North;
    }
}

inline Direction oppositeDirection(Direction source) {
    switch(source) {
    case North: return South;
    case East: return West;
    case South: return North;
    case West: return East;
    case NE: return SW;
    case SE: return NW;
    case SW: return NE;
    default: return SE;
    }
}


#define MAX_FOOD 200
#define MAX_X 100
#define MAX_Y 100
#define SCALE 5

#define FOOD_RAD 4
#define ANTHILL_RAD 7
#define ANT_RAD 0

#define LOC_HELPER(loc,rad) loc.x()-rad, loc.y()-rad, rad*2+1, rad*2+1

inline int sq(int val) {
    return val * val;
}

inline int sqDistance(int x1, int y1, int x2, int y2) {
    int dist = sq(x1 - x2) + sq(y1 - y2);

    return dist;
}

#define CIRCLE_LOOP_HELPER(x,y,rad) \
    int sqRad = sq(rad), sqDist; \
    for(int row = qMax(y - rad, 0); row <= qMin(y + rad, MAX_Y-1); row++) \
        for(int col = qMax(x - rad, 0); col <= qMin(x + rad, MAX_X-1); col++) \
            if (sqRad >= (sqDist = sqDistance(x, y, col, row)))

#endif // DEFAULTS_H
