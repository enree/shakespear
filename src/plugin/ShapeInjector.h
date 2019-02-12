#pragma once

#include "gammaray/core/toolfactory.h"

#include <QWidget>

class QItemSelection;
class QItemSelectionModel;

namespace GammaRay
{
class PropertyController;
}

class ShapeInjector : public QObject
{
    Q_OBJECT
public:
    explicit ShapeInjector(GammaRay::Probe* probe, QObject* parent = 0);

private:
//    QItemSelectionModel* m_selectionModel;
//    GammaRay::PropertyController* m_propertyController;
};

class ShapeInjectorFactory
    : public QObject,
      public GammaRay::StandardToolFactory<QWidget, ShapeInjector>
{
    Q_OBJECT
    Q_INTERFACES(GammaRay::ToolFactory)
    Q_PLUGIN_METADATA(IID "com.kdab.GammaRay.ToolFactory" FILE
                          "shapeinjector.json")

public:
    explicit ShapeInjectorFactory(QObject* parent = 0) : QObject(parent) {}
};
