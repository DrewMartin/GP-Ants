#ifndef COMMON_H
#define COMMON_H

#include <QSharedPointer>

#define QSP QSharedPointer

#define MIN_GRAPH_X -5.0
#define MAX_GRAPH_X 5.0
#define STEP_SIZE 0.01

#define MAX_CONSTANT 5

#define XOVER_PROB 90

#define POP_SIZE 1000
#define ANT_POP_STEP 5
#define MAX_HEIGHT 11
#define ANT_POP_SIZE (ANT_POP_STEP*(MAX_HEIGHT-1)*2)
#define TOURNAMENT_SIZE 8
#define PRECISION 0.001

#define RAND_INT(max) (qrand() % (max))

#endif // COMMON_H
