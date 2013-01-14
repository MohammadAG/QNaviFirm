#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "serverinterface.h"
#include "models/productlistmodel.h"
#include "models/releaselistmodel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ServerInterface *m_serverInterface;

private slots:
    void onGotSid(QString);
    void onProductListUpdated(QList<Product> listOfProducts);
    void onReleaseListUpdated(QList<Release> listOfReleases);

    void onProductClicked(QModelIndex index);
    void onReleaseClicked(QModelIndex index);
};

#endif // MAINWINDOW_H
