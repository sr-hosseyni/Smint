#ifndef SMAINWINDOW_H
#define SMAINWINDOW_H

#include "stextedit.h"

#include <QMainWindow>

class SMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SMainWindow(QWidget *parent = 0);

    static QStringList modelFromFile(const QString& fileName);

private:
    STextEdit *editor;
};

#endif // SMAINWINDOW_H
