#ifndef NODE_H
#define NODE_H

#include <QList>
#include <QSharedPointer>
#include "common.h"
#include <QPair>
#include <QDebug>

#define CandidatePair QPair<QSP<Node<Var, Res> >, int >

template<typename Var, typename Res>
class Node
{
public:
    Node() {}
    virtual ~Node() {}

    virtual Res eval(Var var) = 0;
    virtual QString toString() = 0;

    int getDepth() { return depth; }
    int getHeight() { return height; }
    int getSize() { return size; }

    int getChildCount() { return children.length(); }
    QSP<Node<Var, Res> > getChild(int i) {
        if (i < 0 || i >= getChildCount())
            return QSP<Node<Var, Res> >();
        return children.at(i);
    }

    void updateStats() {
        updateStats(0);
    }

    void setChild(int i, QSP<Node<Var, Res> > node) {
        if (i < 0 || i >= getChildCount())
            return;
        children[i] = node;
    }

    static QSP<Node<Var, Res> > mutate(QSP<Node<Var, Res> > node)
    {
        int i = RAND_INT(node->getSize());
        if (node->getSize() <= 1 || i == 0) {
            QSP<Node<Var, Res> > tree = node->generateSubtree(RAND_INT(MAX_HEIGHT));
            tree->updateStats();
            return tree;
        }

        QSP<Node<Var, Res> > cloned = clone(node);
        CandidatePair candidate;
        getCandidate(candidate, cloned, cloned, 0, i);
        int maxHeight = MAX_HEIGHT - candidate.first->getDepth();
        if (maxHeight <= 0)
            return QSP<Node<Var, Res> >();
        maxHeight = RAND_INT(maxHeight);
        candidate.first->children[candidate.second] = node->generateSubtree(maxHeight);
        cloned->updateStats();
        return cloned;
    }

    static QSP<Node<Var, Res> > crossover(QSP<Node<Var, Res> > n1, QSP<Node<Var, Res> > n2)
    {
        QSP<Node<Var, Res> > cloned = clone(n1);
        CandidatePair c1, c2;

        int rand1, rand2, orig1, orig2, attempts = 0;

        while(true) {
            if (++attempts > 50)
                return QSP<Node<Var, Res> >();
            orig1 = rand1 = RAND_INT(cloned->getSize());
            orig2 = rand2 = RAND_INT(n2->getSize());
            if (rand1 == 0 && rand2 == 0)
                continue;

            if (rand1 == 0)
                c1 = CandidatePair();
            else
                getCandidate(c1, cloned, cloned, 0, rand1);

            if (rand2 == 0)
                c2 = CandidatePair();
            else
                getCandidate(c2, n2, n2, 0, rand2);

            if (c1.first.isNull())
                // The new parent is the root of the first. The subtree from n2 will replace n1.
                // No height check required since all nodes in n2 are assumed to be <= MAX_HEIGHT
                break;
            else if (c2.first.isNull() && c1.first->getDepth() + n2->getHeight() > MAX_HEIGHT)
                // c2 is the root of n2, so got to make sure that it's not too high added to c1
                continue;
            else if (c2.first.isNull() || c1.first->getDepth() + c2.first->children.at(c2.second)->getHeight() <= MAX_HEIGHT)
                // make sure that either c2 is null (height checked already)
                // or c1 + c2 is under the max height
                break;
        }

        if (c1.first.isNull()) {
            cloned = clone(c2.first->children.at(c2.second));
        } else {
            if (c2.first.isNull())
                c1.first->children[c1.second] = clone(n2);
            else
                c1.first->children[c1.second] = clone(c2.first->children.at(c2.second));
        }

        cloned->updateStats();
        return cloned;
    }


protected:
    QList<QSharedPointer<Node<Var, Res> > > children;

    virtual Node<Var, Res> *copy() = 0;
    virtual QSP<Node<Var, Res> > generateSubtree(int maxHeight) = 0;

private:
    void updateStats(int depth)
    {
        this->depth = depth;
        int maxHeight = -1;
        size = 1;
        for (int i = 0; i < children.length(); i++) {
            children.at(i)->updateStats(depth + 1);
            maxHeight = qMax(maxHeight, children.at(i)->getHeight());
            size += children.at(i)->getSize();
        }
        height = maxHeight + 1;
    }

    static void getCandidate(CandidatePair &pair, QSP<Node<Var, Res> > &parent, QSP<Node<Var, Res> > &me, int childNum, int &i)
    {
        if (i-- <= 0) {
            pair = CandidatePair(parent, childNum);
            return;
        }
        QSP<Node<Var, Res> > child;
        for (int j = 0; j < me->children.length() && i > -1; j++) {
            child = me->children.at(j);
            getCandidate(pair, me, child, j, i);
        }
    }

    static QSP<Node<Var, Res> > clone(QSP<Node<Var, Res> > source)
    {
        Node<Var, Res> *copied = source->copy();
        copied->depth = source->depth;
        copied->height = source->height;
        copied->size = source->size;
        for (int i = 0; i < source->children.length(); i++)
            copied->children.append(clone(source->children.at(i)));
        return QSP<Node<Var, Res> >(copied);
    }

    int depth;
    int height;
    int size;

};

#endif // NODE_H
