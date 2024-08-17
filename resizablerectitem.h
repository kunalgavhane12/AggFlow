#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class ResizableRectItem : public QGraphicsRectItem {
public:

    ResizableRectItem(const QRectF &rect, QGraphicsItem *parent);

    void startResizing(const QPointF &scenePos);

    void resize(const QPointF &scenePos);

    void endResizing();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override {
        if (isResizing) {
            resize(event->scenePos());
        }
        QGraphicsRectItem::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override {
        if (isResizing) {
            endResizing();
        }
        QGraphicsRectItem::mouseReleaseEvent(event);
    }

private:
    bool isResizing;
    QPointF initialPos;
    QRectF originalRect;
    enum ResizeHandle { NoHandle, TopLeft, TopRight, BottomLeft, BottomRight } resizeHandle;

    QRectF getResizeHandleRect(ResizeHandle handle) const;

    ResizeHandle getResizeHandle(const QPointF &pos) const;
};
