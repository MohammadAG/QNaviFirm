#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_serverInterface = new ServerInterface(this);
    connect(m_serverInterface, SIGNAL(gotSid(QString)), this, SLOT(onGotSid(QString)));
    connect(m_serverInterface, SIGNAL(productListUpdated(QList<Product>)), this, SLOT(onProductListUpdated(QList<Product>)));
    connect(m_serverInterface, SIGNAL(releaseListUpdated(QList<Release>)), this, SLOT(onReleaseListUpdated(QList<Release>)));
    ui->statusBar->showMessage(tr("Getting Session ID from Nokia servers"));
    m_serverInterface->getSid();

    connect(ui->productListView, SIGNAL(clicked(QModelIndex)), this, SLOT(onProductClicked(QModelIndex)));
    connect(ui->releaseListView, SIGNAL(clicked(QModelIndex)), this, SLOT(onReleaseClicked(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onGotSid(QString)
{
    ui->statusBar->showMessage(tr("Got Session ID, fetching products list..."));
    m_serverInterface->getProductList();
}

void MainWindow::onProductListUpdated(QList<Product> listOfProducts)
{
    ProductListModel *listModel = new ProductListModel(this);
    listModel->setSourceData(listOfProducts);

    ui->productListView->setModel(listModel);

    ui->statusBar->showMessage(tr("Successfully fetched products list!"), 5000);
}

void MainWindow::onProductClicked(QModelIndex index)
{
    ui->releaseListView->setModel(0);
    ui->statusBar->showMessage(tr("Fetching releases for %1").arg(ui->productListView->model()->data(index).toString()));
    m_serverInterface->getReleasesList(ui->productListView->model()->data(index, Qt::UserRole).value<long>());
}

void MainWindow::onReleaseListUpdated(QList<Release> listOfReleases)
{
    ReleaseListModel *listModel = new ReleaseListModel(this);
    listModel->setSourceData(listOfReleases);

    ui->releaseListView->setModel(listModel);

    ui->statusBar->showMessage(tr("Successfully fetched releases list!"), 5000);
}

void MainWindow::onReleaseClicked(QModelIndex index)
{
    Q_UNUSED(index);
}
