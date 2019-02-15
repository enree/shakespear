#include "ShapeInjector.h"

#include "GammarayObjectSelector.h"

#include "core/selectors/LookupError.h"

#include <gammaray/core/probe.h>

#include <QItemSelectionModel>
#include <QLineEdit>
#include <QTextStream>
#include <QTimer>

ShapeInjector::ShapeInjector(GammaRay::Probe* probe, QObject* parent)
    : QObject(parent)
{
    QTimer::singleShot(100, this, [this, probe]() { // NOLINT
        auto model = probe->objectTreeModel();

        m_selector = new shakespear::GammarayObjectSelector(*model);

        auto selector = m_engine.newQObject(m_selector);
        m_engine.globalObject().setProperty("Shakespear", selector);
        m_engine.evaluate(
            "var lineEdit = Shakespear.findObject('.QLineEdit'); "
            "lineEdit.setText('Welcome, JS');"
            "Shakespear.findObject('QLabel').setText('New label');");

        try
        {
            m_selector->object<QWidget>("#Widget")->move(300, 300);
        }
        catch (const shakespear::exception::LookupError& ex)
        {
        }
    });
}
