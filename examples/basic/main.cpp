#include "Widget.h"

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    Widget widget;
    widget.show();

    QApplication::exec();
}
