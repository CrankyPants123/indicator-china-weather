#ifndef CITYCOLLECTIONITEM_H
#define CITYCOLLECTIONITEM_H

#include "data.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkConfigurationManager>

#include <QWidget>
#include <QTime>

namespace Ui {
class citycollectionitem;
}

class citycollectionitem : public QWidget
{
    Q_OBJECT

public:
    explicit citycollectionitem(QWidget *parent = nullptr);
    ~citycollectionitem();

    void setItemWidgetState(bool isShowNormal);
    void setCurrentWeather(QString cityId);
    QString convertCodeToBackgroud(int code);

    QString m_city_id = "";
    bool is_normal_item = true;

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void onWeatherDataRequest(const QString &cityId);
    void onWeatherDataReply();

    void on_btnAddCity_clicked();

    void on_btnDelete_clicked();

private:
    Ui::citycollectionitem *ui;

    QNetworkAccessManager *m_networkManager = nullptr;

signals:
    void showCityAddWiget();
    void requestDeleteCity(QString cityId);
};

#endif // CITYCOLLECTIONITEM_H
