#include "GammarayObjectSelector.h"

#include "core/selectors/ObjectsSelector.h"

#include <gammaray/common/objectmodel.h>

namespace shakespear
{

GammarayObjectSelector::GammarayObjectSelector(
    QAbstractItemModel& model, QObject* parent)
    : QObject(parent), m_model(model)
{
}

QObject* GammarayObjectSelector::findObject(const QString& selector)
{
    const auto role = GammaRay::ObjectModel::ObjectRole;
    return shakespear::ObjectsSelector(selector, &m_model, role).findObject();
}

} // namespace shakespear
