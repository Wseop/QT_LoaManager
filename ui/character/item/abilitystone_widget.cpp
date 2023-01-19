#include "abilitystone_widget.h"
#include "ui_abilitystone_widget.h"
#include "game_data/character/item/abilitystone.h"
#include "ui/widget_manager.h"
#include "ui/font_manager.h"
#include <QLabel>
#include <QNetworkAccessManager>

AbilityStoneWidget::AbilityStoneWidget(QWidget* pParent, const AbilityStone* pAbilityStone) :
    QWidget(pParent),
    ui(new Ui::AbilityStoneWidget),
    m_pAbilityStone(pAbilityStone),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);

    addIcon();
    addLabels();
    setFonts();
}

AbilityStoneWidget::~AbilityStoneWidget()
{
    for (QLabel* pLabel : m_labels)
        delete pLabel;
    delete m_pNetworkManager;
    delete ui;
}

void AbilityStoneWidget::addIcon()
{
    QLabel* pIcon = WidgetManager::createIcon(m_pAbilityStone->getIconPath(), m_pNetworkManager, ICON_WIDTH, ICON_HEIGHT, this);
    m_labels.append(pIcon);
    ui->vLayoutIcon->addWidget(pIcon);
}

void AbilityStoneWidget::addLabels()
{
    QLabel* pNameLabel = WidgetManager::createLabel(m_pAbilityStone->getName(), LABEL_WIDTH, LABEL_HEIGHT, 10, this, colorCode(m_pAbilityStone->getGrade()));
    m_labels.append(pNameLabel);
    ui->vLayoutInfo->addWidget(pNameLabel);

    const QList<PairEngraveValue>& engraves = m_pAbilityStone->getEngraves();
    for (const PairEngraveValue& pairEngraveValue : engraves)
    {
        QString labelText = QString("[%1] +%2").arg(pairEngraveValue.first).arg(pairEngraveValue.second);
        QLabel* pEngraveLabel = WidgetManager::createLabel(labelText, LABEL_WIDTH, LABEL_HEIGHT, 10, this);
        m_labels.append(pEngraveLabel);
        ui->vLayoutInfo->addWidget(pEngraveLabel);
    }

    const QList<PairEngraveValue>& penalties = m_pAbilityStone->getPenalties();
    for (const PairEngraveValue& pairEngraveValue : penalties)
    {
        QString labelText = QString("[%1] +%2").arg(pairEngraveValue.first).arg(pairEngraveValue.second);
        QLabel* pPenaltyLabel = WidgetManager::createLabel(labelText, LABEL_WIDTH, LABEL_HEIGHT, 10, this, "red");
        m_labels.append(pPenaltyLabel);
        ui->vLayoutInfo->addWidget(pPenaltyLabel);
    }
}

void AbilityStoneWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);

    ui->groupAbilityStone->setFont(nanumBold10);
}
