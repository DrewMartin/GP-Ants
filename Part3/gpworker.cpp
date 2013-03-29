#include "gpworker.h"

GPWorker::GPWorker(BlockingQueue<int> *work,
                   BlockingQueue<QSP<AntNode> > *nextGen,
                   BlockingCounter *counter,
                   QList<QSP<AntNode> > *pop) :
    work(work),
    nextGen(nextGen),
    counter(counter),
    pop(pop)
{
}
#define BEST_CHANCE 90

QSP<AntNode> GPWorker::tournamentSelect()
{
    QList<QSP<AntNode> > tournament;
    QSP<AntNode> selection, best;

    for (int i = 0; i < TOURNAMENT_SIZE; i++) {
        selection = pop->at(RAND_INT(pop->length()));
        tournament += selection;
        if (best.isNull() || best->getScore() < selection->getScore())
            best = selection;
    }

    if (!best.isNull() && RAND_INT(100) < BEST_CHANCE)
        return best;

    return tournament.at(RAND_INT(TOURNAMENT_SIZE));
}


void GPWorker::run()
{
    int task;
    QSP<Node> tempNode;
    QSP<AntNode> n1, n2;
    Simulation *sim;
    while (true) {
        task = work->take();
        if (task == WORK_QUIT)
            return;
        if (task == WORK_GEN) {
            if (RAND_INT(100) < XOVER_PROB) {
                while (true) {
                    n1 = tournamentSelect();
                    n2 = tournamentSelect();
                    tempNode = AntNode::crossover(n1, n2);
                    if (!tempNode.isNull()) {
                        n1 = tempNode.staticCast<AntNode>();
                        nextGen->put(n1);
                        break;
                    }
                }
            } else {
                while (true) {
                    n1 = tournamentSelect();
                    tempNode = AntNode::mutate(n1);
                    if (!tempNode.isNull()) {
                        n1 = tempNode.staticCast<AntNode>();
                        nextGen->put(n1);
                        break;
                    }
                }
            }
        } else {
            if (pop->at(task)->isScored()) {
                counter->increment();
            } else {
                sim = new Simulation(MAX_TURNS);
                sim->setBehaviour(pop->at(task));
                sim->run();
                pop->at(task)->setScore(sim->getScore());
                counter->increment();
                delete sim;
            }
        }
    }
}
