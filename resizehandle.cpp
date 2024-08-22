#include "resizehandle.h"
#include <QGraphicsScene>

ResizeHandle::ResizeHandle(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), m_resizeItem(nullptr)
{
    setRect(-5, -5, 10, 10);
    setBrush(QBrush(Qt::red));
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
}

void ResizeHandle::setResizeItem(QGraphicsItem *item)
{
    m_resizeItem = item;
}

QGraphicsItem* ResizeHandle::resizeItem() const
{
    return m_resizeItem;
}

void ResizeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizeItem)
    {
        QPointF newPos = mapToParent(event->pos());

        if (auto rectItem = dynamic_cast<QGraphicsRectItem*>(m_resizeItem))
        {
            QRectF newRect(rectItem->rect().topLeft(), newPos);
            rectItem->setRect(newRect.normalized());
        }
        else if (auto ellipseItem = dynamic_cast<QGraphicsEllipseItem*>(m_resizeItem))
        {
            QRectF newRect(ellipseItem->rect().topLeft(), newPos);
            ellipseItem->setRect(newRect.normalized());
        }
        else if (auto lineItem = dynamic_cast<QGraphicsLineItem*>(m_resizeItem))
        {
            QPointF handlePos = mapToParent(event->pos());
            if (QLineF(handlePos, lineItem->line().p1()).length() < QLineF(handlePos, lineItem->line().p2()).length())
            {
                m_isResizingStart = true;
            }
            else
            {
                m_isResizingStart = false;
            }
            QLineF line = lineItem->line();
            if (m_isResizingStart)
            {
                line.setP1(mapToScene(event->pos()));
            }
            else
            {
                line.setP2(mapToScene(event->pos()));
            }
            lineItem->setLine(line);
        }
        m_resizeItem->update();
    }

    QGraphicsEllipseItem::mouseMoveEvent(event);
}
