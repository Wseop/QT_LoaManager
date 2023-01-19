#include "bracelet_widget.h"
#include "ui_bracelet_widget.h"
#include "game_data/character/item/bracelet.h"
#include "ui/widget_manager.h"
#include "ui/font_manager.h"
#include <QLabel>
#include <QNetworkAccessManager>

BraceletWidget::BraceletWidget(QWidget* pParent, const Bracelet* pBracelet) :
    QWidget(pParent),
    ui(new Ui::BraceletWidget),
    m_pBracelet(pBracelet),
    m_pNetworkManager(new QNetworkAccessManager)
{
    ui->setupUi(this);

    addIcon();
    addLabels();
    setFonts();
}

BraceletWidget::~BraceletWidget()
{
    for (QLabel* pLabel : m_labels)
        delete pLabel;
    delete m_pNetworkManager;
    delete ui;
}

void BraceletWidget::addIcon()
{
    QLabel* pIcon = WidgetManager::createIcon(m_pBracelet->getIconPath(), m_pNetworkManager, this);
    m_labels.append(pIcon);
    ui->vLayoutIcon->addWidget(pIcon);
}

void BraceletWidget::addLabels()
{
    QLabel* pLabelName = WidgetManager::createLabel(m_pBracelet->getName(), LABEL_WIDTH, LABEL_HEIGHT, 10, this, colorCode(m_pBracelet->getGrade()));
    m_labels.append(pLabelName);
    ui->vLayoutInfo->addWidget(pLabelName);

    // 일반효과 - 2개씩 묶어서 추가
    const QStringList& effects = m_pBracelet->getEffects();
    QString effectText;
    for (int i = 0; i < effects.size(); i++)
    {
        if (i & 1)
        {
            effectText += effects[i];
            QLabel* pLabelEffect = WidgetManager::createLabel(effectText, LABEL_WIDTH, LABEL_HEIGHT, 10, this);
            m_labels.append(pLabelEffect);
            ui->vLayoutInfo->addWidget(pLabelEffect);
            effectText = "";
        }
        else
        {
            effectText += QString("%1, ").arg(effects[i]);
        }
    }
    if (effectText != "")
    {
        effectText.chop(2);
        QLabel* pLabelEffect = WidgetManager::createLabel(effectText, LABEL_WIDTH, LABEL_HEIGHT, 10, this);
        m_labels.append(pLabelEffect);
        ui->vLayoutInfo->addWidget(pLabelEffect);
    }

    // [특수효과] - 가로로 이어붙임
    const QStringList& specialEffects = m_pBracelet->getSpecialEffects();
    QString specialEffectText = "";
    for (const QString& effect : specialEffects)
    {
        specialEffectText += QString("%1 ").arg(effect);
    }
    if (specialEffectText != "")
    {
        specialEffectText.chop(1);
        QLabel* pLabelEffect = WidgetManager::createLabel(specialEffectText, LABEL_WIDTH, LABEL_HEIGHT, 10, this);
        m_labels.append(pLabelEffect);
        ui->vLayoutInfo->addWidget(pLabelEffect);
    }
}

void BraceletWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);

    ui->groupBracelet->setFont(nanumBold10);
}
