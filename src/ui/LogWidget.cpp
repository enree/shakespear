#include "LogWidget.h"

#include <QGridLayout>
#include <QSpacerItem>
#include <QTabWidget>
#include <QTextEdit>
#include <QToolButton>

namespace ui
{

LogWidget::LogWidget(QWidget* parent)
{
    auto gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(4);
    gridLayout->setContentsMargins(2, 2, 2, 2);

    auto tbtnClear = new QToolButton(this);
    tbtnClear->setToolTip(tr("Clear"));
    tbtnClear->setIcon(QIcon(":/clear"));
    gridLayout->addWidget(tbtnClear, 0, 0, 1, 1);

    gridLayout->addWidget(m_tabWidget, 0, 1, 2, 1);

    gridLayout->addItem(
        new QSpacerItem(10, 0, QSizePolicy::Minimum, QSizePolicy::Expanding),
        1,
        0,
        1,
        1);

    connect(tbtnClear, &QToolButton::clicked, this, &LogWidget::clear);
}

} // namespace ui
