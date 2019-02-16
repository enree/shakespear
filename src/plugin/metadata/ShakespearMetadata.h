#pragma once

#include "plugin/ShakespearPlugin.h"

#include <gammaray/core/toolfactory.h>

class ShapeInjectorFactory
    : public QObject,
      public GammaRay::
          StandardToolFactory<QObject, shakespear::ShakespearPlugin>
{
    Q_OBJECT
    Q_INTERFACES(GammaRay::ToolFactory)
    Q_PLUGIN_METADATA(IID "com.kdab.GammaRay.ToolFactory" FILE
                          "shakespearplugin.json")

public:
    explicit ShapeInjectorFactory(QObject* parent = 0) : QObject(parent) {}
};
