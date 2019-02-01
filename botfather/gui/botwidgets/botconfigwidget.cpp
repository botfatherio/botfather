#include "botconfigwidget.h"
#include "ui_botconfigwidget.h"
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>

BotConfigWidget::BotConfigWidget(Bot *bot, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::BotConfigWidget)
{
	ui->setupUi(this);
	QVBoxLayout *config_container_layout = new QVBoxLayout(ui->config_container);
	config_container_layout->setMargin(0);
	ui->config_container->setLayout(config_container_layout);

	renderConfig(bot->manifest().config(), config_container_layout);
}

BotConfigWidget::~BotConfigWidget()
{
	delete ui;
}

void BotConfigWidget::renderConfig(const BotConfig &bot_config, QVBoxLayout *vbox_layout)
{
	for (const BotConfigGroup &group : bot_config.groups())
	{
		if (group.settings().isEmpty())
		{
			qDebug() << "Skip empty bot config group.";
			continue;
		}

		renderGroup(group, vbox_layout);
	}
	vbox_layout->addStretch(1);
}

void BotConfigWidget::renderGroup(const BotConfigGroup &group, QVBoxLayout *vbox_layout)
{
	QGroupBox *group_box = new QGroupBox(group.name(), this);
	QFormLayout *form_layout = new QFormLayout(group_box);
	form_layout->setLabelAlignment(Qt::AlignJustify);
	group_box->setLayout(form_layout);
	vbox_layout->addWidget(group_box);

	for (const BotConfigSetting &setting : group.settings())
	{
		if (!setting.isValid())
		{
			qDebug() << "Skip invalid bot config settings.";
			continue;
		}
		renderSetting(setting, form_layout);
	}
}

void BotConfigWidget::renderSetting(const BotConfigSetting &setting, QFormLayout *form_layout)
{
	switch (setting.type()) {
	case BotConfigSetting::Type::Boolean: {
		renderBooleanSetting(setting, form_layout);
		break;
	}
	case BotConfigSetting::Type::Number: {
		renderNumberSetting(setting, form_layout);
		break;
	}
	case BotConfigSetting::Type::Integer: {
		renderIntegerSetting(setting, form_layout);
		break;
	}
	case BotConfigSetting::Type::String: {
		renderStringSetting(setting, form_layout);
		break;
	}
	case BotConfigSetting::Type::Password: {
		renderPasswordSetting(setting, form_layout);
		break;
	}
	case BotConfigSetting::Type::Choice: {
		renderChoiceSetting(setting, form_layout);
		break;
	}
	default:
		qDebug() << "Unhandled switch case  in 'BotConfigWidget::renderBotConfigSetting'";
		break;
	}
}

void BotConfigWidget::renderBooleanSetting(const BotConfigSetting &setting, QFormLayout *form_layout)
{
	QCheckBox *checkbox = new QCheckBox();
	form_layout->addRow(setting.label(), checkbox);
}

void BotConfigWidget::renderNumberSetting(const BotConfigSetting &setting, QFormLayout *form_layout)
{
	QDoubleSpinBox *spinbox = new QDoubleSpinBox();
	form_layout->addRow(setting.label(), spinbox);
}

void BotConfigWidget::renderIntegerSetting(const BotConfigSetting &setting, QFormLayout *form_layout)
{
	QSpinBox *spinbox = new QSpinBox();
	form_layout->addRow(setting.label(), spinbox);
}

void BotConfigWidget::renderStringSetting(const BotConfigSetting &setting, QFormLayout *form_layout)
{
	QLineEdit *lineedit = new QLineEdit();
	lineedit->setPlaceholderText(setting.label());
	form_layout->addRow(setting.label(), lineedit);
}

void BotConfigWidget::renderPasswordSetting(const BotConfigSetting &setting, QFormLayout *form_layout)
{
	QLineEdit *lineedit = new QLineEdit();
	lineedit->setPlaceholderText(setting.label());
	lineedit->setEchoMode(QLineEdit::EchoMode::Password);
	form_layout->addRow(setting.label(), lineedit);
}

void BotConfigWidget::renderChoiceSetting(const BotConfigSetting &setting, QFormLayout *form_layout)
{
	QComboBox *comboxbox = new QComboBox();
	form_layout->addRow(setting.label(), comboxbox);

	QMapIterator<QString, QString> ci(setting.choices());
	while (ci.hasNext())
	{
		ci.next();
		comboxbox->addItem(ci.key(), ci.value());
	}
}
