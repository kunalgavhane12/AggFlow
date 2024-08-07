#include <customdelegate.h>

CustomDelegate::CustomDelegate(int rowHeight, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_rowHeight(rowHeight)
{

}

QSize CustomDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(m_rowHeight);
    return size;
}

void CustomDelegate::setRowHeight(int rowHeight)
{
    if (m_rowHeight != rowHeight)
    {
        m_rowHeight = rowHeight;
        emit sizeHintChanged();
    }
}
