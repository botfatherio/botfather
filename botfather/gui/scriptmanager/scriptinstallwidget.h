#ifndef SCRIPTINSTALLWIDGET_H
#define SCRIPTINSTALLWIDGET_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include "scriptlistmodel.h"

namespace Ui {
class ScriptListWidget;
}

class ScriptInstallWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ScriptInstallWidget(QWidget *parent = nullptr);
	~ScriptInstallWidget();

public slots:
	void loadModelData();
	void itemDoubleClicked(const QModelIndex &index);

signals:
	void scriptInstalled(ScriptRepository *repository);

private:
	Ui::ScriptListWidget *m_ui;
	ScriptListModel *m_scripts_model;
	QSortFilterProxyModel *m_scripts_proxy;
};

#endif // SCRIPTINSTALLWIDGET_H
