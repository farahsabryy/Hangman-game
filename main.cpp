#include <QApplication>
#include "mainmenu.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainMenu mainMenu;
    mainMenu.show();

    return app.exec();
}
