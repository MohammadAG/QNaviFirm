#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QDebug>
#include <QNetworkCookieJar>

#include "qtsoap.h"
#include "defines.h"

#define SID_URL "https://www.caresuite.nokia.com/caresuite/get_params.do?application_id=2"
#define GET_INFO_URL "https://www.caresuite.nokia.com/caresuite/CareSuite?WSDL"
#define SID_NOT_YET_FETCHED "blabla"

class ServerInterface : public QObject
{
    Q_OBJECT
public:
    explicit ServerInterface(QObject *parent = 0);

    QString sid() { return m_sid; }

signals:
    void gotSid(QString sid);
    void productListUpdated(QList<Product> products);
    void releaseListUpdated(QList<Release> releases);

public slots:
    void getSid();
    void getProductList();
    void getReleasesList(long forProductId);
    void getVariantsList(long forReleaseId);

private:
    QNetworkAccessManager *m_nam;
    QNetworkReply *m_reply;

    QNetworkCookieJar *m_cookieJar;

    QString m_sid;

    QString constructSoapMessage(QString methodName, QString payload);
    QString constructPayload(QString text);

    Product parseProduct(QXmlStreamReader &xml);
    Release parseRelease(QXmlStreamReader &xml);

private slots:
    void onGotSid();
    void onGotProductsList();
    void onGotReleasesList();
    void onGotVariantsList();
};

#endif // SERVERINTERFACE_H
