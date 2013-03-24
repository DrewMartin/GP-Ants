#ifndef NODE_H
#define NODE_H

#include <QList>
#include <QSharedPointer>
#include "common.h"
#include <QPair>
#include <QDebug>

#define CandidatePair QPair<QSP<Node<Var, Res> >, QSP<Node<Var, Res> > >
#define CandidatePairList QList<CandidatePair>

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

    int getChildCount() { return children.length(); }
    QSP<Node<Var, Res> > getChild(int i) {
        if (i < 0 || i >= getChildCount())
            return QSP<Node<Var, Res> >();
        return children.at(i);
    }

    void setDepth(int d) { depth = d; }
    void updateHeightDepth() {
        updateHeightDepth(0);
    }

    void setChild(int i, QSP<Node<Var, Res> > node) {
        if (i < 0 || i >= getChildCount())
            return;
        children[i] = node;
    }

    static QSP<Node<Var, Res> > mutate(QSP<Node<Var, Res> > node)
    {
        CandidatePairList nodeList;
        QSP<Node<Var, Res> > cloned = clone(node);
        populateChildList(nodeList, cloned);
        if (nodeList.length() == 0)
            return QSP<Node<Var, Res> >();
        int i = RAND_INT(nodeList.length());
        CandidatePair candidate = nodeList.at(i);
        int maxHeight = MAX_HEIGHT - candidate.first->getDepth();
        if (maxHeight <= 0)
            return QSP<Node<Var, Res> >();
        maxHeight = RAND_INT(maxHeight);
        for (i = 0; i < candidate.first->children.length(); i++)
            if (candidate.first->children.at(i) == candidate.second)
                break;
        candidate.first->children[i] = node->generateSubtree(maxHeight);
        cloned->updateHeightDepth();
        return cloned;
    }

    static QSP<Node<Var, Res> > crossover(QSP<Node<Var, Res> > n1, QSP<Node<Var, Res> > n2)
    {
        CandidatePairList n1Nodes, n2Nodes;
        QSP<Node<Var, Res> > clone1 = clone(n1);
        CandidatePair choice1(QSP<Node<Var, Res> >(), clone1), choice2(QSP<Node<Var, Res> >(), n2);
        n1Nodes.append(choice1);
        n2Nodes.append(choice2);
        populateChildList(n1Nodes, clone1);
        populateChildList(n2Nodes, n2);
        int rand1, rand2, i;

        while(true) {
            rand1 = RAND_INT(n1Nodes.length());
            rand2 = RAND_INT(n2Nodes.length());
            choice1 = n1Nodes.at(rand1);
            choice2 = n2Nodes.at(rand2);
            if (choice1.first.isNull() || choice1.first->getDepth() + choice2.second->getHeight() <= MAX_HEIGHT)
                break;
        }

        if (choice1.first.isNull()) {
            clone1 = clone(choice2.second);
        } else {
            for (i = 0; i < choice1.first->children.length(); i++)
                if (choice1.first->children.at(i) == choice1.second)
                    break;
            choice1.first->children[i] = clone(choice2.second);
        }

        clone1->updateHeightDepth();
        return clone1;
    }


protected:
    QList<QSharedPointer<Node<Var, Res> > > children;

    virtual Node<Var, Res> *copy() = 0;
    virtual QSP<Node<Var, Res> > generateSubtree(int maxHeight) = 0;

private:
    int updateHeightDepth(int depth)
    {
        this->depth = depth;
        int maxHeight = -1;
        for (int i = 0; i < children.length(); i++)
            maxHeight = qMax(maxHeight, children.at(i)->updateHeightDepth(depth + 1));
        height = maxHeight + 1;
        return height;
    }

    static void populateChildList(CandidatePairList &list, QSP<Node<Var, Res> > parent)
    {
        CandidatePair pair;
        for (int i = 0; i < parent->children.length(); i++) {
            pair = CandidatePair(parent, parent->children.at(i));
            list.append(pair);
            populateChildList(list, parent->children.at(i));
        }
    }

    static QSP<Node<Var, Res> > clone(QSP<Node<Var, Res> > source)
    {
        Node<Var, Res> *copied = source->copy();
        copied->depth = source->depth;
        copied->height = source->height;
        for (int i = 0; i < source->children.length(); i++)
            copied->children.append(clone(source->children.at(i)));
        return QSP<Node<Var, Res> >(copied);
    }

    int depth;
    int height;

};

#endif // NODE_H
