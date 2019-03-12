/** @file
 * @brief     Text edit sink widget template specification
 *
 * $Id: $
 */

#include "TextEditGetter.h"

#include <QWidget>

#pragma once

class QTabWidget;
class QTextEdit;

namespace logger
{

/**
 * Text edit sink widget template class
 */
class TextEditSinkWidget : public QWidget, public TextEditGetter
{
    Q_OBJECT

public:
    /**
     * Create TextEditSinkHub object
     */
    explicit TextEditSinkWidget(QWidget* parent = Q_NULLPTR);

    /**
     * Creates new tab with SinkWidget and specified @a name
     * end return it's internal text edit
     */
    QTextEdit* textEdit(const QString& name) override;

protected slots:
    void clear();

protected:
    QTextEdit* addSinkTab(const QString& name);

protected:
    // cppcheck-suppress unsafeClassCanLeak
    QTabWidget* m_tabWidget;
};

} // namespace logger
