#include "emoticonstablemodel.h"

EmoticonsTableModel::EmoticonsTableModel(EmoticonsManager & eman, QObject * parent) :
    QAbstractTableModel(parent),
    mMan(eman)
{
    init();
}

void EmoticonsTableModel::init()
{
    mRowChanges.clear();
    mRowsEnabled.clear();

    for (int i = 0; i < mMan.packs().size(); i++) {
        EmoticonPack * pack = mMan.packs().at(i);
        mRowsEnabled << pack->isEnabled();
        mRowChanges[i] = i;

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
        EmoticonPack * pack = mMan.packs().at(mRowChanges[index.row()]);
        switch (index.column()) {
            case 0:
                return pack->name();
            case 2:
                return pack->id();
        }
    } else if (role == Qt::CheckStateRole) {
        if (index.column() == 1) {
            if (mRowsEnabled[index.row()]) {
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
        mRowsEnabled[mRowChanges[index.row()]] = (state == Qt::Checked);

        emit checkStateChanged(index.row(), state);
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

    qSwap(mRowChanges[srcRow], mRowChanges[destRow]);
    qSwap(mRowsEnabled[srcRow], mRowsEnabled[destRow]);

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
    QSet<int> usedTos;
    for (int from : mRowChanges.keys()) {
        if (usedTos.contains(from))
            continue;

        int to = mRowChanges[from];
        mMan.swapPacks(from, to);
        usedTos << to;
    }

    for (int i = 0; i < mRowsEnabled.size(); i++) {
        mMan.packs().at(i)->setEnabled(mRowsEnabled[i]);
    }

    init();
}
