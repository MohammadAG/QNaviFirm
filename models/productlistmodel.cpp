#include "productlistmodel.h"

ProductListModel::ProductListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void ProductListModel::setSourceData(QList<Product> listOfProducts)
{
    m_listOfProducts = listOfProducts;
    reset();
}

int ProductListModel::rowCount(const QModelIndex &parent) const
{
    return m_listOfProducts.count();
}

QVariant ProductListModel::data(const QModelIndex &index, int role) const
{
    Product product = m_listOfProducts.at(index.row());
    if (role == Qt::DisplayRole) {
        return QString("%1 (%2)").arg(product.marketName, product.productType);
    } else if (role == Qt::UserRole) {
        return QVariant(product.productId);
    } else {
        return QVariant();
    }
}
