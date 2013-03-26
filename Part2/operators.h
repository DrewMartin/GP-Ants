#ifndef OPERATORS_H
#define OPERATORS_H

#include <qmath.h>
#include <QString>

inline double mult(double v1, double v2)
{
    return v1 * v2;
}

inline double add(double v1, double v2)
{
    return v1 + v2;
}

inline double sub(double v1, double v2)
{
    return v1 - v2;
}

inline double divide(double v1, double v2)
{
    if (v2 == 0) {
        if (v1 == 0)
            return 0;
        if (v1 < 0)
            return INT_MIN + 1;
        return INT_MAX - 1;
    }
    return v1 / v2;
}

inline double power(double v1, double v2)
{
    return qPow(v1, v2);
}


typedef double (*SingleArgOp)(double arg);
typedef double (*TwoArgOp)(double arg1, double arg2);

inline QString getSymbolForSingleArgOp(SingleArgOp op) {
    if (op == qSin)
        return QString("sin(%1)");
    if (op == qCos)
        return QString("cos(%1)");

    return "UNKNOWN";
}

inline QString getSymbolForTwoArgOp(TwoArgOp op) {
    if (op == mult)
        return QString("(%1 * %2)");
    if (op == add)
        return QString("(%1 + %2)");
    if (op == sub)
        return QString("(%1 - %2)");
    if (op == divide)
        return QString("(%1 / %2)");
    if (op == power)
        return QString("(%1^%2)");

    return "UNKNOWN";
}

#endif // OPERATORS_H
