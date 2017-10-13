#include "smainwindow.h"

#include <QApplication>

#include <Code/automata.h>

int main(int argc, char *argv[])
{
//    Q_INIT_RESOURCE(smint);

//    Automata a = Automata();
//    a.run("namespace xxx;");

    QApplication app(argc, argv);

    SMainWindow mainWindow;
    mainWindow.resize(640, 512);
    mainWindow.show();

    return app.exec();
}
