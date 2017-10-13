#include "automata.h"
#include <QList>
#include <QString>

Automata::Automata()
{
    currentState = new State();
}

bool Automata::run(QString documnet)
{
    QStringList words = documnet.split(QRegExp("[~!@#\\$%\\^&\\*\\(\\)_+{}\\|:\"<>\\?,\\./;'\\[\\]\\-=]"));
    for (int i=0; i < words.length(); i++)
    {
        walk(words[i]);
    }
}

bool Automata::walk(QString symbol)
{
    currentState = currentState->next(symbol);

    return currentState != 0;
}

/**
 * Class state
 */
State::State()
{
    transitions = QMap<QString, State>();
}

State State::addTransition(QString symbol, State state)
{
    return transitions[symbol] = state;
}

State *State::next(QString symbol)
{
    if (transitions.contains(symbol))
        return &transitions[symbol];

    return 0;
}
