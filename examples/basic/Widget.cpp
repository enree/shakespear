#include "Widget.h"
#include "ui_Widget.h"

#include <QPushButton>
#include <QTextEdit>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pBut, &QPushButton::clicked, this, [this]() {
        ui->testEdit->setText("CLICKED!");
    });
}

Widget::~Widget()
{
    delete ui;
}
