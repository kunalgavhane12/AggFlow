#ifndef ARROWLINEITEM_H
#define ARROWLINEITEM_H

#include <QGraphicsLineItem>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <cmath>

class ArrowLineItem : public QGraphicsLineItem
{
public:
    ArrowLineItem(QLineF line, QGraphicsItem* parent = nullptr);
    int lineWidth;
    void write(QDataStream &out) const;
    void read(QDataStream &in);
    void saveToXml(QXmlStreamWriter &xmlWriter) const;
    void loadFromXml(QXmlStreamReader &xmlReader);
    void SetStartCircle(QGraphicsEllipseItem* circle);
    void SetEndCircle(QGraphicsEllipseItem* circle);
    QGraphicsEllipseItem* GetStartCircle();
    QGraphicsEllipseItem* GetEndCircle();
    void SetStartCircleAttributes();
    void SetEndCircleAttributes();
    int GetStartCircleItemId() const;
    int GetEndCircleItemId() const;
    bool GetIsStartCircleStartConnected() const;
    bool GetIsStartCircleEndConnected() const;
    bool GetIsEndCircleStartConnected() const;
    bool GetIsEndCircleEndConnected() const;

private:
    QGraphicsEllipseItem* StartCircle;
    QGraphicsEllipseItem* EndCircle;

    int StartCircleItemId;
    bool IsStartCircleStartConnected;
    bool IsStartCircleEndConnected;
    int EndCircleItemId;
    bool IsEndCircleStartConnected;
    bool IsEndCircleEndConnected;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
};

#endif // ARROWLINEITEM_H
