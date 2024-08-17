#include "resizablerectitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QDebug>

ResizableRectItem::ResizableRectItem(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsRectItem(rect, parent), isResizing(false), resizeHandle(NoHandle)
{
    setFlag(ItemIsSelectable);
    setFlag(ItemIsMovable);
}

void ResizableRectItem::startResizing(const QPointF &scenePos)
{
    isResizing = true;
    initialPos = scenePos;
    originalRect = rect();
   resizeHandle = getResizeHandle(scenePos);
}

void ResizableRectItem::resize(const QPointF &scenePos) {
    if (!isResizing) {
        return;
    }

    QPointF delta = scenePos - initialPos;

    QRectF newRect = originalRect;
    switch (resizeHandle) {
    case TopLeft:
        newRect.setTopLeft(newRect.topLeft() + delta);
        break;
    case TopRight:
        newRect.setTopRight(newRect.topRight() + delta);
        break;
    case BottomLeft:
        newRect.setBottomLeft(newRect.bottomLeft() + delta);
        break;
    case BottomRight:
        newRect.setBottomRight(newRect.bottomRight() + delta);
        break;
    default:
        break;
    }

    setRect(newRect);
}

void ResizableRectItem::endResizing() {
    isResizing = false;
    resizeHandle = NoHandle;
}

void ResizableRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
//    QPointF pos = mapToItem(event->pos());
//    if (getResizeHandle(pos) != NoHandle) {
//        startResizing(event->scenePos());
//    }
    QGraphicsRectItem::mousePressEvent(event);
}

QRectF ResizableRectItem::getResizeHandleRect(ResizeHandle handle) const
{
      if (handle == TopLeft) {
        return QRectF(rect().topLeft(), QSizeF(8, 8));
    }
}

ResizableRectItem::ResizeHandle ResizableRectItem::getResizeHandle(const QPointF &pos) const
{
     if (getResizeHandleRect(TopLeft).contains(pos)) {
        return TopLeft;
    }
    return NoHandle;
}
