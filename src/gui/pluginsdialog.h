#ifndef PLUGINSDIALOG_H
#define PLUGINSDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class PluginsDialog;
}

class PluginsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit PluginsDialog(QWidget * parent = 0);
        ~PluginsDialog();

    protected:
        virtual void changeEvent(QEvent *e) override;

    private slots:
        void populatePluginTree();
        void on_pluginTree_currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);
        void on_detailsButton_clicked();

    private:
        Ui::PluginsDialog * ui;
};

#endif // PLUGINSDIALOG_H
