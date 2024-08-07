#ifndef CUSTOMDELEGATE_H
#define CUSTOMDELEGATE_H

#include <QStyledItemDelegate>
#include <QStandardItem>
#include <QLabel>

class IconListModel : public QStandardItemModel
{
public:
    explicit IconListModel(QObject* parent = nullptr) : QStandardItemModel(parent)
    {

    }

    void setData(const QStringList& labels, const QList<QIcon>& icons)
    {
        Q_ASSERT(labels.size() == icons.size());

        for (int i = 0; i < labels.size(); ++i)
        {
            QStandardItem* item = new QStandardItem(icons.at(i), QString());
            item->setData(icons.at(i), Qt::UserRole + 1); // Store the icon for drag and drop
            item->setData(labels.at(i), Qt::ToolTipRole);

            appendRow(item);
        }
    }
};

class CustomDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    CustomDelegate(int rowHeight, QObject *parent = nullptr);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setRowHeight(int rowHeight);

signals:
    void sizeHintChanged();

private:
    int m_rowHeight;
};

#endif // CUSTOMDELEGATE_H
