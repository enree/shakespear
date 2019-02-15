#pragma once

#include "GammarayObjectSelector.h"

#include "gammaray/core/toolfactory.h"

#include <QJSEngine>
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
    // This class is owned by JS engine
    shakespear::GammarayObjectSelector* m_selector;
    QJSEngine m_engine;
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
