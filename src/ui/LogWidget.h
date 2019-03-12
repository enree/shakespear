#pragma once

#include "logging/TextEditSinkWidget.h"

namespace ui
{

class LogWidget : public logger::TextEditSinkWidget
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget* parent = NULL);
};

} // namespace ui
