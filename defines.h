#ifndef DEFINES_H
#define DEFINES_H

#include <QString>
#include <QList>

struct NokiaFile {
    QString fileName;
    long fileSize;
    QString downloadURL;
};

struct Product {
    QString productId;
    QString productType;
    QString marketName;
    QByteArray image;
    QString corePlatform;
    bool turnOnBeforeFactorySet;
    QString supportedFlashSequences;
    QString PPMFilePolicy;
    int contentFilePolicy;
    int variantFilePolicy;
    int userDataBackupPolicy;
    QList<NokiaFile> fileList;
};

struct Release {
    QString releaseId;
    QString version;
};

#endif // DEFINES_H
