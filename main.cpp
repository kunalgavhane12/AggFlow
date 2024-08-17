#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

//void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
//{
//    QPointF scenePos = mapToScene(event->pos());
//    QGraphicsItem *item = scene->itemAt(scenePos, QTransform());

//    if (item)
//    {
//        if(dynamic_cast<QGraphicsRectItem *>(item))
//        {
//            qDebug()<<"selectRect";
//        }
//        else if(dynamic_cast<QGraphicsEllipseItem *>(item))
//        {
//            qDebug() << "selectEllipse";

//            QGraphicsEllipseItem* ellipseItem = dynamic_cast<QGraphicsEllipseItem *>(item);

//            if (ellipseItem == currentItem)
//            {
//                qDebug() << "selectEllipseDraw";
//            }
//            else
//            {
//                QColor ellipseColor = ellipseItem->brush().color(); // Get the color of the ellipse

//                if (ellipseColor == Qt::blue || ellipseColor == Qt::red)
//                {
//                    qDebug() << "selectRedBlue";
//                    qDebug()<<dynamic_cast<QGraphicsEllipseItem *>(item);
//                    lineStartPoint = scenePos;
//                    currentLine = new ArrowLineItem(QLineF(lineStartPoint, lineStartPoint));
//                    scene->addItem(currentLine);
//                    lineConnections[currentLine].first = dynamic_cast<QGraphicsEllipseItem *>(item);
//                    currentLine->SetStartCircle(dynamic_cast<QGraphicsEllipseItem *>(item));
//                    item->parentItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
//                    currentLine->SetStartCircleAttributes();
//                }
//                else
//                {
//                    qDebug() << "Other ellipse item";
//                }
//            }
//        }
//    }

//    if (item && dynamic_cast<QGraphicsProxyWidget *>(item))
//    {
//        qDebug()<<"select2";
//        QGraphicsScene* scene = item->scene();
//        if (scene)
//        {
//            foreach(QGraphicsItem* selectedItem, scene->selectedItems())
//            {
//                selectedItem->setSelected(false);
//            }
//        }

//        item->setSelected(true);

//        itemStartPosition = dynamic_cast<QGraphicsProxyWidget *>(item)->scenePos();
//        emit PublishNewData(QString("(%1, %2)").arg(scenePos.x()).arg(scenePos.y()));
//    }

//    startPoint = scenePos;
//    switch (currentMode)
//    {
//    case ArrowMode:
//        currentItem = new QGraphicsLineItem(QLineF(startPoint, startPoint));
//        break;
//    case LineMode:
//        currentItem = new QGraphicsLineItem(QLineF(startPoint, startPoint));
//        break;
//    case PolylineMode:
//        break;
//    case EllipseMode:
//        currentItem = new QGraphicsEllipseItem(QRectF(startPoint, QSizeF(0, 0)));
//        break;
//    case RectangleMode:
//        currentItem = new QGraphicsRectItem(QRectF(startPoint, QSizeF(0, 0)));
//        break;
//    default:
//        QGraphicsView::mousePressEvent(event);
//        return;
//    }
//    if (currentItem)
//    {
//        scene->addItem(currentItem);
//    }

//    QGraphicsView::mousePressEvent(event);
//}

//void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
//{
//    QPointF endPoint = mapToScene(event->pos());

//    if (currentLine)
//    {
//        QLineF newLine(lineStartPoint, mapToScene(event->pos()));
//        currentLine->setLine(newLine);
//    }

//    switch (currentMode) {
//    case ArrowMode:
//    case LineMode: {
//        QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem *>(currentItem);
//        if (line) {
//            line->setLine(QLineF(startPoint, endPoint));
//        }
//        break;
//    }
//    case EllipseMode: {
//        QGraphicsEllipseItem *ellipse = qgraphicsitem_cast<QGraphicsEllipseItem *>(currentItem);
//        if (ellipse) {
//            QRectF rect(startPoint, endPoint);
//            ellipse->setRect(rect.normalized());
//        }
//        break;
//    }
//    case RectangleMode: {
//        QGraphicsRectItem *rect = qgraphicsitem_cast<QGraphicsRectItem *>(currentItem);
//        if (rect) {
//            QRectF rectF(startPoint, endPoint);
//            rect->setRect(rectF.normalized());
//        }
//        break;
//    }
//    case PolylineMode:
//        break;
//    default:
//        QGraphicsView::mouseMoveEvent(event);
//        break;
//    }

//    update();
//    QGraphicsView::mouseMoveEvent(event);
//}

//void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
//{
//    if (currentLine)
//    {
//        lineConnections[currentLine].first->parentItem()->setFlag(QGraphicsItem::ItemIsMovable, true);

//        QPointF scenePos = mapToScene(event->pos());
//        QList<QGraphicsItem *>items = scene->items(scenePos);

//        bool lineDrawn = false;
//        for(auto item:items)
//        {
//            auto test = dynamic_cast<QGraphicsEllipseItem *>(item);
//            if (item && test)
//            {
//                QLineF newLine(lineStartPoint, scenePos);
//                currentLine->setLine(newLine);
//                lineConnections[currentLine].second = dynamic_cast<QGraphicsEllipseItem *>(item);
//                currentLine->SetEndCircle(dynamic_cast<QGraphicsEllipseItem *>(item));
//                currentLine->SetEndCircleAttributes();
//                lineDrawn = true;
//                break;
//            }
//        }

//        if(!lineDrawn || (lineConnections[currentLine].first->parentItem() == lineConnections[currentLine].second->parentItem()))
//        {
//            scene->removeItem(currentLine);
//            lineConnections.remove(currentLine);
//            delete currentLine;
//        }
//        else
//        {
//            AddItemToAddStack(currentLine);
//        }

//        currentLine = nullptr;
//    }
//    else if (currentItem)
//    {
//        currentMode = None;
//        currentItem->setFlag(QGraphicsItem::ItemIsMovable, true);
//        currentItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
//        currentItem = nullptr;
//    }
//    else
//    {
//        QPointF scenePos = mapToScene(event->pos());
//        QList<QGraphicsItem *>items = scene->items(scenePos);
//        for(QGraphicsItem *itm:items)
//        {
//            CustomPixmapItem *cpItm = dynamic_cast<CustomPixmapItem *>(itm);
//            if(cpItm)
//            {
//                emit PublishNewData(QString("(%1, %2)").arg(cpItm->pos().x()).arg(cpItm->pos().y()));
//                AddItemToMoveStack(cpItm);
//                break;
//            }
//        }
//    }

//    QGraphicsView::mouseReleaseEvent(event);
//}
