#include "pluginsdialog.h"
#include "ui_pluginsdialog.h"

#include "plugindetailsdialog.h"
#include "managers/emoglercore.h"

PluginsDialog::PluginsDialog(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::PluginsDialog)
{
    ui->setupUi(this);

    populatePluginTree();
}

void PluginsDialog::changeEvent(QEvent * e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            populatePluginTree();
            break;
        default:
            break;
    }
}

void PluginsDialog::populatePluginTree()
{
    // remove children of top level items
    for (int i = 0; i < ui->pluginTree->topLevelItemCount(); i++) {
        QTreeWidgetItem * it = ui->pluginTree->topLevelItem(i);
        while (it->childCount())
            delete it->takeChild(0);
    }

    EmoglerCore & core = EmoglerCore::instance();
    PluginManager & pman = core.pluginManager();
    for (Plugin * pl : pman.plugins()) {
        QTreeWidgetItem * it;

        switch (pl->category()) {
            case Plugin::Extension:
                it = new QTreeWidgetItem(ui->pluginTree->topLevelItem(0));
                break;
            case Plugin::Protocol:
                it = new QTreeWidgetItem(ui->pluginTree->topLevelItem(1));
                break;
            default:
                continue;
        }

        it->setText(0, pl->name(core.language()));
        it->setText(1, pl->description(core.language()));
        it->setIcon(0, pl->icon());
        it->setCheckState(0, (pl->isLoaded()) ? Qt::Checked : Qt::Unchecked);
        it->setData(0, Qt::UserRole, QVariant::fromValue<Plugin *>(pl));
    }

    connect(ui->pluginTree, &QTreeWidget::itemChanged, this, [](QTreeWidgetItem * it, int column) {
        if (column != 0)
            return;

        Plugin * pl = it->data(0, Qt::UserRole).value<Plugin *>();
        if (pl) {
            bool checked = it->checkState(0) == Qt::Checked;
            if (pl->isLoaded() != checked) {
                pl->toggleLoad();
            }
        }
    });


    ui->pluginTree->expandAll();
    ui->pluginTree->resizeColumnToContents(0);
}

void PluginsDialog::on_pluginTree_currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * /*previous*/)
{
    QVariant data = current->data(0, Qt::UserRole);
    ui->detailsButton->setEnabled(data.isValid());
}

void PluginsDialog::on_detailsButton_clicked()
{
    QVariant data = ui->pluginTree->currentItem()->data(0, Qt::UserRole);
    if (data.isValid()) {
        Plugin * pl = data.value<Plugin *>();
        PluginDetailsDialog dlg(this, *pl);
        dlg.exec();
    }
}

PluginsDialog::~PluginsDialog()
{
    delete ui;
}
