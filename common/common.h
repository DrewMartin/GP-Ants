#ifndef COMMON_H
#define COMMON_H

#include <QSharedPointer>

#define QSP QSharedPointer

#define MIN_X -5.0
#define MAX_X 5.0
#define STEP_SIZE 0.01

#define MAX_CONSTANT 5

#define XOVER_PROB 90

#define POP_SIZE 1000
#define MAX_HEIGHT 11
#define TOURNAMENT_SIZE 8
#define PRECISION 0.001

#define RAND_INT(max) (qrand() % (max))

#endif // COMMON_H
