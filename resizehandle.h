#ifndef RESIZEHANDLE_H
#define RESIZEHANDLE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>

class ResizeHandle : public QGraphicsEllipseItem
{
public:
    explicit ResizeHandle(QGraphicsItem *parent = nullptr);

    void setResizeItem(QGraphicsItem *item);
    QGraphicsItem* resizeItem() const;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QGraphicsItem *m_resizeItem;
    bool m_isResizingStart;
};

#endif // RESIZEHANDLE_H
