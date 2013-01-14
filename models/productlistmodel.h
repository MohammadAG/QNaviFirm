#ifndef PRODUCTLISTMODEL_H
#define PRODUCTLISTMODEL_H

#include <QAbstractListModel>

#include "defines.h"

class ProductListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ProductListModel(QObject *parent = 0);
    void setSourceData(QList<Product> listOfProducts);

signals:

public slots:

private:
    QList<Product> m_listOfProducts;

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
};

#endif // PRODUCTLISTMODEL_H
