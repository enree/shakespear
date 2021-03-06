#pragma once

#include "core/selectors/LookupError.h"

#include <QObject>
#include <QVariantList>

class QAbstractItemModel;
class QJSEngine;

namespace shakespear
{

class GammarayObjectSelector : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString lastError READ lastError)
public:
    GammarayObjectSelector(
        QAbstractItemModel& model,
        QJSEngine& engine,
        QObject* parent = nullptr);

    /**
      Find object from AUT by selector
    */
    Q_INVOKABLE QObject* findObject(const QString& selector);

    /**
      Find first object from AUT by selector
    */
    Q_INVOKABLE QObject* findFirstObject(const QString& selector);

    /**
      Find all objects from AUT by selector
    */
    Q_INVOKABLE QVariantList findObjects(const QString& selector);

    /**
      Return last error in selection objects
      */
    Q_INVOKABLE QString lastError() const;

private:
    QAbstractItemModel& m_model;
    QString m_lastError;

    QJSEngine& m_engine;
};

} // namespace shakespear
