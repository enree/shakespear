#pragma once

#include "core/selectors/LookupError.h"

#include <QObject>

class QAbstractItemModel;

namespace shakespear
{

class GammarayObjectSelector : public QObject
{
    Q_OBJECT
public:
    GammarayObjectSelector(
        QAbstractItemModel& model, QObject* parent = nullptr);

    /**
      Find object from AUT by selector
    */
    Q_INVOKABLE QObject* findObject(const QString& selector);

    /**
      Find object from AUT by selector, cast to T*. Throw if either no object is
      found or no cast performed
    */
    template <typename T>
    T* object(const QString& selector)
    {
        T* o = qobject_cast<T*>(findObject(selector));
        if (o == nullptr)
        {
            BOOST_THROW_EXCEPTION(exception::ObjectNotFound());
        }
        return o;
    }

private:
    QAbstractItemModel& m_model;
};

} // namespace shakespear
