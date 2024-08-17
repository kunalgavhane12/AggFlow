#include "CustomGraphicsView.h"
#include <QGraphicsProxyWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDataStream>

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
    , scene(new QGraphicsScene(this))
    , currentLine(nullptr)
    , UndoStack(new QUndoStack(this))
{
    setScene(scene);
    setAcceptDrops(true);
    setRenderHints(QPainter::HighQualityAntialiasing);
    setDragMode(QGraphicsView::RubberBandDrag);
    setFixedSizeAndScene(QSize(600, 500));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    scene->setSceneRect(0, 0,600,400);
    setMouseTracking(true);

    acnDel = new QAction(tr("Delete line"), this);
    acnSetVal = new QAction(tr("Set value"), this);

    connect(acnDel, &QAction::triggered, this, &CustomGraphicsView::onActionDelete);
    connect(acnSetVal, &QAction::triggered, this, &CustomGraphicsView::onSetValue);
    connect(this, &CustomGraphicsView::UndoTriggered, UndoStack, &QUndoStack::undo);
    connect(this, &CustomGraphicsView::RedoTriggered, UndoStack, &QUndoStack::redo);
}

void CustomGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        event->acceptProposedAction();
    }
}

void CustomGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        event->acceptProposedAction();
    }
}

void CustomGraphicsView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        QByteArray encodedData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);

        int row, col;
        QMap<int, QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;

        QIcon icon = qvariant_cast<QIcon>(roleDataMap[Qt::UserRole + 1]);
        QPixmap pixmap = icon.pixmap(64, 64);
        CustomPixmapItem* item = new CustomPixmapItem(pixmap);
        item->setPos(mapToScene(event->pos()));
        scene->addItem(item);
        connect(item, &CustomPixmapItem::positionChanged, this, &CustomGraphicsView::updateLinePosition);

        EmitDebugData(event->pos());
        AddItemToAddStack(item);

        event->acceptProposedAction();
    }
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QPointF scenePos = mapToScene(event->pos());
    QGraphicsItem *item = scene->itemAt(scenePos, QTransform());

    if (item && dynamic_cast<ResizableRectItem *>(item))
    {
        qDebug() << "resize";
        currentMode = ResizeMode;
        resizingItem = dynamic_cast<ResizableRectItem *>(item);
        resizingItem->startResizing(scenePos);
        return;
    }

    if (item && dynamic_cast<QGraphicsEllipseItem *>(item))
    {
        handleEllipseInteraction(scenePos, dynamic_cast<QGraphicsEllipseItem *>(item));
    }
    else if (currentMode != ResizeMode)
    {
        startDrawing(scenePos);
    }

    QGraphicsView::mousePressEvent(event);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF endPoint = mapToScene(event->pos());

    if (currentLine)
    {
        QLineF newLine(lineStartPoint, endPoint);
        currentLine->setLine(newLine);
    }
    else if (resizingItem)
    {
        resizingItem->resize(endPoint);
        return;
    }
    else if (currentItem)
    {
        switch (currentMode)
        {
        case ArrowMode:
        case LineMode:
            qgraphicsitem_cast<QGraphicsLineItem *>(currentItem)->setLine(QLineF(startPoint, endPoint));
            break;
        case EllipseMode:
            qgraphicsitem_cast<QGraphicsEllipseItem *>(currentItem)->setRect(QRectF(startPoint, endPoint).normalized());
            break;
        case RectangleMode:
            qgraphicsitem_cast<QGraphicsRectItem *>(currentItem)->setRect(QRectF(startPoint, endPoint).normalized());
            break;
        case PolylineMode:
            break;
        default:
            return;
        }
    }

    update();
    QGraphicsView::mouseMoveEvent(event);
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (currentLine)
    {
        lineConnections[currentLine].first->parentItem()->setFlag(QGraphicsItem::ItemIsMovable, true);
        QPointF scenePos = mapToScene(event->pos());
        QList<QGraphicsItem *> items = scene->items(scenePos);

        bool lineDrawn = false;
        for (auto item : items)
        {
            if (auto ellipseItem = dynamic_cast<QGraphicsEllipseItem *>(item))
            {
                QLineF newLine(lineStartPoint, scenePos);
                currentLine->setLine(newLine);
                lineConnections[currentLine].second = ellipseItem;
                currentLine->SetEndCircle(ellipseItem);
                currentLine->SetEndCircleAttributes();
                lineDrawn = true;
                break;
            }
        }

        if (!lineDrawn || (lineConnections[currentLine].first->parentItem() == lineConnections[currentLine].second->parentItem()))
        {
            scene->removeItem(currentLine);
            lineConnections.remove(currentLine);
            delete currentLine;
        }
        else
        {
            AddItemToAddStack(currentLine);
        }
        currentLine = nullptr;
    }
    else if (currentItem)
    {
        currentMode = None;
        currentItem->setFlag(QGraphicsItem::ItemIsMovable, true);
        currentItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
        currentItem = nullptr;
    }
    else
    {
        QPointF scenePos = mapToScene(event->pos());
        QList<QGraphicsItem *> items = scene->items(scenePos);
        for (QGraphicsItem *itm : items)
        {
            if (auto cpItm = dynamic_cast<CustomPixmapItem *>(itm))
            {
                emit PublishNewData(QString("(%1, %2)").arg(cpItm->pos().x()).arg(cpItm->pos().y()));
                AddItemToMoveStack(cpItm);
                break;
            }
        }
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void CustomGraphicsView::startDrawing(const QPointF &scenePos)
{
    startPoint = scenePos;
    switch (currentMode)
    {
    case ArrowMode:
    case LineMode:
        currentItem = new QGraphicsLineItem(QLineF(startPoint, startPoint));
        break;
    case EllipseMode:
        currentItem = new QGraphicsEllipseItem(QRectF(startPoint, QSizeF(0, 0)));
        break;
    case RectangleMode:
        currentItem = new QGraphicsRectItem(QRectF(startPoint, QSizeF(0, 0)));
        break;
    case PolylineMode:
        break;
    default:
        return;
    }
    if (currentItem)
    {
        scene->addItem(currentItem);
    }
}

void CustomGraphicsView::handleEllipseInteraction(const QPointF &scenePos, QGraphicsEllipseItem *ellipseItem)
{
    QColor ellipseColor = ellipseItem->brush().color();
    if (ellipseColor == Qt::blue || ellipseColor == Qt::red)
    {
        lineStartPoint = scenePos;
        currentLine = new ArrowLineItem(QLineF(lineStartPoint, lineStartPoint));
        scene->addItem(currentLine);
        lineConnections[currentLine].first = ellipseItem;
        currentLine->SetStartCircle(ellipseItem);
        ellipseItem->parentItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
        currentLine->SetStartCircleAttributes();
    }
}

void CustomGraphicsView::handleProxyWidgetInteraction(const QPointF &scenePos, QGraphicsProxyWidget *proxyWidget)
{
    auto selectedItem = scene->selectedItems();
    for (auto item : selectedItem)
    {
        item->setSelected(false);
    }
    proxyWidget->setSelected(true);
    itemStartPosition = proxyWidget->scenePos();
    emit PublishNewData(QString("(%1, %2)").arg(scenePos.x()).arg(scenePos.y()));
}

void CustomGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    contextMenu.clear();
    QList<QGraphicsItem *> lst = items(event->pos());
    for(QGraphicsItem* item:lst)
    {
        CustomPixmapItem *widget = dynamic_cast<CustomPixmapItem *>(item);
        if(widget)
        {
            contextMenu.addAction(acnSetVal);
            selectedItem = widget;
        }
    }
    contextMenu.exec(event->globalPos());
}

void CustomGraphicsView::updateLinePosition()
{
    for (auto it = lineConnections.begin(); it != lineConnections.end(); ++it)
    {
        QGraphicsLineItem *line = it.key();
        QGraphicsEllipseItem *StartCircle = it.value().first;
        QGraphicsEllipseItem *EndCircle = it.value().second;

        if (StartCircle && EndCircle)
        {
            line->setLine(QLineF(StartCircle->scenePos(), EndCircle->scenePos()));
        }
    }
}

void CustomGraphicsView::ClearScene()
{
    RemoveAllLines();
    scene->clear();
    UndoStack->clear();
    emit PublishUndoData(QString());
    emit PublishRedoData(QString());
    emit PublishNewData(QString());
    emit PublishOldData(QString());
}

void CustomGraphicsView::setFixedSizeAndScene(const QSize &size)
{
    setFixedSize(size);
    scene->setSceneRect(0, 0, size.width(), size.height());
}

void CustomGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu.clear();
    ArrowLineItem *line = dynamic_cast<ArrowLineItem *>(itemAt(event->pos()));
    if (line)
    {
        // Add actions to the context menu
        contextMenu.addAction(acnDel);
        selectedItem = line;
        // Show the context menu at the cursor position
    }
    contextMenu.exec(event->globalPos());
}

void CustomGraphicsView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(AnchorUnderMouse);
    double scalefactor = 1.5;

    if(event->modifiers() & Qt::ControlModifier)
    {
        if(event->delta() > 0)
        {
            scale(scalefactor,scalefactor);
        }
        else
        {
            scale(1/scalefactor,1/scalefactor);
        }
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}

void CustomGraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    scene->setSceneRect(0, 0, event->size().width(), event->size().height());
}

//remove lines and break connections . Remember to delete pointers
void CustomGraphicsView::RemoveLines()
{
    ArrowLineItem * arrowLine = dynamic_cast<ArrowLineItem *>(selectedItem);
    lineConnections.remove(arrowLine);
}

//remove lines and break connections . Remember to delete pointers
void CustomGraphicsView::RemoveAllLines()
{
    for (auto it = lineConnections.begin(); it != lineConnections.end(); ++it)
    {
        delete it.key();
    }

    lineConnections.clear();
}

void CustomGraphicsView::onActionDelete()
{
    if (selectedItem)
    {
        scene->removeItem(selectedItem);
        RemoveLines();
        delete selectedItem;
        selectedItem = nullptr;
    }
}

void CustomGraphicsView::onSetValue()
{
    CustomPixmapItem* item = dynamic_cast<CustomPixmapItem *>(selectedItem);
    if(item)
    {
        double value = QInputDialog::getDouble(this, "Enter Value:", "Operation:", 0, 0, 1000, 2, nullptr);
        item->SetText(QString::number(value));
    }
}

void CustomGraphicsView::onResult()
{
    double result = 0.0;
    QSet<CustomPixmapItem*> visitItems;

    for (auto it = lineConnections.begin(); it != lineConnections.end(); ++it)
    {
        QGraphicsEllipseItem *startEllipse = it.value().first;
        QGraphicsEllipseItem *endEllipse = it.value().second;

        if (startEllipse && endEllipse)
        {
            CustomPixmapItem *startItem = dynamic_cast<CustomPixmapItem *>(startEllipse->parentItem());
            CustomPixmapItem *endItem = dynamic_cast<CustomPixmapItem *>(endEllipse->parentItem());

            int endId = endItem->GetItemId();
            int n;

            if(endId > 4){
                n = endId % 4;
            }else{
                n = endId;
            }

            switch(n){
            case 1 :
                result += startItem->GetText().toDouble() +  endItem->GetText().toDouble();
                visitItems.insert(startItem);
                qDebug()<<"case 1 :"<<result;
                break;
            case 2 :
                result += startItem->GetText().toDouble() * endItem->GetText().toDouble();
                visitItems.insert(endItem);
                qDebug()<<"case 1 :"<<result;
                break;
            case 3 :
                result += startItem->GetText().toDouble() / endItem->GetText().toDouble();
                visitItems.insert(endItem);
                qDebug()<<"case 1 :"<<result;
                break;
            default:
                result += startItem->GetText().toDouble() - endItem->GetText().toDouble();
                qDebug()<<"case 1 :"<<result;
                break;
            }
        }
    }

    emit resultUpdated(QString::number(result));
}

void CustomGraphicsView::saveToFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Could not open file for writing");
        return;
    }

    QDataStream out(&file);
    // Save all CustomPixmapItems
    QList<QGraphicsItem *> items = scene->items();
    for (QGraphicsItem *item : items) {
        if (CustomPixmapItem *pixmapItem = dynamic_cast<CustomPixmapItem *>(item)) {
            out << QString("CustomPixmapItem");
            pixmapItem->write(out);
        } else if (ArrowLineItem *lineItem = dynamic_cast<ArrowLineItem *>(item)) {
            out << QString("ArrowLineItem");
            lineItem->write(out);
        }
    }

    QMessageBox msgBox;
    msgBox.setText("Data Saved Succesfully!!!");
    msgBox.exec();
}

void CustomGraphicsView::loadFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Could not open file for reading");
        return;
    }

    QDataStream in(&file);
    scene->clear();
    lineConnections.clear();

    QList<ArrowLineItem*> lineItems;
    QMap<int, CustomPixmapItem*> customItems;
    while (!in.atEnd()) {
        QString itemType;
        in >> itemType;

        if (itemType == "CustomPixmapItem") {
            CustomPixmapItem *pixmapItem = new CustomPixmapItem(QPixmap());
            pixmapItem->read(in);
            pixmapItem->HideLabelIfNeeded();
            scene->addItem(pixmapItem);
            customItems.insert(pixmapItem->GetItemId(), pixmapItem);
            connect(pixmapItem, &CustomPixmapItem::positionChanged, this, &CustomGraphicsView::updateLinePosition);
        } else if (itemType == "ArrowLineItem") {
            ArrowLineItem *lineItem = new ArrowLineItem(QLineF());
            lineItem->read(in);
            scene->addItem(lineItem);
            lineItems.append(lineItem);
        }
    }

    reconnectLines(lineItems, customItems);
}

void CustomGraphicsView::saveToXml(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Scene");

    // Serialize CustomPixmapItems
    for (QGraphicsItem* item : scene->items())
    {
        if (CustomPixmapItem* pixmapItem = dynamic_cast<CustomPixmapItem*>(item))
        {
            pixmapItem->saveToXml(xmlWriter);
        }
    }

    // Serialize ArrowLineItems
    for (QGraphicsItem* item : scene->items())
    {
        if (ArrowLineItem* arrowItem = dynamic_cast<ArrowLineItem*>(item))
        {
            arrowItem->saveToXml(xmlWriter);
        }
    }

    xmlWriter.writeEndElement(); // Scene
    xmlWriter.writeEndDocument();

    QMessageBox msgBox;
    msgBox.setText("Data in Xml Saved Successfully!");
    msgBox.exec();
}

void CustomGraphicsView::loadFromXml(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open file for reading");
        return;
    }

    QXmlStreamReader xmlReader(&file);
    scene->clear();
    lineConnections.clear();

    QList<ArrowLineItem*> lineItems;
    QMap<int, CustomPixmapItem*> customItems;

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        if (token == QXmlStreamReader::StartElement) {
            if (xmlReader.name() == "CustomPixmapItem") {
                CustomPixmapItem *pixmapItem = new CustomPixmapItem(QPixmap());
                pixmapItem->loadFromXml(xmlReader);
                pixmapItem->HideLabelIfNeeded();
                scene->addItem(pixmapItem);
                customItems.insert(pixmapItem->GetItemId(), pixmapItem);
                connect(pixmapItem, &CustomPixmapItem::positionChanged, this, &CustomGraphicsView::updateLinePosition);
            } else if (xmlReader.name() == "ArrowLineItem") {
                ArrowLineItem *lineItem = new ArrowLineItem(QLineF());
                lineItem->loadFromXml(xmlReader);
                scene->addItem(lineItem);
                lineItems.append(lineItem);
            }
        }
    }

    if (xmlReader.hasError()) {
        qWarning("XML error: %s", xmlReader.errorString().toStdString().c_str());
    }

    xmlReader.clear();
    reconnectLines(lineItems, customItems);
}

void CustomGraphicsView::reconnectLines(QList<ArrowLineItem*> lineItems, QMap<int, CustomPixmapItem*> customItems)
{
    for (ArrowLineItem* line : lineItems) {
        CustomPixmapItem* startItem = customItems.value(line->GetStartCircleItemId(), nullptr);
        CustomPixmapItem* endItem = customItems.value(line->GetEndCircleItemId(), nullptr);

        if (startItem && endItem)
        {
            if(line->GetIsStartCircleStartConnected())
            {
                line->SetStartCircle(customItems[line->GetStartCircleItemId()]->GetStartCircle());
            }
            if(line->GetIsStartCircleEndConnected())
            {
                line->SetStartCircle(customItems[line->GetStartCircleItemId()]->GetEndCircle());
            }
            if(line->GetIsEndCircleStartConnected())
            {
                line->SetEndCircle(customItems[line->GetEndCircleItemId()]->GetStartCircle());
            }
            else if(line->GetIsEndCircleEndConnected())
            {
                line->SetEndCircle(customItems[line->GetEndCircleItemId()]->GetEndCircle());
            }

            if (line->GetStartCircle() && line->GetEndCircle()) {
                lineConnections[line].first = line->GetStartCircle();
                lineConnections[line].second = line->GetEndCircle();
            }
        }
    }

    updateLinePosition();
}

void CustomGraphicsView::EmitDebugData(QPoint pos)
{
    emit PublishUndoData(QString());
    emit PublishRedoData(QString());
    emit PublishNewData(QString());
    emit PublishOldData(QString());
    emit PublishOldData(QString("(%1, %2)").arg(mapToScene(pos).x()).arg(mapToScene(pos).y()));
}

void CustomGraphicsView::AddItemToAddStack(QGraphicsItem* item)
{
    AddCommand* command = new AddCommand(scene, item);
    connect(command, &AddCommand::PublishUndoData, this, &CustomGraphicsView::PublishUndoData);
    connect(command, &AddCommand::PublishRedoData, this, &CustomGraphicsView::PublishRedoData);
    connect(command, &AddCommand::NotifyUndoCompleted, this, &CustomGraphicsView::updateLinePosition);
    connect(command, &AddCommand::NotifyRedoCompleted, this, &CustomGraphicsView::updateLinePosition);
    UndoStack->push(command);
}

void CustomGraphicsView::AddItemToMoveStack(QGraphicsItem* item)
{
    MoveCommand* command = new MoveCommand(item, itemStartPosition, item->scenePos());
    connect(command, &MoveCommand::PublishUndoData, this, &CustomGraphicsView::PublishUndoData);
    connect(command, &MoveCommand::PublishRedoData, this, &CustomGraphicsView::PublishRedoData);
    connect(command, &MoveCommand::NotifyUndoCompleted, this, &CustomGraphicsView::updateLinePosition);
    connect(command, &MoveCommand::NotifyRedoCompleted, this, &CustomGraphicsView::updateLinePosition);
    UndoStack->push(command);
}
