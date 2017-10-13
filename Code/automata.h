#ifndef SAUTOMATA_H
#define SAUTOMATA_H

#include <QString>
#include <QMap>

struct State final
{
    QMap<QString, State> transitions;

    State();
    State addTransition(QString symbol, State state);
    State *next(QString symbol);
};

class Automata final
{
public:
    Automata();
    bool run(QString documnet);

private:
    bool walk(QString symbol);

private:
    State *currentState;
};

#endif // SAUTOMATA_H
