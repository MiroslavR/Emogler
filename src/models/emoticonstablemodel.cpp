#include "emoticonstablemodel.h"

EmoticonsTableModel::EmoticonsTableModel(EmoticonsManager & eman, QObject * parent) :
    QAbstractTableModel(parent),
    mMan(eman)
{
    init();
}

void EmoticonsTableModel::init()
{
    mRows.clear();

    for (int i = 0; i < mMan.packs().size(); i++) {
        const EmoticonPack * pack = mMan.packs().at(i);
        mRows << Row{i, pack->isEnabled()};

        emit dataChanged(index(i, 0), index(i, columnCount()));
    }
}

int EmoticonsTableModel::rowCount(const QModelIndex & /*parent*/) const
{
    return mMan.packs().size();
}

int EmoticonsTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 3;
}

QVariant EmoticonsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Use");
            case 2:
                return tr("ID");
            default:
                break;
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

QVariant EmoticonsTableModel::data(const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole) {
        const EmoticonPack * pack = mMan.packs().at(mRows[index.row()].origIndex);
        switch (index.column()) {
            case 0:
                return pack->name();
            case 2:
                return pack->id();
        }
    } else if (role == Qt::CheckStateRole) {
        if (index.column() == 1) {
            if (mRows[index.row()].enabled) {
                return Qt::Checked;
            } else {
                return Qt::Unchecked;
            }
        }
    }
    return QVariant();
}

bool EmoticonsTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::CheckStateRole) {
        Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
        mRows[index.row()].enabled = (state == Qt::Checked);

        emit checkStateChanged(index.row(), state);
        emit dataChanged(index, index, QVector<int>() << Qt::CheckStateRole);
    }
    return true;
}

int EmoticonsTableModel::switchRows(int srcRow, int destRow)
{
    if ((srcRow < 0 || srcRow >= rowCount()) || (destRow < 0 || destRow >= rowCount()))  {
        return false;
    }

    if (srcRow == destRow)
        return true;

    std::swap(mRows[srcRow], mRows[destRow]);

    emit dataChanged(index(srcRow, 0), index(srcRow, columnCount()));
    emit dataChanged(index(destRow, 0), index(destRow, columnCount()));

    return true;
}

Qt::ItemFlags EmoticonsTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags ret = QAbstractTableModel::flags(index);
    if (index.column() == 1) {
        ret |= Qt::ItemIsUserCheckable;
    }
    return ret;
}

void EmoticonsTableModel::commit()
{
    for (int i = 0; i < mRows.size(); i++) {
        const Row & row = mRows[i];
        mMan.packs().at(row.origIndex)->setPriority(mRows.size() - i);
        mMan.packs().at(row.origIndex)->setEnabled(row.enabled);
    }

    mMan.sortPacksByPriority();

    init();
}
