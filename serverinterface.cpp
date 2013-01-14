#include "serverinterface.h"

ServerInterface::ServerInterface(QObject *parent) :
    QObject(parent),
    m_nam(new QNetworkAccessManager(this)),
    m_cookieJar(new QNetworkCookieJar(this))
{
    m_nam->setCookieJar(m_cookieJar);
    m_sid = SID_NOT_YET_FETCHED;
}

void ServerInterface::getSid()
{
    QNetworkReply *reply = m_nam->get(QNetworkRequest(QUrl(SID_URL)));
    if (reply)
        connect(reply, SIGNAL(finished()), this, SLOT(onGotSid()));
}

void ServerInterface::onGotSid()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    QVariant cookieVar = reply->header(QNetworkRequest::SetCookieHeader);
    if (cookieVar.isValid()) {
        QList<QNetworkCookie> cookies = cookieVar.value<QList<QNetworkCookie> >();
        foreach (QNetworkCookie cookie, cookies) {
            if (cookie.name() == "JSESSIONID") {
                m_sid = cookie.value();
                qDebug() << "Got sid" << m_sid;
                emit gotSid(m_sid);
            }
        }
    }
}

void ServerInterface::getProductList()
{
    qDebug() << Q_FUNC_INFO;
    QString soapMessage = constructSoapMessage("GetProductList", constructPayload("<string0 xsi:type=\"xsd:string\"/>"));
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
    request.setUrl(QUrl(GET_INFO_URL));
    m_reply = m_nam->post(request, soapMessage.toUtf8());
    if (m_reply)
        connect(m_reply, SIGNAL(finished()), this, SLOT(onGotProductsList()));
}

void ServerInterface::onGotProductsList()
{
    QByteArray data = m_reply->readAll();
    m_reply->deleteLater();
    m_reply = 0;

    QtSoapMessage msg;
    msg.setContent(data);

    QString xmlData = msg.toXmlString();
    qDebug() << xmlData;

    msg.clear();
    data.clear();

    QXmlStreamReader xml(xmlData);
    QList<Product> listOfProducts;

    bool foundRespone = false;

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() != "result" && !foundRespone) {
                continue;
            } else {
                foundRespone = true;
            }

            if (xml.name() == "product") {
                Product product = parseProduct(xml);
                if (product.productId != 0)
                    listOfProducts.append(product);
            }
        }

        if (token == QXmlStreamReader::EndElement) {
            if (xml.name() == "result")
                break;
        }
    }

    emit productListUpdated(listOfProducts);
}

Product ServerInterface::parseProduct(QXmlStreamReader &xml)
{
    Product product;

    while (!(xml.tokenType() == QXmlStreamReader::EndElement &&
             xml.name() == "product")) {

        // Yay for lazy coding
        if (xml.tokenType() == QXmlStreamReader::StartElement) {
            if (xml.name() == "marketName") {
                xml.readNext();
                product.marketName = xml.text().toString();
            }

            if (xml.name() == "productID") {
                xml.readNext();
                product.productId = xml.text().toString();
            }

            if (xml.name() == "productType") {
                xml.readNext();
                product.productType = xml.text().toString();
            }

            if (xml.name() == "corePlatform") {
                xml.readNext();
                product.corePlatform = xml.text().toString();
            }

            if (xml.name() == "turnOnBeforeFactorySet") {
                xml.readNext();
                product.turnOnBeforeFactorySet = QVariant(xml.text().toString()).toBool();
            }

            if (xml.name() == "supportedFlashSequences") {
                xml.readNext();
                product.supportedFlashSequences = xml.text().toString();
            }

            if (xml.name() == "PPMFilePolicy") {
                xml.readNext();
                product.PPMFilePolicy = xml.text().toString().toInt();
            }

            if (xml.name() == "contentFilePolicy") {
                xml.readNext();
                product.contentFilePolicy = xml.text().toString().toInt();
            }

            if (xml.name() == "variantFilePolicy") {
                xml.readNext();
                product.variantFilePolicy = xml.text().toString().toInt();
            }
            /*
            if (xml.name() == "files") {
                while (!(xml.tokenType() == QXmlStreamReader::EndElement &&
                         xml.name() == "files")) {
                    NokiaFile file;
                    while (!(xml.tokenType() == QXmlStreamReader::EndElement &&
                             xml.name() == "file")) {
                        if (xml.name() == "fileName") {
                            xml.readNext();
                            file.fileName = xml.text().toString();
                        }

                        if (xml.name() == "fileSize") {
                            xml.readNext();
                            file.fileSize = xml.text().toString().toLong();
                        }

                        if (xml.name() == "downloadURL") {
                            xml.readNext();
                            file.downloadURL = xml.text().toString();
                        }

                        xml.readNext();
                    }
                    product.fileList.append(file);
                    xml.readNext();
                }
            }*/
        }

        xml.readNext();
    }

    qDebug() << QString("Found product %1 with ID %2").arg(product.marketName, product.productId);
    return product;
}

void ServerInterface::getReleasesList(long productId)
{
    qDebug() << "Loading releases for product" << productId;
    QString text = QString("<longVal xsi:type=\"xsd:long\">%1</longVal>").arg(productId);
    QString soapMessage = constructSoapMessage("GetReleasesForProduct", constructPayload(text));
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
    request.setUrl(QUrl(GET_INFO_URL));
    m_reply = m_nam->post(request, soapMessage.toUtf8());
    if (m_reply)
        connect(m_reply, SIGNAL(finished()), this, SLOT(onGotReleasesList()));
}

void ServerInterface::onGotReleasesList()
{
    QByteArray data = m_reply->readAll();
    m_reply->deleteLater();
    m_reply = 0;

    QtSoapMessage msg;
    msg.setContent(data);

    QString xmlData = msg.toXmlString(1);

    msg.clear();
    data.clear();

    qDebug() << xmlData;

    bool foundRespone = false;
    QList<Release> listOfReleases;

    QXmlStreamReader xml(xmlData);

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() != "result" && !foundRespone) {
                continue;
            } else {
                foundRespone = true;
            }

            if (xml.name() == "release")
                listOfReleases.append(parseRelease(xml));
        }

        if (token == QXmlStreamReader::EndElement) {
            if (xml.name() == "result")
                break;
        }
    }

    emit releaseListUpdated(listOfReleases);
}

Release ServerInterface::parseRelease(QXmlStreamReader &xml)
{
    Release release;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement &&
             xml.name() == "release")) {

        // Yay for lazy coding
        if (xml.tokenType() == QXmlStreamReader::StartElement) {
            if (xml.name() == "releaseID") {
                xml.readNext();
                release.releaseId = xml.text().toString().toLong();
            }

            if (xml.name() == "version") {
                xml.readNext();
                release.version = xml.text().toString();
            }
        }

        xml.readNext();
    }

    return release;
}

void ServerInterface::getVariantsList(long releaseId)
{
    QString text = QString("<longVal xsi:type=\"xsd:long\">%1</longVal>").arg(releaseId);
    QString soapMessage = constructSoapMessage("GetReleaseVariants", constructPayload(text));
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
    request.setUrl(QUrl(GET_INFO_URL));
    m_reply = m_nam->post(request, soapMessage.toUtf8());
    if (m_reply)
        connect(m_reply, SIGNAL(finished()), this, SLOT(onGotVariantsList()));
}

void ServerInterface::onGotVariantsList()
{

}

QString ServerInterface::constructPayload(QString text)
{
    QString payload;
    payload.append(QString("\t\t<string xsi:type=\"xsd:string\">%1</string>").arg(m_sid));
    payload.append("\n");
    payload.append(QString("\t\t%1").arg(text));
    payload.append("\n");
    return payload;
}

QString ServerInterface::constructSoapMessage(QString methodName, QString payload)
{
    QString soapMessage;
    soapMessage.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
    soapMessage.append("\n");
    soapMessage.append("<soap:Envelope");
    soapMessage.append("\n");
    soapMessage.append("\txmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"");
    soapMessage.append("\n");
    soapMessage.append("\txmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\"");
    soapMessage.append("\n");
    soapMessage.append("\txmlns:tns=\"http://www.posti-care_suite.posti.nokia.com/POSTI_CareSuite_Interface.wsdl\"");
    soapMessage.append("\n");
    soapMessage.append("\txmlns:types=\"http://www.posti-care_suite.posti.nokia.com/POSTI_CareSuite_Interface.wsdl/encodedTypes\"");
    soapMessage.append("\n");
    soapMessage.append("\txmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"");
    soapMessage.append("\n");
    soapMessage.append("\txmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">");
    soapMessage.append("\n");
    soapMessage.append("    <soap:Body soap:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">");
    soapMessage.append("\n");
    soapMessage.append(QString("\t<tns:%1>").arg(methodName));
    soapMessage.append("\n");
    soapMessage.append(payload);
    soapMessage.append(QString("\t</tns:%1>").arg(methodName));
    soapMessage.append("\n");
    soapMessage.append("    </soap:Body>");
    soapMessage.append("\n");
    soapMessage.append("</soap:Envelope>");
    soapMessage.append("\n");
    return soapMessage;
}
