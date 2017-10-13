#include "stextedit.h"
#include "ssyntaxhighlighter.h"
#include "smainwindow.h"
#include <QStringListModel>
#include <QAbstractItemView>
#include <QScrollBar>

STextEdit::STextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    setFont(font);

    QStringList keywords = SMainWindow::modelFromFile(":/resources/php/keywords.txt");

    highlighter = new SSyntaxHighlighter(keywords, document());

    completer = new QCompleter(this);
    completer->setModel(new QStringListModel(keywords, completer));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    setCompleter(completer);

    const int tabStop = 4;  // 4 characters

    QFontMetrics metrics(font);
    setTabStopWidth(tabStop * metrics.width(' '));
}

void STextEdit::setCompleter(QCompleter *c)
{
    if (completer)
        QObject::disconnect(completer, 0, this, 0);

    completer = c;

    if (!completer)
        return;

    completer->setWidget(this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(completer, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}

QCompleter *STextEdit::getCompleter() const
{
    return completer;
}

void STextEdit::insertCompletion(const QString& completion)
{
    if (completer->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString STextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void STextEdit::focusInEvent(QFocusEvent *e)
{
    if (completer)
        completer->setWidget(this);
    QTextEdit::focusInEvent(e);
}

void STextEdit::keyPressEvent(QKeyEvent *e)
{
    if (completer && completer->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    } else {
        if (e->key() == Qt::Key_Tab)
        {
            insertPlainText("    ");
            e->ignore();
            return;
        }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!completer || !isShortcut) // do not process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!completer || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 1
                      || eow.contains(e->text().right(1)))) {
        completer->popup()->hide();
        return;
    }

    if (completionPrefix != completer->completionPrefix()) {
        completer->setCompletionPrefix(completionPrefix);
        completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(completer->popup()->sizeHintForColumn(0)
                + completer->popup()->verticalScrollBar()->sizeHint().width());
    completer->complete(cr); // popup it up!
}
