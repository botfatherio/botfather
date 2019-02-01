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
	qDeleteAll(m_bot_config_settings);
	delete ui;
}

void BotConfigWidget::renderConfig(BotConfig bot_config, QVBoxLayout *vbox_layout)
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

void BotConfigWidget::renderGroup(BotConfigGroup group, QVBoxLayout *vbox_layout)
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

		BotConfigSetting *setting_p = new BotConfigSetting(setting);
		m_bot_config_settings.append(setting_p);

		renderSetting(setting_p, form_layout);
	}
}

void BotConfigWidget::renderSetting(BotConfigSetting *setting, QFormLayout *form_layout)
{
	switch (setting->type()) {
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

void BotConfigWidget::renderBooleanSetting(BotConfigSetting *setting, QFormLayout *form_layout)
{
	QCheckBox *checkbox = new QCheckBox();
	form_layout->addRow(setting->label(), checkbox);

	checkbox->setChecked(setting->value().toBool());
	connect(checkbox, &QCheckBox::toggled, [setting](bool checked){
		setting->setValue(checked);
	});
}

void BotConfigWidget::renderNumberSetting(BotConfigSetting *setting, QFormLayout *form_layout)
{
	QDoubleSpinBox *spinbox = new QDoubleSpinBox();
	form_layout->addRow(setting->label(), spinbox);

	spinbox->setValue(setting->value().toDouble());
	connect(spinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [setting](double new_value){
		setting->setValue(new_value);
	});
}

void BotConfigWidget::renderIntegerSetting(BotConfigSetting *setting, QFormLayout *form_layout)
{
	QSpinBox *spinbox = new QSpinBox();
	form_layout->addRow(setting->label(), spinbox);

	spinbox->setValue(setting->value().toInt());
	connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged), [setting](int new_value){
		setting->setValue(new_value);
	});
}

void BotConfigWidget::renderStringSetting(BotConfigSetting *setting, QFormLayout *form_layout)
{
	QLineEdit *lineedit = new QLineEdit();
	lineedit->setPlaceholderText(setting->label());
	form_layout->addRow(setting->label(), lineedit);

	lineedit->setText(setting->value().toString());
	connect(lineedit, &QLineEdit::editingFinished, [lineedit, setting](){
		setting->setValue(lineedit->text());
	});
}

void BotConfigWidget::renderPasswordSetting(BotConfigSetting *setting, QFormLayout *form_layout)
{
	QLineEdit *lineedit = new QLineEdit();
	lineedit->setPlaceholderText(setting->label());
	lineedit->setEchoMode(QLineEdit::EchoMode::Password);
	form_layout->addRow(setting->label(), lineedit);

	lineedit->setText(setting->value().toString());
	connect(lineedit, &QLineEdit::editingFinished, [lineedit, setting](){
		setting->setValue(lineedit->text());
	});
}

void BotConfigWidget::renderChoiceSetting(BotConfigSetting *setting, QFormLayout *form_layout)
{
	QComboBox *comboxbox = new QComboBox();
	form_layout->addRow(setting->label(), comboxbox);

	QMapIterator<QString, QString> ci(setting->choices());
	while (ci.hasNext())
	{
		ci.next();
		comboxbox->addItem(ci.key(), ci.value());
	}

	int stored_index = comboxbox->findData(setting->value());
	comboxbox->setCurrentIndex(stored_index);

	connect(comboxbox, QOverload<int>::of(&QComboBox::currentIndexChanged), [comboxbox, setting](){
		setting->setValue(comboxbox->currentData());
	});
}
