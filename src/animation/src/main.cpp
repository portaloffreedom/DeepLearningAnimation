#include "graphics/nn_debug_view.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    nn_debug_view w;
    w.show();

    return app.exec();
}

