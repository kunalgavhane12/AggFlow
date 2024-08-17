#include "customshapeitem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

CustomShapeItem::CustomShapeItem(ShapeType shapeType, QGraphicsItem *parent)
    : QGraphicsItem(parent), shapeType(shapeType)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF CustomShapeItem::boundingRect() const
{
    if (shapeType == Line || shapeType == Arrow)
    {
        return QRectF(shapeLine.p1(), shapeLine.p2());
    }
    else
    {
        return shapeRect;
    }
}

QPainterPath CustomShapeItem::shape() const
{
    QPainterPath path;
    switch (shapeType)
    {
    case Rectangle:
        path.addRect(shapeRect);
        break;
    case Ellipse:
        path.addEllipse(shapeRect);
        break;
    case Line:
        path.moveTo(shapeLine.p1());
        path.lineTo(shapeLine.p2());
        break;
    case FixedLine:
        path.moveTo(shapeLine.p1());
        path.lineTo(shapeLine.p2());
        break;
    case Arrow:
        path.moveTo(shapeLine.p1());
        path.lineTo(shapeLine.p2());
        //ADD LOGIC FOR ARROW HEAD
        break;
    }
    return path;
}

void CustomShapeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    switch (shapeType)
    {
    case Rectangle:
        painter->drawRect(shapeRect);
        break;
    case Ellipse:
        painter->drawEllipse(shapeRect);
        break;
    case Line:
        painter->drawLine(shapeLine.p1(), shapeLine.p2());
        break;
    case FixedLine:
         painter->drawLine(shapeLine.p1(), shapeLine.p2());
        break;
    case Arrow:
        painter->drawLine(shapeLine.p1(), shapeLine.p2());
        // Add arrowhead drawing logic here
        break;
    }

    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
        addHandles();
    }
}

void CustomShapeItem::setShapeRect(const QRectF &rect)
{
    prepareGeometryChange();
    shapeRect = rect;
    update();
}

QRectF CustomShapeItem::getShapeRect() const
{
    return shapeRect;
}

void CustomShapeItem::setShapeLine(const QLineF &line)
{
    prepareGeometryChange();
    shapeLine = line;
    update();
}

QLineF CustomShapeItem::getShapeLine() const
{
    return shapeLine;
}
void CustomShapeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        qDebug()<<"SHAPECLICK";
        QGraphicsItem::mousePressEvent(event);
    }
}

void CustomShapeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void CustomShapeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

void CustomShapeItem::addHandles()
{
    removeHandles();
    if (shapeType == Rectangle || shapeType == Ellipse)
    {
        handles.append(new QGraphicsEllipseItem(QRectF(-5, -5, 10, 10), this));
        handles.append(new QGraphicsEllipseItem(QRectF(-5, -5, 10, 10), this));
        handles.append(new QGraphicsEllipseItem(QRectF(-5, -5, 10, 10), this));
        handles.append(new QGraphicsEllipseItem(QRectF(-5, -5, 10, 10), this));
    }
    else if (shapeType == Line)
    {
        handles.append(new QGraphicsEllipseItem(QRectF(-5, -5, 10, 10), this));
        handles.append(new QGraphicsEllipseItem(QRectF(-5, -5, 10, 10), this));
    }
    updateHandles();
}

void CustomShapeItem::removeHandles()
{
    qDeleteAll(handles);
    handles.clear();
}

void CustomShapeItem::updateHandles()
{
    for (int i = 0; i < handles.size(); ++i)
    {
        QGraphicsEllipseItem *handle = handles[i];
        if (shapeType == Rectangle || shapeType == Ellipse)
        {
            switch (i)
            {
            case TopLeft:
                handle->setPos(shapeRect.topLeft());
                break;
            case TopRight:
                handle->setPos(shapeRect.topRight());
                break;
            case BottomLeft:
                handle->setPos(shapeRect.bottomLeft());
                break;
            case BottomRight:
                handle->setPos(shapeRect.bottomRight());
                break;
            }
        }
        else if (shapeType == Line)
        {
            if (i == 0)
                handle->setPos(shapeLine.p1());
            else if (i == 1)
                handle->setPos(shapeLine.p2());
        }
    }
}

CustomShapeItem::HandleType CustomShapeItem::handleAt(const QPointF &point) const
{qDebug()<<"handleAt";
    for (int i = 0; i < handles.size(); ++i)
    {
        qDebug()<<"handleAt...";
        if (handles[i]->contains(point))
            return static_cast<HandleType>(i);
    }
    return TopLeft; // Default
}

void CustomShapeItem::resizeShape(HandleType handleType, const QPointF &newPos)
{
    qDebug()<<"resizeShape";
    if (shapeType == Rectangle || shapeType == Ellipse)
    {
        qDebug()<<"resizeShape...";
        switch (handleType)
        {
        case TopLeft:
            shapeRect.setTopLeft(newPos);
            break;
        case TopRight:
            shapeRect.setTopRight(newPos);
            break;
        case BottomLeft:
            shapeRect.setBottomLeft(newPos);
            break;
        case BottomRight:
            shapeRect.setBottomRight(newPos);
            break;
        }
    }
    else if (shapeType == Line)
    {
        if (handleType == TopLeft)
            shapeLine.setP1(newPos);
        else if (handleType == BottomLeft)
            shapeLine.setP2(newPos);
    }
    prepareGeometryChange();
    update();
}


//QPolygonF CustomShapeItem::scaledPolygon(const QPolygonF& old, CustomShapeItem::Direction direction, const QPointF& newPos)
//{
//    qreal oldWidth = old.boundingRect().width();
//    qreal oldHeight = old.boundingRect().height();
//    qreal scaleWidth, scaleHeight;
//    switch(direction)
//    {
//    case TopLeft:
//    {
//        QPointF fixPoint = old.boundingRect().bottomRight();
//        scaleWidth = (fixPoint.x() - newPos.x()) / oldWidth;
//        scaleHeight = (fixPoint.y() - newPos.y()) / oldHeight;
//        break;
//    }
//    case Top:
//    {
//        QPointF fixPoint = old.boundingRect().bottomLeft();
//        scaleWidth = 1;
//        scaleHeight = (fixPoint.y() - newPos.y()) / oldHeight;
//        break;
//    }
//    case TopRight:
//    {
//        QPointF fixPoint = old.boundingRect().bottomLeft();
//        scaleWidth = (newPos.x() - fixPoint.x()) / oldWidth;
//        scaleHeight = (fixPoint.y() - newPos.y() ) / oldHeight;
//        break;
//    }
//    case Right:
//    {
//        QPointF fixPoint = old.boundingRect().bottomLeft();
//        scaleWidth = (newPos.x() - fixPoint.x()) / oldWidth;
//        scaleHeight = 1;
//        break;
//    }
//    case BottomRight:
//    {
//        QPointF fixPoint = old.boundingRect().topLeft();
//        scaleWidth = (newPos.x() - fixPoint.x()) / oldWidth;
//        scaleHeight = (newPos.y() - fixPoint.y()) / oldHeight;
//        break;
//    }
//    case Bottom:
//    {
//        QPointF fixPoint = old.boundingRect().topLeft();
//        scaleWidth = 1;
//        scaleHeight = (newPos.y() - fixPoint.y()) / oldHeight;
//        break;
//    }
//    case BottomLeft: {
//        QPointF fixPoint = old.boundingRect().topRight();
//        scaleWidth = (fixPoint.x() - newPos.x()) / oldWidth;
//        scaleHeight = (newPos.y() - fixPoint.y()) / oldHeight;
//        break;
//    }
//    case Left:
//    {
//        QPointF fixPoint = old.boundingRect().bottomRight();
//        scaleWidth = (fixPoint.x() - newPos.x()) / oldWidth;
//        scaleHeight = 1;
//        break;
//    }
//    }
//    QTransform trans;
//    trans.scale(scaleWidth, scaleHeight);
//    return trans.map(old);
//}



