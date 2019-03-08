#include "GammarayObjectSelector.h"

#include "log/Log.h"

#include "core/selectors/ObjectsSelector.h"

#include <gammaray/common/objectmodel.h>

#include <QJSEngine>

namespace shakespear
{

GammarayObjectSelector::GammarayObjectSelector(
    QAbstractItemModel& model, QJSEngine& engine, QObject* parent)
    : QObject(parent), m_model(model), m_engine(engine)
{
}

QObject* GammarayObjectSelector::findObject(const QString& selector)
{
    const auto role = GammaRay::ObjectModel::ObjectRole;
    m_lastError.clear();

    try
    {
        return shakespear::ObjectsSelector(selector, &m_model, role)
            .findObject();
    }
    catch (const std::exception& ex)
    {
        m_engine.throwError(ex.what());
    }
    return nullptr;
}

QString GammarayObjectSelector::lastError() const
{
    return m_lastError;
}

} // namespace shakespear
