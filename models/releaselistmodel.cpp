#include "releaselistmodel.h"

ReleaseListModel::ReleaseListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void ReleaseListModel::setSourceData(QList<Release> listOfReleases)
{
    m_listOfReleases = listOfReleases;
    reset();
}

int ReleaseListModel::rowCount(const QModelIndex &parent) const
{
    return m_listOfReleases.count();
}

QVariant ReleaseListModel::data(const QModelIndex &index, int role) const
{
    Release release = m_listOfReleases.at(index.row());
    if (role == Qt::DisplayRole) {
        return release.version;
    } else if (role == Qt::UserRole) {
        return QVariant(release.releaseId);
    } else {
        return QVariant();
    }
}
