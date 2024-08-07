#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPointF>

class AddCommand : public QObject, public QUndoCommand {
    Q_OBJECT
public:
    AddCommand(QGraphicsScene* scene, QGraphicsItem* item, QUndoCommand* parent = nullptr);

protected:
    void undo() override;
    void redo() override;

signals:
    void NotifyUndoCompleted();
    void NotifyRedoCompleted();
    void PublishUndoData(QString data);
    void PublishRedoData(QString data);

private:
    QGraphicsScene* GScene;
    QGraphicsItem* Item;
};

class RemoveCommand : public QObject, public QUndoCommand {
    Q_OBJECT
public:
    RemoveCommand(QGraphicsScene* scene, QGraphicsItem* item, QUndoCommand* parent = nullptr);

protected:
    void undo() override;
    void redo() override;

signals:
    void NotifyUndoCompleted();
    void NotifyRedoCompleted();
    void PublishUndoData(QString data);
    void PublishRedoData(QString data);

private:
    QGraphicsScene* GScene;
    QGraphicsItem* Item;
};

class MoveCommand : public QObject, public QUndoCommand {
    Q_OBJECT
public:
    MoveCommand(QGraphicsItem* item, const QPointF& oldPos, const QPointF& newPos, QUndoCommand* parent = nullptr);

protected:
    void undo() override;
    void redo() override;

signals:
    void NotifyUndoCompleted();
    void NotifyRedoCompleted();
    void PublishUndoData(QString data);
    void PublishRedoData(QString data);

private:
    QGraphicsItem* Item;
    QPointF OldPos;
    QPointF NewPos;
};

#endif // ADDCOMMAND_H
