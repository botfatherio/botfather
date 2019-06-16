#include "bot_config_widget.hpp"
#include "ui_botconfigwidget.h"
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QDebug>

BotConfigWidget::BotConfigWidget(Bot *bot, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::BotConfigWidget)
{
	ui->setupUi(this);
	QVBoxLayout *config_container_layout = new QVBoxLayout(ui->config_container);
	config_container_layout->setMargin(0);
	ui->config_container->setLayout(config_container_layout);

	QTimer::singleShot(1, [this, bot, config_container_layout](){
		renderConfig(bot->manifest().config(), config_container_layout);
	});
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
		if (group.options().isEmpty())
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

	for (const BotConfigOption &option : group.options())
	{
		if (!option.isValid())
		{
			qDebug() << "Skip invalid bot config settings.";
			continue;
		}

		BotConfigOption *option_p = new BotConfigOption(option);
		m_bot_config_settings.append(option_p);

		if (!option_p->isStored())
		{
			// Store the fallback if there is no other value stored yet. When there is no value stored
			// the Engines ConfigAPI won't return a value (not even the fallback)
			option_p->setValue(option_p->fallback());
		}

		renderOption(option_p, form_layout);
	}
}

void BotConfigWidget::renderOption(BotConfigOption *option_p, QFormLayout *form_layout)
{
	switch (option_p->type()) {
	case BotConfigOption::Type::Boolean: {
		renderBooleanOption(option_p, form_layout);
		break;
	}
	case BotConfigOption::Type::Number: {
		renderNumberOption(option_p, form_layout);
		break;
	}
	case BotConfigOption::Type::Integer: {
		renderIntegerOption(option_p, form_layout);
		break;
	}
	case BotConfigOption::Type::String: {
		renderStringOption(option_p, form_layout);
		break;
	}
	case BotConfigOption::Type::Password: {
		renderPasswordOption(option_p, form_layout);
		break;
	}
	case BotConfigOption::Type::Choice: {
		renderChoiceOption(option_p, form_layout);
		break;
	}
	default:
		qDebug() << "Unhandled switch case  in 'BotConfigWidget::renderBotConfigSetting'";
		break;
	}
}

void BotConfigWidget::renderBooleanOption(BotConfigOption *option_p, QFormLayout *form_layout)
{
	QCheckBox *checkbox = new QCheckBox();
	form_layout->addRow(option_p->label(), checkbox);

	checkbox->setChecked(option_p->value().toBool());
	connect(checkbox, &QCheckBox::toggled, [option_p](bool checked){
		option_p->setValue(checked);
	});
}

void BotConfigWidget::renderNumberOption(BotConfigOption *option_p, QFormLayout *form_layout)
{
	QDoubleSpinBox *spinbox = new QDoubleSpinBox();
	form_layout->addRow(option_p->label(), spinbox);

	if (option_p->hasRange())
	{
		spinbox->setRange(option_p->min(), option_p->max());
	}

	spinbox->setValue(option_p->value().toDouble());
	connect(spinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [option_p](double new_value){
		option_p->setValue(new_value);
	});
}

void BotConfigWidget::renderIntegerOption(BotConfigOption *option_p, QFormLayout *form_layout)
{
	QSpinBox *spinbox = new QSpinBox();
	form_layout->addRow(option_p->label(), spinbox);

	if (option_p->hasRange())
	{
		spinbox->setRange(option_p->min(), option_p->max());
	}

	spinbox->setValue(option_p->value().toInt());
	connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged), [option_p](int new_value){
		option_p->setValue(new_value);
	});
}

void BotConfigWidget::renderStringOption(BotConfigOption *option_p, QFormLayout *form_layout)
{
	QLineEdit *lineedit = new QLineEdit();
	lineedit->setPlaceholderText(option_p->label());
	lineedit->setClearButtonEnabled(true);
	form_layout->addRow(option_p->label(), lineedit);

	lineedit->setText(option_p->value().toString());
	connect(lineedit, &QLineEdit::editingFinished, [lineedit, option_p](){
		option_p->setValue(lineedit->text());
	});
}

void BotConfigWidget::renderPasswordOption(BotConfigOption *option_p, QFormLayout *form_layout)
{
	QLineEdit *lineedit = new QLineEdit();
	lineedit->setPlaceholderText(option_p->label());
	lineedit->setEchoMode(QLineEdit::EchoMode::Password);
	lineedit->setClearButtonEnabled(true);
	form_layout->addRow(option_p->label(), lineedit);

	lineedit->setText(option_p->value().toString());
	connect(lineedit, &QLineEdit::editingFinished, [lineedit, option_p](){
		option_p->setValue(lineedit->text());
	});
}

void BotConfigWidget::renderChoiceOption(BotConfigOption *option_p, QFormLayout *form_layout)
{
	QComboBox *comboxbox = new QComboBox();
	form_layout->addRow(option_p->label(), comboxbox);

	QMapIterator<QString, QString> ci(option_p->choices());
	while (ci.hasNext())
	{
		ci.next();
		comboxbox->addItem(ci.key(), ci.value());
	}

	int stored_index = comboxbox->findData(option_p->value());
	comboxbox->setCurrentIndex(stored_index);

	connect(comboxbox, QOverload<int>::of(&QComboBox::currentIndexChanged), [comboxbox, option_p](){
		option_p->setValue(comboxbox->currentData());
	});
}
