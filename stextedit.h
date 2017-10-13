#ifndef STEXTEDIT_H
#define STEXTEDIT_H

#include <QTextEdit>
#include <QCompleter>

#include "ssyntaxhighlighter.h"

class STextEdit : public QTextEdit
{
    Q_OBJECT

public:
    STextEdit(QWidget *parent = 0);

    void setCompleter(QCompleter *c);
    QCompleter *getCompleter() const;

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;

private slots:
    void insertCompletion(const QString &completion);

private:
    QString textUnderCursor() const;

private:
    SSyntaxHighlighter *highlighter;
    QCompleter *completer;
};

#endif // STEXTEDIT_H
