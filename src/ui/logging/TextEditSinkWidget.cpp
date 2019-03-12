/** @file
 * @brief     Text edit sink widget implementation
 *
 * $Id: $
 */

#include "TextEditSinkWidget.h"

#include <QBoxLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QToolButton>
#include <QWidget>

namespace logger
{

TextEditSinkWidget::TextEditSinkWidget(QWidget* parent)
    : QWidget(parent), m_tabWidget(new QTabWidget(this))
{
}

QTextEdit* TextEditSinkWidget::textEdit(const QString& name)
{
    return addSinkTab(name);
}

QTextEdit* TextEditSinkWidget::addSinkTab(const QString& name)
{
    auto logArea = new QTextEdit(m_tabWidget);
    logArea->setUndoRedoEnabled(false);
    logArea->setReadOnly(true);
    m_tabWidget->addTab(logArea, name);

    return logArea;
}

void TextEditSinkWidget::clear()
{
    QWidget* current = m_tabWidget->currentWidget();
    if (auto logArea = qobject_cast<QTextEdit*>(current))
    {
        logArea->clear();
    }
}

} // namespace logger
