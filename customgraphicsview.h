#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QPointF>
#include <QMap>
#include <QPolygonF>
#include <QPainterPath>
#include "CustomPixmapItem.h"
#include "arrowlineitem.h"
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QUndoStack>
#include <QMessageBox>
#include <QIcon>
#include <QInputDialog>
#include <addcommand.h>
#include <QDebug>
#include <QApplication>
#include <QDomDocument>
#include <QBuffer>
#include "customshapeitem.h"

using LineConnectionsMap = QMap<QGraphicsLineItem *, QPair<QGraphicsEllipseItem *, QGraphicsEllipseItem *>>;

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    CustomGraphicsView(QWidget *parent = nullptr);
    void ClearScene();
    void setFixedSizeAndScene(const QSize& size);

    void setShapeType(CustomShapeItem::ShapeType shape);
    void handleProxyWidgetInteraction(const QPointF &scenePos, QGraphicsProxyWidget *proxyWidget);
    void handleEllipseInteraction(const QPointF &scenePos, QGraphicsEllipseItem *ellipseItem);
    void handleItemInteraction(const QPointF &scenePos, QGraphicsItem *item);

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
    void resizeEvent(QResizeEvent *event) override;

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
    void onSetValue();

public slots:
    void onActionDelete();
    void saveToFile(const QString &fileName);
    void loadFromFile(const QString &fileName);
    void onResult();
    void saveToXml(const QString &fileName);
    void loadFromXml(const QString &fileName);
    void onActionTopTriggered();
    void onActionBottomTriggered();
    void onActionLeftTriggered();
    void onActionRightTriggered();

private:
    void RemoveLines();
    void RemoveAllLines();
    void reconnectLines(QList<ArrowLineItem*> lineItems, QMap<int, CustomPixmapItem*> customItems);
    void EmitDebugData(QPoint pos);
    void AddItemToAddStack(QGraphicsItem *item);
    void AddItemToMoveStack(QGraphicsItem *item);

    LineConnectionsMap lineConnections;
    QGraphicsScene *scene;
    QGraphicsItem *currentItem = nullptr;
    QGraphicsItem *selectedItem = nullptr;
    ArrowLineItem *currentLine = nullptr;
    QPointF startPoint;
    QPointF lineStartPoint;
    QPointF itemStartPosition;
    QMenu contextMenu;
    QAction *acnDel;
    QAction *acnSetVal;
    QAction *acnResult;
    QUndoStack* UndoStack;

    CustomShapeItem *currentItem1 = nullptr;
    bool drawing;
    CustomShapeItem::ShapeType shapeType;
    QPointF origin;
};

#endif // CUSTOMGRAPHICSVIEW_H
