#ifndef EMOTICONSTABLEMODEL_H
#define EMOTICONSTABLEMODEL_H

#include <QAbstractTableModel>

#include "managers/emoticonsmanager.h"

class EmoticonsTableModel : public QAbstractTableModel
{
    Q_OBJECT

    public:
        explicit EmoticonsTableModel(EmoticonsManager & eman, QObject * parent = 0);

        virtual int rowCount(const QModelIndex & parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex & parent = QModelIndex()) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
        virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
        int switchRows(int srcRow, int destRow);
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

        void commit();

    signals:
        void checkStateChanged(int, Qt::CheckState);

    public slots:

    private:
        void init();

        EmoticonsManager & mMan;
        QMap<int, int> mRowChanges;
        QList<bool> mRowsEnabled;
};

#endif // EMOTICONSTABLEMODEL_H
