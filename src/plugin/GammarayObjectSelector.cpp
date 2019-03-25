#include "GammarayObjectSelector.h"

#include "log/Log.h"

#include "core/selectors/ObjectsSelector.h"

#include <gammaray/common/objectmodel.h>

#include <QJSEngine>

namespace shakespear
{

template <typename C>
QVariantList toVariantList(const C& container)
{
    QVariantList variantList;
    variantList.reserve(container.size());
    std::transform(
        container.begin(),
        container.end(),
        std::back_inserter(variantList),
        [](auto v) { return QVariant::fromValue(v); });
    return variantList;
}

const auto role = GammaRay::ObjectModel::ObjectRole;

GammarayObjectSelector::GammarayObjectSelector(
    QAbstractItemModel& model, QJSEngine& engine, QObject* parent)
    : QObject(parent), m_model(model), m_engine(engine)
{
}

QObject* GammarayObjectSelector::findObject(const QString& selector)
{
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

QObject* GammarayObjectSelector::findFirstObject(const QString& selector)
{
    m_lastError.clear();

    try
    {
        return shakespear::ObjectsSelector(selector, &m_model, role)
            .findFirstObject();
    }
    catch (const std::exception& ex)
    {
        m_engine.throwError(ex.what());
    }
    return nullptr;
}

QVariantList GammarayObjectSelector::findObjects(const QString& selector)
{
    m_lastError.clear();

    try
    {
        auto objects = shakespear::ObjectsSelector(selector, &m_model, role)
                           .findObjects();
        return toVariantList(objects);
    }
    catch (const std::exception& ex)
    {
        m_engine.throwError(ex.what());
    }
    return {};
}

QString GammarayObjectSelector::lastError() const
{
    return m_lastError;
}

} // namespace shakespear
