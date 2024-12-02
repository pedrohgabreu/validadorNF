#include "dashboard.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Dashboard dashboard;
    dashboard.show();

    return app.exec();
}
