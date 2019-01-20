#pragma once

#include <QObject>

namespace shakespear
{

class QObjectSubclass : public QObject
{
    Q_OBJECT
};

class QObjectSubSubclass : public QObjectSubclass
{
    Q_OBJECT
};

class QAlternativeSubSubclass : public QObjectSubclass
{
    Q_OBJECT
};

} // namespace shakespear
