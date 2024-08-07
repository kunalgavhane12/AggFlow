#include <addcommand.h>

AddCommand::AddCommand(QGraphicsScene* scene, QGraphicsItem* item, QUndoCommand* parent)
    : QUndoCommand(parent), GScene(scene), Item(item)
{

}

void AddCommand::undo()
{
    GScene->removeItem(Item);
    emit PublishUndoData(QString("(%1, %2)").arg(Item->pos().x()).arg(Item->pos().y()));
    emit NotifyUndoCompleted();
}

void AddCommand::redo()
{
    GScene->addItem(Item);
    emit PublishRedoData(QString("(%1, %2)").arg(Item->pos().x()).arg(Item->pos().y()));
    emit NotifyRedoCompleted();
}

RemoveCommand::RemoveCommand(QGraphicsScene* scene, QGraphicsItem* item, QUndoCommand* parent)
        : QUndoCommand(parent), GScene(scene), Item(item)
{

}

void RemoveCommand::undo()
{
    GScene->addItem(Item);
}

void RemoveCommand::redo()
{
    GScene->removeItem(Item);
}

MoveCommand::MoveCommand(QGraphicsItem* item, const QPointF& oldPos, const QPointF& newPos, QUndoCommand* parent)
    : QUndoCommand(parent), Item(item), OldPos(oldPos), NewPos(newPos)
{

}

void MoveCommand::undo()
{
    Item->setPos(OldPos);
    emit PublishUndoData(QString("(%1, %2)").arg(OldPos.x()).arg(OldPos.y()));
    emit NotifyUndoCompleted();
}

void MoveCommand::redo()
{
    Item->setPos(NewPos);
    emit PublishRedoData(QString("(%1, %2)").arg(NewPos.x()).arg(NewPos.y()));
    emit NotifyRedoCompleted();
}
