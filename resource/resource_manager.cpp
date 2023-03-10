#include "resource_manager.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

ResourceManager *ResourceManager::mpInstance = nullptr;

ResourceManager::ResourceManager()
{
    initializeIconPath();
}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::initializeIconPath()
{
    mIconPath["어빌리티 스톤"] = ":/image/item/abilitystone/0.png";
    mIconPath["각인서"] = ":/image/item/book/0.png";
    mIconPath["보석"] = ":/image/item/gem/0.png";
    mIconPath["실링"] = ":/image/item/money/0.png";
    mIconPath["골드"] = ":/image/item/money/1.png";
    mIconPath["선명한 지혜의 기운"] = ":/image/item/vigor/0.png";
    mIconPath["빛나는 지혜의 기운"] = ":/image/item/vigor/1.png";

    QString reforgeIconPath = ":/image/item/reforge/%1_%2.png";
    QJsonArray reforges = loadJson("reforge").find("Reforge")->toArray();
    for (const QJsonValue& value : reforges)
    {
        const QJsonObject& reforge = value.toObject();
        int id1 = reforge.find("Id")->toInt();
        const QJsonArray& items = reforge.find("Items")->toArray();

        for (const QJsonValue& value : items)
        {
            const QJsonObject& item = value.toObject();
            const QString& name = item.find("Name")->toString();
            int id2 = item.find("Id")->toInt();

            mIconPath[name] = reforgeIconPath.arg(id1).arg(id2);
        }
    }
}

ResourceManager *ResourceManager::getInstance()
{
    if (mpInstance == nullptr)
        mpInstance = new ResourceManager();

    return mpInstance;
}

void ResourceManager::destroyInstance()
{
    if (mpInstance == nullptr)
        return;

    delete mpInstance;
    mpInstance = nullptr;
}

QJsonObject ResourceManager::loadJson(QString fileName)
{
    const QString filePath = ":/json/%1.json";

    QFile file(filePath.arg(fileName));
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << Q_FUNC_INFO << fileName << " Open fail";
        return QJsonObject();
    }

    QJsonObject json = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    return json;
}

QString ResourceManager::iconPath(QString item)
{
    if (!mIconPath.contains(item))
        return "";
    else
        return mIconPath[item];
}
