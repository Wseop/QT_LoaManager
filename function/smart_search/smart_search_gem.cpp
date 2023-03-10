#include "smart_search_gem.h"
#include "ui_smart_search_gem.h"
#include "ui/widget_manager.h"
#include "api/api_manager.h"
#include "api/search_option.h"
#include "game/item/gem.h"

#include <QLabel>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>

SmartSearchGem::SmartSearchGem(QLayout *pLayout) :
    ui(new Ui::SmartSearchGem),
    mSearchList({
        {"5레벨 멸화", "6레벨 멸화", "7레벨 멸화", "8레벨 멸화", "9레벨 멸화", "10레벨 멸화"},
        {"5레벨 홍염", "6레벨 홍염", "7레벨 홍염", "8레벨 홍염", "9레벨 홍염", "10레벨 홍염"}
    })
{
    ui->setupUi(this);

    pLayout->addWidget(this);
    this->hide();

    initializeUI();
}

SmartSearchGem::~SmartSearchGem()
{
    for (QWidget* pWidget : mWidgets)
        delete pWidget;
    clearUI();
    delete ui;
}

void SmartSearchGem::refresh()
{
    clearUI();

    for (int i = 0; i < mSearchList.size(); i++)
    {
        for (int j = 0; j < mSearchList[i].size(); j++)
        {
            QNetworkAccessManager *pNetworkManager = new QNetworkAccessManager();
            connect(pNetworkManager, &QNetworkAccessManager::finished, this, &SmartSearchGem::parseSearchResult);
            connect(pNetworkManager, &QNetworkAccessManager::finished, pNetworkManager, &QNetworkAccessManager::deleteLater);

            // 보석 가격 검색
            SearchOption searchOption(SearchType::Auction);
            searchOption.setCategoryCode(CategoryCode::Gem);
            searchOption.setItemName(mSearchList[i][j]);
            searchOption.setItemTier(3);
            searchOption.setPageNo(1);
            searchOption.setSortCondition("ASC");

            ApiManager::getInstance()->post(pNetworkManager, LostarkApi::Auction, QJsonDocument(searchOption.toJsonObject()).toJson());
        }
    }
}

void SmartSearchGem::initializeUI()
{
    const QStringList attributes[2] = {{"#", "멸화", "즉시 구매가\n(최소 입찰가)"}, {"#", "홍염", "즉시 구매가\n(최소 입찰가)"}};
    const QList<QGridLayout*> layouts = {ui->gridLeft, ui->gridRight};

    for (int i = 0; i < layouts.size(); i++)
    {
        // attribute label 추가
        for (int j = 0; j < attributes[i].size(); j++)
        {
            QLabel *pLabel = WidgetManager::createLabel(attributes[i][j], 14, "", 200, 50);
            layouts[i]->addWidget(pLabel, 0, j, Qt::AlignHCenter);
            mWidgets.append(pLabel);
        }
    }
}

void SmartSearchGem::updateUI(const Gem gem, Price price)
{
    QGridLayout *pLayout = gem.gemType() == GemType::멸화 ? ui->gridLeft : ui->gridRight;
    int row = (2 * gem.gemLevel()) - 9;

    QNetworkAccessManager *pIconLoader = new QNetworkAccessManager();
    connect(pIconLoader, &QNetworkAccessManager::finished, pIconLoader, &QNetworkAccessManager::deleteLater);

    QFrame *pHLine = WidgetManager::createLine(QFrame::HLine);
    pLayout->addWidget(pHLine, row++, 0, 1, -1);
    mGemWidgets.append(pHLine);

    QLabel *pIcon = WidgetManager::createIcon(gem.iconPath(), pIconLoader, itemGradeToBGColor(gem.itemGrade()));
    pLayout->addWidget(pIcon, row, 0, Qt::AlignHCenter);
    mGemWidgets.append(pIcon);

    QLabel *pLabelName = WidgetManager::createLabel(gem.itemName(), 10, itemGradeToTextColor(gem.itemGrade()), 300);
    pLayout->addWidget(pLabelName, row, 1, Qt::AlignHCenter);
    mGemWidgets.append(pLabelName);

    QLabel *pLabelPrice = WidgetManager::createLabel(QString("%L1\n(%L2)").arg(price.buyPrice).arg(price.bidStartPrice), 10, "", 300, 50);
    pLayout->addWidget(pLabelPrice, row, 2, Qt::AlignHCenter);
    mGemWidgets.append(pLabelPrice);
}

void SmartSearchGem::clearUI()
{
    for (QWidget* pWidget : mGemWidgets)
        delete pWidget;

    mGemWidgets.clear();
}

void SmartSearchGem::parseSearchResult(QNetworkReply *pReply)
{
    // 검색 결과 parsing
    QJsonDocument response = QJsonDocument::fromJson(pReply->readAll());
    if (response.isNull())
        return;

    const QJsonObject &item = response.object().find("Items")->toArray()[0].toObject();
    const QString &itemName = item.find("Name")->toString();

    // 보석 타입 설정
    GemType gemType;
    if (itemName.contains("멸화"))
        gemType = GemType::멸화;
    else
        gemType = GemType::홍염;

    Gem gem(gemType);
    gem.setItemName(itemName);
    gem.setItemGrade(qStringToItemGrade(item.find("Grade")->toString()));
    gem.setIconPath(item.find("Icon")->toString());

    // 보석 레벨 설정
    if (itemName.startsWith("10"))
        gem.setGemLevel(10);
    else
        gem.setGemLevel(itemName[0].digitValue());

    // 가격 parsing
    const QJsonObject &auctionInfo = item.find("AuctionInfo")->toObject();
    int buyPrice = auctionInfo.find("BuyPrice")->toInt();
    int bidStartPrice = auctionInfo.find("BidStartPrice")->toInt();

    updateUI(gem, {buyPrice, bidStartPrice});
}
