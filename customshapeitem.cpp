#include "customshapeitem.h"
#include "resizehandle.h"  // Include the header for ResizeHandle
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <cmath>

CustomShapeItem::CustomShapeItem(ShapeType shapeType, QGraphicsItem *parent)
    : QGraphicsItem(parent), shapeType(shapeType)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    addHandles();
}

QRectF CustomShapeItem::boundingRect() const
{
    if (shapeType == Line || shapeType == Arrow)
    {
        return QRectF(shapeLine.p1(), shapeLine.p2()).normalized();
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
    case ConvLine:
    case ConvReverseLine:
    case Line:
        path.moveTo(shapeLine.p1());
        path.lineTo(shapeLine.p2());
        break;
    case Rectangle:
        path.addRect(shapeRect);
        break;
    case Ellipse:
        path.addEllipse(shapeRect);
        break;
    case Arrow:
        {
            QLineF line(shapeLine);
            path.moveTo(line.p1());
            path.lineTo(line.p2());

            double angle = std::atan2(-line.dy(), line.dx());
            QPointF arrowP1 = line.p2() - QPointF(sin(angle + M_PI / 3) * 10, cos(angle + M_PI / 3) * 10);
            QPointF arrowP2 = line.p2() - QPointF(sin(angle + M_PI - M_PI / 3) * 10, cos(angle + M_PI - M_PI / 3) * 10);

            QPolygonF arrowHead;
            arrowHead << line.p2() << arrowP1 << arrowP2;

            path.addPolygon(arrowHead);
        }
        break;
    case PolygonLine:
        path.moveTo(shapeLine.p1());
        path.lineTo(shapeLine.p2());
        break;
    }
    return path;
}

void CustomShapeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    switch (shapeType)
    {
    case ConvLine:
        painter->drawLine(shapeLine.p1(), shapeLine.p2());
        painter->setBrush(Qt::white);
        painter->drawEllipse(shapeLine.p1(), 5, 5);
        painter->setBrush(Qt::green);
        painter->drawEllipse(shapeLine.p2(), 5, 5);
        break;
    case ConvReverseLine:
        painter->drawLine(shapeLine.p1(), shapeLine.p2());
        painter->setBrush(Qt::yellow);
        painter->drawEllipse(shapeLine.p1(), 5, 5);
        painter->setBrush(Qt::green);
        painter->drawEllipse(shapeLine.p2(), 5, 5);
        break;
    case Line:
        painter->drawLine(shapeLine.p1(), shapeLine.p2());
        break;
    case Rectangle:
        painter->drawRect(shapeRect);
        break;
    case Ellipse:
        painter->drawEllipse(shapeRect);
        break;
    case Arrow:
        {
            QLineF line(shapeLine);
            painter->drawLine(line);

            double angle = std::atan2(-line.dy(), line.dx());
            QPointF arrowP1 = line.p2() - QPointF(sin(angle + M_PI / 3) * 10, cos(angle + M_PI / 3) * 10);
            QPointF arrowP2 = line.p2() - QPointF(sin(angle + M_PI - M_PI / 3) * 10, cos(angle + M_PI - M_PI / 3) * 10);

            QPolygonF arrowHead;
            arrowHead << line.p2() << arrowP1 << arrowP2;

            painter->setBrush(Qt::black);
            painter->drawPolygon(arrowHead);
        }
        break;
    case PolygonLine:
        painter->drawLine(shapeLine.p1(), shapeLine.p2());
        break;
    }

    if (option->state & QStyle::State_Selected)
    {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::blue, 2, Qt::DashLine));
        painter->drawRect(boundingRect());
    }
}

void CustomShapeItem::setShapeRect(const QRectF &rect)
{
    prepareGeometryChange();
    shapeRect = rect;
    updateHandles();
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
    updateHandles();
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
        QGraphicsItem::mousePressEvent(event);
        // Handle the case where the user is clicking on a handle
//        HandleType handle = handleAt(event->pos());
//        if (handle != TopLeft)
//        {
//            m_currentHandle = handle;
//        }
    }
}

void CustomShapeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
//        if (m_currentHandle != TopLeft)
//        {
//            resizeShape(m_currentHandle, event->scenePos());
//        }
//        else
        {
            QGraphicsItem::mouseMoveEvent(event);
        }
    }
}

void CustomShapeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_currentHandle = TopLeft;
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

void CustomShapeItem::addHandles()
{
//    removeHandles();
//    handles.clear();

    if (shapeType == Rectangle || shapeType == Ellipse)
    {
        // Create ResizeHandle objects instead of QGraphicsEllipseItem
//        for (int i = 0; i < 4; ++i)
//        {
//            ResizeHandle *handle = new ResizeHandle(this);
//            handles.append(handle);
//            handle->setResizeItem(this);
//        }
    }
    else if (shapeType == Line || shapeType == ConvLine || shapeType == ConvReverseLine)
    {
        // Create ResizeHandle objects instead of QGraphicsEllipseItem
//        for (int i = 0; i < 2; ++i)
//        {
//            ResizeHandle *handle = new ResizeHandle(this);
//            handles.append(handle);
//            handle->setResizeItem(this);
//        }
    }
//    updateHandles();
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
        ResizeHandle *handle = handles[i];
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
        else if (shapeType == Line || shapeType == ConvLine || shapeType == ConvReverseLine)
        {
            if (i == 0)
                handle->setPos(shapeLine.p1());
            else if (i == 1)
                handle->setPos(shapeLine.p2());
        }
    }
}

CustomShapeItem::HandleType CustomShapeItem::handleAt(const QPointF &point) const
{
    for (int i = 0; i < handles.size(); ++i)
    {
        if (handles[i]->contains(point))
            return static_cast<HandleType>(i);
    }
    return TopLeft;
}

void CustomShapeItem::resizeShape(HandleType handleType, const QPointF &newPos)
{
    prepareGeometryChange();

    if (shapeType == Rectangle || shapeType == Ellipse)
    {
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
    else if (shapeType == Line || shapeType == ConvLine || shapeType == ConvReverseLine)
    {
        if (handleType == TopLeft)
            shapeLine.setP1(newPos);
        else if (handleType == BottomLeft)
            shapeLine.setP2(newPos);
    }

    updateHandles();
    update();
}
