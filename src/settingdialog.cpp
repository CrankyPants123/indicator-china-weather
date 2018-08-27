/*
 * Copyright (C) 2013 ~ 2018 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
 *
 * Authors:
 *  Kobe Lee    lixiang@kylinos.cn/kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "settingdialog.h"
#include "settingtitlebar.h"
#include "searchdialog.h"
//#include "citylistwidget.h"
#include "citywidget.h"
#include "spinbox.h"

#include <QApplication>
#include <QDesktopServices>
#include <QStackedWidget>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDebug>

namespace {
const int BUTTON_WIDGET_HEIGHT = 30;
const int GROUP_BOX_MARGIN = 20;
}

SettingDialog::SettingDialog(QWidget *parent):
    QDialog(parent)
    , m_mainLayout(new QVBoxLayout(this))
    , m_stackedWidget(new QStackedWidget)
//    , m_okBtn(new QPushButton(this))
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(442, 326);
    this->setFocusPolicy(Qt::ClickFocus);
    this->setWindowTitle(tr("Kylin Weather - Setting"));
    this->setWindowIcon(QIcon(":/res/indicator-china-weather.png"));

    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(0);

    m_titleBar = new SettingTitleBar;
    m_titleBar->setFixedHeight(100);
    m_locationWidget = new CityWidget;//m_locationWidget = new QWidget;
//    m_locationWidget->setContentsMargins(0, 0, 0, 0);
    m_locationWidget->setFixedHeight(this->height() - m_titleBar->height()/* - BUTTON_WIDGET_HEIGHT*/);
    m_locationWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_systemWidget = new QWidget;
    m_systemWidget->setStyleSheet("QWidget{border-radius: 0px; background-color:#ffffff;}");
    m_systemWidget->setFixedHeight(this->height() - m_titleBar->height()/* - BUTTON_WIDGET_HEIGHT*/);
    m_systemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    /*m_okBtn->setFixedSize(90, 25);
    m_okBtn->setFocusPolicy(Qt::NoFocus);
    m_okBtn->setText(tr("Close"));*/

    //------------------
    /*m_cityListWidget = new CityListWidget(m_locationWidget);
    m_addCityBtn = new QPushButton(m_locationWidget);
    m_addCityBtn->setFocusPolicy(Qt::NoFocus);
    m_addCityBtn->setStyleSheet("QPushButton{font-size:12px;color:#808080;background:transparent;text-align:left;}");//margin-left:18px;border:1px solid rgba(0, 0, 0, 0.08);   QPushButton:hover{background-color:#f5fbff;}
    m_addCityBtn->setFixedWidth(this->width());
    m_addCityBtn->setText("Add");
    m_addCityBtn->setIconSize(QSize(24, 24));
    m_addCityBtn->setIcon(QIcon(":/res/add.png"));

    QVBoxLayout *m_cityLayout = new QVBoxLayout(m_locationWidget);
    m_cityLayout->setContentsMargins(0, 0, 0, 0);
    m_cityLayout->setSpacing(0);
    m_cityLayout->addWidget(m_cityListWidget, 1, Qt::AlignTop);
    m_cityLayout->addWidget(m_addCityBtn, 0, Qt::AlignBottom);*/

    //-------------------------
    m_variableGroup = new QGroupBox(m_systemWidget);
    m_variableGroup->setStyleSheet("QGroupBox{border:1px solid #e0e0e0;border-radius:2px;margin-top:15px;font-size:14px;}QGroupBox:title{subcontrol-origin: margin;subcontrol-position: top left;padding: 6px 3px;color:#00609a;font-weight:bold;}");
    m_variableGroup->setTitle(tr("Variable configuration"));
    m_variableGroup->setFixedWidth(this->width() - 2*GROUP_BOX_MARGIN);
//    m_variableGroup->setContentsMargins(10, 10, 10, 10);

    QLabel *updateFreqLabel = new QLabel;
    updateFreqLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    updateFreqLabel->setStyleSheet("QLabel{color:#808080;font-size:14px;text-align:left;}");
    updateFreqLabel->setText(tr("update frequency") + "(5-60)");
    m_spinBox = new SpinBox(m_systemWidget);
    m_spinBox->setFixedWidth(80);
    m_spinBox->setContextMenuPolicy(Qt::NoContextMenu);
    m_spinBox->setRange(15, 60);
    //TODO:Read default refresh time from conf
    m_spinBox->setValue(20);

    QHBoxLayout *m_variableLayout = new QHBoxLayout;
    m_variableLayout->setContentsMargins(GROUP_BOX_MARGIN, 10, GROUP_BOX_MARGIN, 10);
    m_variableLayout->setSpacing(5);
    m_variableLayout->addWidget(updateFreqLabel);
    m_variableLayout->addWidget(m_spinBox);
    m_variableGroup->setLayout(m_variableLayout);

    m_fixedGroup = new QGroupBox(m_systemWidget);
    m_fixedGroup->setStyleSheet("QGroupBox{border:1px solid #e0e0e0;border-radius:2px;margin-top:15px;font-size:14px;}QGroupBox:title{subcontrol-origin: margin;subcontrol-position: top left;padding: 6px 3px;color:#00609a;font-weight:bold;}");
    m_fixedGroup->setTitle(tr("Fixed configuration"));
    m_fixedGroup->setFixedWidth(m_variableGroup->width());

    QLabel *tempLabel = new QLabel;
    tempLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    tempLabel->setStyleSheet("QLabel{color:#808080;font-size:14px;text-align:left;}");
    tempLabel->setText(tr("Temperature standard") + ": " +  "˚C");
    QLabel *serviceLabel = new QLabel;
    serviceLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    serviceLabel->setStyleSheet("QLabel{color:#808080;font-size:14px;text-align:left;}");
    serviceLabel->setText(tr("Source of weather") + ": " +  "heweather");

    QVBoxLayout *m_fixedLayout = new QVBoxLayout;
    m_fixedLayout->setContentsMargins(GROUP_BOX_MARGIN, 10, GROUP_BOX_MARGIN, 10);
    m_fixedLayout->setSpacing(10);
    m_fixedLayout->addWidget(tempLabel);
    m_fixedLayout->addWidget(serviceLabel);
    m_fixedGroup->setLayout(m_fixedLayout);

    QVBoxLayout *m_systemLayout = new QVBoxLayout(m_systemWidget);
    m_systemLayout->setContentsMargins(10, 10, 10, 10);
    m_systemLayout->setSpacing(20);
    m_systemLayout->addWidget(m_variableGroup, 0, Qt::AlignLeft | Qt::AlignTop);
    m_systemLayout->addWidget(m_fixedGroup, 0, Qt::AlignLeft);
    m_systemLayout->addStretch();

    m_stackedWidget->addWidget(m_locationWidget);
    m_stackedWidget->addWidget(m_systemWidget);
    m_stackedWidget->setCurrentWidget(m_locationWidget);

    m_mainLayout->addWidget(m_titleBar, 0, Qt::AlignTop);
    m_mainLayout->addWidget(m_stackedWidget, 1, Qt::AlignVCenter);
//    m_mainLayout->addWidget(m_okBtn, 0, Qt::AlignRight);
    m_mainLayout->addStretch();

    connect(m_titleBar, &SettingTitleBar::requestCloseDialog, this, [=] {
        this->reject();
    });
    connect(m_locationWidget, &CityWidget::requestAddCity, this, [=] {
        SearchDialog dlg;
        connect(&dlg, &SearchDialog::requesetSetCurrentCity, this, [this] (const LocationData &data) {
            qDebug() << "set city's id=" << data.id;
            CitySettingData info;
            info.active = false;
            info.id = data.id;
            info.name = data.city;
            info.icon = ":/res/weather_icons/lightgrey/100.png";
            m_locationWidget->addCityItem(info);
            emit this->requesetSetCurrentCity(data);
        });
        dlg.exec();
    });
    connect(m_locationWidget, &CityWidget::requestRemoveCityFromMenu, this, &SettingDialog::requestRemoveCityFromMenu);
    /*connect(m_addCityBtn, &QPushButton::clicked, this, [=] {
        SearchDialog dlg;
        connect(&dlg, &SearchDialog::requesetSetCurrentCity, this, [this] (const LocationData &data) {
            qDebug() << "set city's id=" << data.id;
            CitySettingData info;
            info.active = false;
            info.id = data.id;
            info.name = data.city;
            info.icon = ":/res/weather_icons/lightgrey/100.png";
            m_cityListWidget->loadItem(info);
            emit this->requesetSetCurrentCity(data);
        });
        dlg.exec();
    });*/

    /*connect(m_okBtn, &QPushButton::clicked, this, [=] {
        this->accept();
    });*/
    connect(m_titleBar, &SettingTitleBar::requestSwitchPage, this, [=] (bool b) {
        if (b) {
            m_stackedWidget->setCurrentWidget(m_locationWidget);
//            m_cityListWidget->resetData();
        }
        else {
            m_stackedWidget->setCurrentWidget(m_systemWidget);
        }
    });

    connect(m_spinBox, static_cast<void(SpinBox::*)(int)>(&SpinBox::valueChanged), this, [=] (int value) {
        qDebug() << "SpinBox Value:"  << value << ", Text:"  << m_spinBox->text();
    });
}

SettingDialog::~SettingDialog()
{
    QLayoutItem *child;
    while ((child = m_mainLayout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void SettingDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = true;
        m_dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
    }
}

void SettingDialog::mouseReleaseEvent(QMouseEvent *)
{
    m_mousePressed = false;
}

void SettingDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mousePressed) {
        QPoint move_pos = event->globalPos();
        move(move_pos - m_dragPosition);
        event->accept();
    }
}

void SettingDialog::setData()
{

}

void SettingDialog::accept()
{
    hide();
    setResult(QDialog::Accepted);
    emit applied();
}

void SettingDialog::reject()
{
    hide();
    setResult(QDialog::Rejected);
    setResult(QDialog::Accepted);
}