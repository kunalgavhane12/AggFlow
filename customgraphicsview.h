#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QPointF>
#include <QMap>
#include "CustomPixmapItem.h"
#include <arrowlineitem.h>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QUndoStack>

using LineConnectionsMap = QMap<QGraphicsLineItem *, QPair<QGraphicsEllipseItem *, QGraphicsEllipseItem *>>;

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CustomGraphicsView(QWidget *parent = nullptr);
    void ClearScene();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event)override;

signals:
    void UndoTriggered();
    void RedoTriggered();
    void PublishOldData(QString data);
    void PublishNewData(QString data);
    void PublishUndoData(QString data);
    void PublishRedoData(QString data);
    void resultUpdated(const QString &result);

private slots:
    void updateLinePosition();
    void onActionSave();
    void onActionDelete();
    void onSetValue();

public slots:
    void saveToFile(const QString &fileName);
    void loadFromFile(const QString &fileName);
    void onResult();
    void saveToXml(const QString &fileName);
    void loadFromXml(const QString &fileName);

private:
    void RemoveLines();
    void RemoveAllLines();
    void reconnectLines(QList<ArrowLineItem*> lineItems, QMap<int, CustomPixmapItem*> customItems);
    void EmitDebugData(QPoint pos);
    void AddItemToAddStack(QGraphicsItem *item);
    void AddItemToMoveStack(QGraphicsItem *item);

    QGraphicsScene *scene;
    ArrowLineItem *currentLine;
    QPointF lineStartPoint;
    LineConnectionsMap lineConnections;
    QMenu contextMenu;
    QAction *acnSave;
    QAction *acnDel;
    QAction *acnSetVal;
    QAction *acnResult;
    QGraphicsItem *selectedItem = nullptr;
    QPointF itemStartPosition;
    QUndoStack* UndoStack;
};

#endif // CUSTOMGRAPHICSVIEW_H
