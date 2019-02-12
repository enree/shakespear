#include "ShapeInjector.h"

#include <gammaray/common/objectbroker.h>
#include <gammaray/common/objectmodel.h>
#include <gammaray/core/probe.h>
#include <gammaray/core/objecttypefilterproxymodel.h>
#include <gammaray/core/propertycontroller.h>

#include <QItemSelectionModel>
#include <QLCDNumber>
#include <QFile>
#include <QTextStream>
#include <QTimer>

ShapeInjector::ShapeInjector(GammaRay::Probe* probe, QObject* parent)
    : QObject(parent)
//    , m_propertyController(
//          new GammaRay::
//              PropertyController("com.kdab.GammaRay.ShapeInjector", this))
{
//    QAbstractProxyModel* model
//        = new GammaRay::ObjectTypeFilterProxyModel<QLCDNumber>(this);
//    model->setSourceModel(probe->objectListModel());
//    probe->registerModel("com.kdab.GammaRay.LCDWidgetModel", model);

//    m_selectionModel = GammaRay::ObjectBroker::selectionModel(model);

    QTimer::singleShot(300, [probe](){
        QFile file("/home/yuri/out.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);

        auto model = probe->objectTreeModel();
        out << model->rowCount();
        for (int i = 0; i < model->rowCount(); ++ i)
        {
            out << model->index(i, 0).data(Qt::DisplayRole).toString() << "|"
                << model->index(i, 1).data(Qt::DisplayRole).toString() << "\n";
        }

        auto indices = model->match(
            model->index(0, 1),
            Qt::DisplayRole,
            "QLCDNumber",
            1,
            Qt::MatchExactly | Qt::MatchRecursive);

        out << indices.size();
        if (indices.size() > 0)
        {
            auto index = indices.first();
            auto object = index.data(GammaRay::ObjectModel::ObjectRole).value<QObject*>();
            out << (object != nullptr);

            if (auto number = qobject_cast<QLCDNumber*>(object))
            {
                number->display(2);
            }
        }
    });


}
