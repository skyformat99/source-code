/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>

#include "commands.h"
#include "diagramitem.h"

MoveCommand::MoveCommand(DiagramItem *diagramItem, const QPointF &oldPos,
                 QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myDiagramItem = diagramItem;
    newPos = diagramItem->pos();
    myOldPos = oldPos;
}

bool MoveCommand::mergeWith(const QUndoCommand *command)
{
    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
    DiagramItem *item = moveCommand->myDiagramItem;

    if (myDiagramItem != item)
    return false;

    newPos = item->pos();
    setText(QObject::tr("Move %1")
        .arg(createCommandString(myDiagramItem, newPos)));

    return true;
}

void MoveCommand::undo()
{
    myDiagramItem->setPos(myOldPos);
    /* The original example has the following two lines. But they are redundant */
	//myDiagramItem->scene()->update();
    //setText(QObject::tr("Move %1")
    //    .arg(createCommandString(myDiagramItem, newPos)));
}

void MoveCommand::redo()
{
    myDiagramItem->setPos(newPos);
    setText(QObject::tr("Move %1")
        .arg(createCommandString(myDiagramItem, newPos)));
}



DeleteCommand::DeleteCommand(QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myGraphicsScene = scene;
    QList<QGraphicsItem *> list = myGraphicsScene->selectedItems();
    list.first()->setSelected(false);
    myDiagramItem = static_cast<DiagramItem *>(list.first());
    setText(QObject::tr("Delete %1")
        .arg(createCommandString(myDiagramItem, myDiagramItem->pos())));
}
//! [4]

//! [5]
void DeleteCommand::undo()
{
    myGraphicsScene->addItem(myDiagramItem);
    myGraphicsScene->update();
}
//! [5]

//! [6]
void DeleteCommand::redo()
{
    myGraphicsScene->removeItem(myDiagramItem);
}
//! [6]

//! [7]
AddCommand::AddCommand(DiagramItem::DiagramType addType,
                       QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    static int itemCount = 0;

    myGraphicsScene = scene;
    myDiagramItem = new DiagramItem(addType);
    initialPosition = QPointF((itemCount * 15) % int(scene->width()),
                              (itemCount * 15) % int(scene->height()));
    scene->update();
    ++itemCount;
    setText(QObject::tr("Add %1")
        .arg(createCommandString(myDiagramItem, initialPosition)));
}
//! [7]

//! [8]
void AddCommand::undo()
{
    myGraphicsScene->removeItem(myDiagramItem);
    myGraphicsScene->update();
}
//! [8]

//! [9]
void AddCommand::redo()
{
    myGraphicsScene->addItem(myDiagramItem);
    myDiagramItem->setPos(initialPosition);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}
//! [9]

QString createCommandString(DiagramItem *item, const QPointF &pos)
{
    return QObject::tr("%1 at (%2, %3)")
        .arg(item->diagramType() == DiagramItem::Box ? "Box" : "Triangle")
        .arg(pos.x()).arg(pos.y());
}
