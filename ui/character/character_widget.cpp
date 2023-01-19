#include "character_widget.h"
#include "ui_character_widget.h"
#include "game_data/character/character.h"
#include "game_data/character/profile/profile.h"
#include "game_data/character/card/card.h"
#include "game_data/character/item/equip.h"
#include "game_data/character/item/accessory.h"
#include "game_data/character/item/abilitystone.h"
#include "game_data/character/item/bracelet.h"
#include "game_data/character/engrave/engrave.h"
#include "ui/widget_manager.h"
#include "ui/character/item/equip_widget.h"
#include "ui/character/item/accessory_widget.h"
#include "ui/character/item/abilitystone_widget.h"
#include "ui/character/item/bracelet_widget.h"
#include "ui/character/engrave/engrave_widget.h"
#include "ui/font_manager.h"

CharacterWidget::CharacterWidget(QWidget* pParent, const Character* pCharacter) :
    QWidget(pParent),
    ui(new Ui::CharacterWidget),
    m_pCharacter(pCharacter),
    m_pAbilityStoneWidget(nullptr),
    m_pBraceletWidget(nullptr),
    m_pEngraveWidget(nullptr)
{
    ui->setupUi(this);
    ui->vLayoutCharacter->setAlignment(Qt::AlignHCenter);

    setFonts();
    setStyleSheets();
    addProfileWidget();
    addEquipWidgets();
    addAccessoryWidgets();
    addAbilityStoneWidget();
    addBraceletWidget();
    addEngraveWidget();
}

CharacterWidget::~CharacterWidget()
{
    for (QLabel* pLabel : m_labels)
        delete pLabel;
    for (EquipWidget* pEquipWidget : m_equipWidgets)
        delete pEquipWidget;
    for (AccessoryWidget* pAccessoryWidget : m_accessoryWidgets)
        delete pAccessoryWidget;
    if (m_pAbilityStoneWidget != nullptr)
        delete m_pAbilityStoneWidget;
    if (m_pBraceletWidget != nullptr)
        delete m_pBraceletWidget;
    if (m_pEngraveWidget != nullptr)
        delete m_pEngraveWidget;
    delete ui;
}

void CharacterWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumBold12 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 12);

    ui->pbSibling->setFont(nanumBold10);
    ui->lbName->setFont(nanumBold10);
    ui->groupTitleGuild->setFont(nanumBold10);
    ui->groupServerClass->setFont(nanumBold10);
    ui->lbBattleLevel->setFont(nanumBold10);
    ui->lbExpLevel->setFont(nanumBold10);
    ui->lbItemLevel->setFont(nanumBold10);
    ui->groupAbility->setFont(nanumBold10);
    ui->groupCard->setFont(nanumBold10);
}

void CharacterWidget::setStyleSheets()
{
    QString labelColor = "QLabel { color: %1 }";

    ui->lbGuild->setStyleSheet(labelColor.arg(COLOR_GUILD));
    ui->lbServer->setStyleSheet(labelColor.arg(COLOR_SERVER));
    ui->lbItemLevel->setStyleSheet(labelColor.arg(COLOR_ITEMLEVEL));
}

void CharacterWidget::addProfileWidget()
{
    const Profile* pProfile = m_pCharacter->getProfile();
    ui->lbName->setText(pProfile->getCharacterName());
    ui->lbTitle->setText(pProfile->getTitle());
    ui->lbGuild->setText(pProfile->getGuild());
    ui->lbServer->setText("@" + pProfile->getServer());
    ui->lbClass->setText(classToStr(pProfile->getClass()));
    ui->lbBattleLevel->setText(QString("전투 Lv.%1").arg(pProfile->getCharacterLevel()));
    ui->lbExpLevel->setText(QString("원정대 Lv.%1").arg(pProfile->getExpLevel()));
    ui->lbItemLevel->setText(QString("아이템 Lv.%1").arg(pProfile->getItemLevel()));

    const QMap<Ability, int>& abilities = pProfile->getAbilities();
    ui->lbCritical->setText(abilityToStr(Ability::치명) + " " + QString::number(abilities[Ability::치명]));
    ui->lbSpecification->setText(abilityToStr(Ability::특화) + " " + QString::number(abilities[Ability::특화]));
    ui->lbSwiftness->setText(abilityToStr(Ability::신속) + " " + QString::number(abilities[Ability::신속]));
    ui->lbDomination->setText(abilityToStr(Ability::제압) + " " + QString::number(abilities[Ability::제압]));
    ui->lbEndurance->setText(abilityToStr(Ability::인내) + " " + QString::number(abilities[Ability::인내]));
    ui->lbExpertise->setText(abilityToStr(Ability::숙련) + " " + QString::number(abilities[Ability::숙련]));

    const QList<Card*>& cards = m_pCharacter->getCards();
    for (const Card* pCard : cards)
    {
        // 최종완성단계 효과만 추가
        const QString& cardEffectName = pCard->getEffectNames().back();
        QString cardEffectText;

        // 카드효과 (+ 각성합계) 문자열 생성
        if (cardEffectName.contains("각성합계"))
        {
            // 각성합계 제외, 카드효과만 추출
            int nameSize = cardEffectName.indexOf("(") - 1;
            cardEffectText = cardEffectName.sliced(0, nameSize);
            // 각성합계 수치 추가
            nameSize += 2;
            int awaken = cardEffectName.sliced(nameSize, cardEffectName.indexOf("각성합계") - nameSize).toInt();
            cardEffectText += QString(" (%1각)").arg(awaken);
        }
        else
        {
            cardEffectText = cardEffectName;
        }

        QLabel* pCardLabel = WidgetManager::createLabel(cardEffectText, 300, 25, 10, this);
        m_labels.append(pCardLabel);
        ui->vLayoutCard->addWidget(pCardLabel);
    }
}

void CharacterWidget::addEquipWidgets()
{
    for (int i = 0; i < 6; i++) {
        const Equip* pEquip = m_pCharacter->getEquip(static_cast<ItemType>(i));
        if (pEquip == nullptr)
            continue;

        EquipWidget* pEquipWidget = new EquipWidget(this, pEquip);
        m_equipWidgets.append(pEquipWidget);
        ui->hLayoutEquip->addWidget(pEquipWidget);
    }
}

void CharacterWidget::addAccessoryWidgets()
{
    const Accessory* pNeck = m_pCharacter->getAccessory(ItemType::목걸이);
    if (pNeck != nullptr)
    {
        AccessoryWidget* pAccessoryWidget = new AccessoryWidget(this, pNeck);
        m_accessoryWidgets.append(pAccessoryWidget);
        ui->hLayoutAccessory->addWidget(pAccessoryWidget);
    }

    for (int i = 0; i < 2; i++)
    {
        const Accessory* pEar = m_pCharacter->getAccessory(ItemType::귀걸이, i);
        if (pEar != nullptr)
        {
            AccessoryWidget* pAccessoryWidget = new AccessoryWidget(this, pEar);
            m_accessoryWidgets.append(pAccessoryWidget);
            ui->hLayoutAccessory->addWidget(pAccessoryWidget);
        }

        const Accessory* pRing = m_pCharacter->getAccessory(ItemType::반지, i);
        if (pRing != nullptr)
        {
            AccessoryWidget* pAccessoryWidget = new AccessoryWidget(this, pRing);
            m_accessoryWidgets.append(pAccessoryWidget);
            ui->hLayoutAccessory->addWidget(pAccessoryWidget);
        }
    }
}

void CharacterWidget::addAbilityStoneWidget()
{
    const AbilityStone* pAbilityStone = m_pCharacter->getAbilityStone();
    if (pAbilityStone != nullptr)
    {
        m_pAbilityStoneWidget = new AbilityStoneWidget(this, pAbilityStone);
        ui->hLayoutStoneBraceletEngrave->addWidget(m_pAbilityStoneWidget);
    }
}

void CharacterWidget::addBraceletWidget()
{
    const Bracelet* pBracelet = m_pCharacter->getBracelet();
    if (pBracelet != nullptr)
    {
        m_pBraceletWidget = new BraceletWidget(this, pBracelet);
        ui->hLayoutStoneBraceletEngrave->addWidget(m_pBraceletWidget);
    }
}

void CharacterWidget::addEngraveWidget()
{
    const Engrave* pEngrave = m_pCharacter->getEngrave();
    if (pEngrave != nullptr)
    {
        m_pEngraveWidget = new EngraveWidget(this, pEngrave);
        ui->hLayoutStoneBraceletEngrave->addWidget(m_pEngraveWidget);
    }
}
