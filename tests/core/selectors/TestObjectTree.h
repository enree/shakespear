#pragma once

#include "DummyObjects.h"
#include "TreeModel.h"

namespace shakespear
{

/*
 *  The model has the following form:
 *  QObject#root
 *      QObjectSubclass#widget
 *          QObjectSubSubclass#label[text=label1]
 *          QAlternativeSubSubclass#edit[text=edit1][classes=own shared]
 *      QObjectSubSubclass#label[text=label2]
 *          QAlternativeSubSubclass#edit2[text=edit][classes=otherown
 *              shared]
 *      QAlternativeSubSubclass#edit3[text=edit]
 */
class TestObjectTree : public models::TreeModel
{
    Q_OBJECT
public:
    enum Roles
    {
        QObjectRole = Qt::UserRole + 1
    };

    explicit TestObjectTree(QObject* parent = nullptr);
};

} // namespace shakespear
