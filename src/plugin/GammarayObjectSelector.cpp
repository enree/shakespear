#include "GammarayObjectSelector.h"

#include "log/Log.h"

#include "core/selectors/ObjectsSelector.h"

#include <gammaray/common/objectmodel.h>

#include <QJSEngine>

namespace shakespear
{

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
        QVariantList variantList;
        for (auto object: objects)
        {
            variantList.push_back(QVariant::fromValue(object));
        }
        return variantList;
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
