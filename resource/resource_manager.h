#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QObject>
#include <QHash>

class ResourceManager : public QObject
{
    Q_OBJECT

private:
    ResourceManager();
    ~ResourceManager();

    void initializeIconPath();

public:
    static ResourceManager *getInstance();
    static void destroyInstance();

public:
    QJsonObject loadJson(QString fileName);
    QString iconPath(QString item);

private:
    static ResourceManager *mpInstance;

private:
    QHash<QString, QString> mIconPath;
};

#endif // RESOURCEMANAGER_H
