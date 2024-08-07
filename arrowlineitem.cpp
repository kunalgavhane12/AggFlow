#include "ArrowLineItem.h"
#include <custompixmapitem.h>
#include <QColor>

ArrowLineItem::ArrowLineItem(QLineF line, QGraphicsItem* parent)
    : QGraphicsLineItem(line, parent)
    , lineWidth(2)
{
    QPen pen(Qt::black, lineWidth, Qt::DotLine); // Set pen to dotted line
    setPen(pen);
}

void ArrowLineItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QGraphicsLineItem::paint(painter, option, widget);

    QLineF line = this->line();
    double angle = std::atan2(-line.dy(), line.dx());

    // Define the arrowhead points
    QPointF arrowP1 = line.p2() - QPointF(sin(angle + M_PI / 3) * 10, cos(angle + M_PI / 3) * 10);
    QPointF arrowP2 = line.p2() - QPointF(sin(angle + M_PI - M_PI / 3) * 10, cos(angle + M_PI - M_PI / 3) * 10);

    // Draw the arrowhead with a solid pen
    // comment below 2 lines for solid arrowhead
    // 3rd line is for colour of arrowhead
    //    QPen arrowPen(Qt::black, lineWidth, Qt::SolidLine);
    //    painter->setPen(arrowPen);
    //    painter->setBrush(Qt::green); // Fill color for the arrowhead

    QPolygonF arrowHead;
    arrowHead << line.p2() << arrowP1 << arrowP2;
    painter->drawPolygon(arrowHead);
}

void ArrowLineItem::write(QDataStream &out) const {
    out << line();
    out << dynamic_cast<CustomPixmapItem *>(StartCircle->parentItem())->GetItemId();
    out << dynamic_cast<CustomPixmapItem *>(StartCircle->parentItem())->GetStartConnected();
    out << dynamic_cast<CustomPixmapItem *>(StartCircle->parentItem())->GetEndConnected();

    out << dynamic_cast<CustomPixmapItem *>(EndCircle->parentItem())->GetItemId();
    out << dynamic_cast<CustomPixmapItem *>(EndCircle->parentItem())->GetStartConnected();
    out << dynamic_cast<CustomPixmapItem *>(EndCircle->parentItem())->GetEndConnected();
}

void ArrowLineItem::read(QDataStream &in) {
    QLineF line;
    in >> line;
    setLine(line);

    int itemIdStart, itemIdEnd;
    bool startCircleStartItem, EndCircleStartItem, startCircleEndItem, EndCircleEndItem;
    in >> itemIdStart >> startCircleStartItem >> EndCircleStartItem >> itemIdEnd >> startCircleEndItem >> EndCircleEndItem;

    StartCircleItemId = itemIdStart;
    IsStartCircleStartConnected = startCircleStartItem;
    IsStartCircleEndConnected = EndCircleStartItem;
    EndCircleItemId = itemIdEnd;
    IsEndCircleStartConnected = startCircleEndItem;
    IsEndCircleEndConnected = EndCircleEndItem;
}

void ArrowLineItem::saveToXml(QXmlStreamWriter &xmlWriter) const
{
    xmlWriter.writeStartElement("ArrowLineItem");

    // Write the line
    QLineF line = this->line();
    xmlWriter.writeTextElement("line", QString("%1,%2,%3,%4").arg(line.x1()).arg(line.y1()).arg(line.x2()).arg(line.y2()));

    // Write the start and end items and their connection states
    CustomPixmapItem* startItem = dynamic_cast<CustomPixmapItem*>(StartCircle->parentItem());
    CustomPixmapItem* endItem = dynamic_cast<CustomPixmapItem*>(EndCircle->parentItem());

    if (startItem && endItem) {
        xmlWriter.writeTextElement("StartItemId", QString::number(startItem->GetItemId()));
        xmlWriter.writeTextElement("StartCircleStartConnected", QString::number(startItem->GetStartConnected()));
        xmlWriter.writeTextElement("StartCircleEndConnected", QString::number(startItem->GetEndConnected()));

        xmlWriter.writeTextElement("EndItemId", QString::number(endItem->GetItemId()));
        xmlWriter.writeTextElement("EndCircleStartConnected", QString::number(endItem->GetStartConnected()));
        xmlWriter.writeTextElement("EndCircleEndConnected", QString::number(endItem->GetEndConnected()));
    }

    xmlWriter.writeEndElement(); // ArrowLineItem
}

void ArrowLineItem::loadFromXml(QXmlStreamReader &xmlReader)
{
    QLineF line;
        int startItemId = 0, endItemId = 0;
        bool startCircleStartConnected = false, startCircleEndConnected = false;
        bool endCircleStartConnected = false, endCircleEndConnected = false;

        while (!xmlReader.atEnd() && !xmlReader.hasError())
        {
            QXmlStreamReader::TokenType token = xmlReader.readNext();
            if (token == QXmlStreamReader::StartElement)
            {
                if (xmlReader.name() == "line")
                {
                    QStringList points = xmlReader.readElementText().split(",");
                    if (points.size() == 4)
                    {
                        line.setP1(QPointF(points[0].toDouble(), points[1].toDouble()));
                        line.setP2(QPointF(points[2].toDouble(), points[3].toDouble()));
                    }
                } else if (xmlReader.name() == "StartItemId") {
                    startItemId = xmlReader.readElementText().toInt();
                } else if (xmlReader.name() == "EndItemId") {
                    endItemId = xmlReader.readElementText().toInt();
                } else if (xmlReader.name() == "StartCircleStartConnected") {
                    startCircleStartConnected = xmlReader.readElementText().toInt();
                } else if (xmlReader.name() == "StartCircleEndConnected") {
                    startCircleEndConnected = xmlReader.readElementText().toInt();
                } else if (xmlReader.name() == "EndCircleStartConnected") {
                    endCircleStartConnected = xmlReader.readElementText().toInt();
                } else if (xmlReader.name() == "EndCircleEndConnected") {
                    endCircleEndConnected = xmlReader.readElementText().toInt();
                }
            } else if (token == QXmlStreamReader::EndElement && xmlReader.name() == "ArrowLineItem") {
                break;
            }
        }

        setLine(line);
        StartCircleItemId = startItemId;
        IsStartCircleStartConnected = startCircleStartConnected;
        IsStartCircleEndConnected = startCircleEndConnected;
        EndCircleItemId = endItemId;
        IsEndCircleStartConnected = endCircleStartConnected;
        IsEndCircleEndConnected = endCircleEndConnected;
}

void ArrowLineItem::SetStartCircle(QGraphicsEllipseItem *circle)
{
    StartCircle = new QGraphicsEllipseItem;
    StartCircle = circle;
}

void ArrowLineItem::SetEndCircle(QGraphicsEllipseItem *circle)
{
    EndCircle = new QGraphicsEllipseItem;
    EndCircle = circle;
}

QGraphicsEllipseItem *ArrowLineItem::GetStartCircle()
{
    return StartCircle;
}

QGraphicsEllipseItem *ArrowLineItem::GetEndCircle()
{
    return EndCircle;
}

void ArrowLineItem::SetStartCircleAttributes()
{
    CustomPixmapItem *parentItem = dynamic_cast<CustomPixmapItem *>(StartCircle->parentItem());
    QColor clr = StartCircle->brush().color();
    clr == Qt::red ? parentItem->SetStartConnected(true) : parentItem->SetEndConnected(true);
}

void ArrowLineItem::SetEndCircleAttributes()
{
    CustomPixmapItem *parentItem = dynamic_cast<CustomPixmapItem *>(EndCircle->parentItem());
    QColor clr = EndCircle->brush().color();
    clr == Qt::red ? parentItem->SetStartConnected(true) : parentItem->SetEndConnected(true);
}

int ArrowLineItem::GetStartCircleItemId() const
{
    return StartCircleItemId;
}

int ArrowLineItem::GetEndCircleItemId() const
{
    return EndCircleItemId;
}

bool ArrowLineItem::GetIsStartCircleStartConnected() const
{
    return IsStartCircleStartConnected;
}

bool ArrowLineItem::GetIsStartCircleEndConnected() const
{
    return IsStartCircleEndConnected;
}

bool ArrowLineItem::GetIsEndCircleStartConnected() const
{
    return IsEndCircleStartConnected;
}

bool ArrowLineItem::GetIsEndCircleEndConnected() const
{
    return IsEndCircleEndConnected;
}
