#ifndef CUSTOMPIXMAPITEM_H
#define CUSTOMPIXMAPITEM_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QLabel>
#include <QXmlStreamWriter>

class CustomPixmapItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    static int GlobalItemId;
    CustomPixmapItem(const QPixmap &pixmap);
    void SetText(const QString &text);
    QString GetText() const;
    void write(QDataStream &out) const;
    void read(QDataStream &in);
    void saveToXml(QXmlStreamWriter &xmlWriter) const;
    void loadFromXml(QXmlStreamReader &xmlReader);
    void SetStartConnected(bool connected);
    void SetEndConnected(bool connected);
    bool GetStartConnected();
    bool GetEndConnected();
    void SetItemId(int itemId);
    int GetItemId();
    void HideLabelIfNeeded();

    int pixmapWidth() const { return PixmapLabel->pixmap()->width(); }
    int pixmapHeight() const { return PixmapLabel->pixmap()->height(); }

    QGraphicsEllipseItem *GetStartCircle() const;
    QGraphicsEllipseItem *GetEndCircle() const;

signals:
    void positionChanged();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    void AddEndCircles();

    QPointF DragStartPosition;
    bool IsDraggingInProgress;
    QWidget* ContainerWidget;
    QLabel* TextLabel;
    QLabel* PixmapLabel;
    QGraphicsProxyWidget* ProxyWid;
    QGraphicsEllipseItem *StartCircle;
    QGraphicsEllipseItem *EndCircle;
    int ItemId;
    bool IsStartConnected;
    bool IsEndConnected;
};

#endif // CUSTOMPIXMAPITEM_H
