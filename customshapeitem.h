#ifndef CUSTOMSHAPEITEM_H
#define CUSTOMSHAPEITEM_H

#include <QGraphicsItem>
#include <QPen>
#include <QBrush>

class CustomShapeItem : public QGraphicsItem
{
public:
    enum ShapeType { Rectangle, Ellipse, Line, Arrow, FixedLine };

    CustomShapeItem(ShapeType shapeType, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setShapeRect(const QRectF &rect);
    QRectF getShapeRect() const;
    QLineF getShapeLine() const;
    void setShapeLine(const QLineF &line);
    ShapeType getShapeType() const;

signals:


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
private:
    ShapeType shapeType;
    QRectF shapeRect;
    QLineF shapeLine;

    enum HandleType { TopLeft, TopRight, BottomLeft, BottomRight, MiddleLeft, MiddleRight, TopCenter, BottomCenter };
    QVector<QGraphicsEllipseItem*> handles;

    void updateHandles();
    void addHandles();
    void removeHandles();
    HandleType handleAt(const QPointF &point) const;
    void resizeShape(HandleType handleType, const QPointF &newPos);
};

#endif // CUSTOMSHAPEITEM_H

