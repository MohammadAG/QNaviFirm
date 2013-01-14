#ifndef RELEASELISTMODEL_H
#define RELEASELISTMODEL_H

#include <QAbstractListModel>

#include "defines.h"

class ReleaseListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ReleaseListModel(QObject *parent = 0);
    void setSourceData(QList<Release> listOfReleases);

signals:

public slots:

private:
    QList<Release> m_listOfReleases;

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
};

#endif // RELEASELISTMODEL_H
