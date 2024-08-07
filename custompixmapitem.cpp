#include "CustomPixmapItem.h"
#include <QGraphicsScene>
#include <QPen>
#include <QWidget>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QVBoxLayout>
#include <QBuffer>

namespace
{
const char* DEFAULT_TEXT = "Text";
}

int CustomPixmapItem::GlobalItemId = 0;

CustomPixmapItem::CustomPixmapItem(const QPixmap &pixmap)
    : IsDraggingInProgress(false)
    , ContainerWidget(new QWidget)
    , TextLabel(new QLabel(DEFAULT_TEXT))
    , PixmapLabel(new QLabel)
    , ProxyWid(new QGraphicsProxyWidget)
    , StartCircle (new QGraphicsEllipseItem(-10, -10, 10, 10, this))
    , EndCircle (new QGraphicsEllipseItem(-10, -10, 10, 10, this))
    , ItemId(0)
    , IsStartConnected(false)
    , IsEndConnected(false)
{
    ItemId = ++GlobalItemId;
    setFlag(ItemIsMovable);
    //    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);

    PixmapLabel->setPixmap(pixmap);
    AddEndCircles();
}

void CustomPixmapItem::AddEndCircles()
{
    //  this way also we can add widget also depending on future requirement might need it so keeping commented code
    //    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
    //    pixmapItem->setPos(0, 0); // Position the image in the scene

    // Create a QGraphicsTextItem for displaying text
    //    textItem->setDefaultTextColor(Qt::black); // Set text color
    //    textItem->setFont(QFont("Arial", 16));    // Set text font and size
    //    textItem->setPos(0, 55); // Position the text in the scene

    StartCircle->setBrush(Qt::red);
    EndCircle->setBrush(Qt::blue);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(TextLabel);
    layout->addWidget(PixmapLabel);
    ContainerWidget->setLayout(layout);
    ContainerWidget->setFixedSize(100,100);
    ContainerWidget->setAttribute(Qt::WA_TranslucentBackground);
    ProxyWid->setWidget(ContainerWidget);
    TextLabel->setFont(QFont("Arial", 16));
    TextLabel->hide();
    TextLabel->setObjectName("LABEL");

    addToGroup(ProxyWid);
    addToGroup(StartCircle);
    addToGroup(EndCircle);

    // Update circle positions relative to the group
    QRectF bdRect = boundingRect();
    StartCircle->setPos(-EndCircle->boundingRect().width(), bdRect.height() / 2);
    EndCircle->setPos(bdRect.width(), bdRect.height() / 2);

    //    the circles opacity and colour i will adjust later if needed
    //    EndCircle->setOpacity(0.5);
}

QGraphicsEllipseItem *CustomPixmapItem::GetEndCircle() const
{
    return EndCircle;
}

QGraphicsEllipseItem *CustomPixmapItem::GetStartCircle() const
{
    return StartCircle;
}

void CustomPixmapItem::SetText(const QString &text)
{
    TextLabel->setText(text);
    TextLabel->show();
}

QString CustomPixmapItem::GetText() const
{
    return TextLabel->text();
}

void CustomPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        DragStartPosition = event->pos();
        IsDraggingInProgress = true;
    }
    QGraphicsItemGroup::mousePressEvent(event);
}

void CustomPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (IsDraggingInProgress)
    {
        QPointF newPos = pos() + event->pos() - DragStartPosition;
        setPos(newPos);
        emit positionChanged();
    }
    update();
    QGraphicsItemGroup::mouseMoveEvent(event);
}

void CustomPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        IsDraggingInProgress = false;
    }
    QGraphicsItemGroup::mouseReleaseEvent(event);
}

QVariant CustomPixmapItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if ((change == ItemPositionChange || change == ItemSelectedChange
         || change == ItemPositionHasChanged) && scene())
    {
        emit positionChanged();
    }
    return QGraphicsItem::itemChange(change, value);
}

void CustomPixmapItem::write(QDataStream &out) const {
    out << pos();
    out << PixmapLabel->pixmap()->toImage();
    out << TextLabel->text();
    out << CustomPixmapItem::GlobalItemId;
    out << ItemId;
    out << IsStartConnected;
    out << IsEndConnected;
}

void CustomPixmapItem::read(QDataStream &in) {
    QPointF position;
    QImage image;
    QString text;
    int globalItemId;
    int itemId;
    bool isStartConn;
    bool isEndConn;

    in >> position >> image >> text >> globalItemId >> itemId >> isStartConn >> isEndConn;

    setPos(position);
    PixmapLabel->setPixmap(QPixmap::fromImage(image));
    SetText(text);
    ItemId = itemId;
    GlobalItemId = GlobalItemId > globalItemId ? GlobalItemId : globalItemId;
    ItemId = itemId;
    SetStartConnected(isStartConn);
    SetEndConnected(isEndConn);
}

void CustomPixmapItem::saveToXml(QXmlStreamWriter &xmlWriter) const
{
    xmlWriter.writeStartElement("CustomPixmapItem");

    // Write position
    QPointF position = pos();
    xmlWriter.writeTextElement("Position", QString("%1,%2").arg(position.x()).arg(position.y()));

    // Write pixmap
    QImage image = PixmapLabel->pixmap()->toImage();
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    xmlWriter.writeTextElement("Pixmap", imageData.toBase64());

    // Write text
    QString text = TextLabel->text();
    xmlWriter.writeTextElement("Text", text);

    // Write IDs and connection states
    xmlWriter.writeTextElement("GlobalItemId", QString::number(GlobalItemId));
    xmlWriter.writeTextElement("ItemId", QString::number(ItemId));
    xmlWriter.writeTextElement("IsStartConnected", QString::number(IsStartConnected));
    xmlWriter.writeTextElement("IsEndConnected", QString::number(IsEndConnected));

    xmlWriter.writeEndElement(); // CustomPixmapItem
}

void CustomPixmapItem::loadFromXml(QXmlStreamReader &xmlReader)
{
    QPointF position;
    QImage image;
    QString text;
    int globalItemId=0;
    int itemId=0;
    bool isStartConn=false;
    bool isEndConn=false;

    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement)
        {
            if (xmlReader.name() == "Position")
            {
                QStringList coords = xmlReader.readElementText().split(",");
                if (coords.size() == 2)
                {
                    position.setX(coords[0].toDouble());
                    position.setY(coords[1].toDouble());
                }
            } else if (xmlReader.name() == "Pixmap") {
                QByteArray imageData = QByteArray::fromBase64(xmlReader.readElementText().toUtf8());
                image.loadFromData(imageData, "PNG");
            } else if (xmlReader.name() == "Text") {
                text = xmlReader.readElementText();
            } else if (xmlReader.name() == "GlobalItemId") {
                globalItemId = xmlReader.readElementText().toInt();
            } else if (xmlReader.name() == "ItemId") {
                itemId = xmlReader.readElementText().toInt();
            } else if (xmlReader.name() == "IsStartConnected") {
                isStartConn = xmlReader.readElementText().toInt();
            } else if (xmlReader.name() == "IsEndConnected") {
                isEndConn = xmlReader.readElementText().toInt();
            }
        } else if (token == QXmlStreamReader::EndElement && xmlReader.name() == "CustomPixmapItem") {
            break;
        }
    }
    setPos(position);
    PixmapLabel->setPixmap(QPixmap::fromImage(image));
    SetText(text);
    ItemId = itemId;
    GlobalItemId = GlobalItemId > globalItemId ? GlobalItemId : globalItemId;
    SetStartConnected(isStartConn);
    SetEndConnected(isEndConn);
}

void CustomPixmapItem::SetStartConnected(bool connected)
{
    IsStartConnected = connected;
}

void CustomPixmapItem::SetEndConnected(bool connected)
{
    IsEndConnected = connected;
}

bool CustomPixmapItem::GetStartConnected()
{
    return IsStartConnected;
}

bool CustomPixmapItem::GetEndConnected()
{
    return IsEndConnected;
}

void CustomPixmapItem::SetItemId(int itemId)
{
    ItemId = itemId;
}

int CustomPixmapItem::GetItemId()
{
    return ItemId;
}

void CustomPixmapItem::HideLabelIfNeeded()
{
    if(TextLabel->text().compare(DEFAULT_TEXT) == 0)
    {
        TextLabel->hide();
    }
}
