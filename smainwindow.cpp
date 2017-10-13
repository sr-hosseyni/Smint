#include <QtWidgets>

#include "smainwindow.h"

SMainWindow::SMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    editor = new STextEdit;
    setCentralWidget(editor);
    setWindowTitle(tr("Smint"));
}

QStringList SMainWindow::modelFromFile(const QString& fileName)
{
    QStringList words;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return words;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

    words.sort();

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return words;
}
