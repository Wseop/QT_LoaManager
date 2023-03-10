#ifndef CHARACTER_H
#define CHARACTER_H

#include "game/character/profile.h"
#include "game/item/weapon.h"
#include "game/item/armor.h"
#include "game/item/accessory.h"
#include "game/item/abilitystone.h"
#include "game/item/bracelet.h"
#include "game/engrave/engrave.h"
#include "game/character/card.h"
#include "game/item/gem.h"
#include "game/skill/skill.h"

#include <QList>

class Character
{
public:
    Character();
    ~Character();

    QList<Profile *> siblings() const;
    void addSibling(Profile *pSibling);

    Profile *getProfile() const;
    void setProfile(Profile *pProfile);

    Weapon *getWeapon() const;
    void setWeapon(Weapon *pWeapon);

    Armor *getArmor(ArmorPart part) const;
    void setArmor(Armor *pArmor);

    Accessory *getAccessory(AccessoryPart part, int index) const;
    void setAccessory(Accessory *pAccessory, int index);

    AbilityStone *getAbilityStone() const;
    void setAbilityStone(AbilityStone *pAbilityStone);

    Bracelet *getBracelet() const;
    void setBracelet(Bracelet *pBracelet);

    Engrave *getEngrave() const;
    void setEngrave(Engrave *pEngrave);

    Card *getCard() const;
    void setCard(Card *pCard);

    QList<Gem *> gems() const;
    void addGem(Gem *pGem);

    QList<Skill *> skills() const;
    void addSkill(Skill *pSkill);

private:
    // 보유 캐릭터
    QList<Profile *> mSiblings;
    // 프로필
    Profile *mpProfile;
    // 착용 아이템
    Weapon *mpWeapon;
    QList<Armor *> mArmors;
    QList<Accessory *> mAccessories;
    AbilityStone *mpAbilityStone;
    Bracelet *mpBracelet;
    QList<Gem *> mGems;
    // 장착 각인
    Engrave *mpEngrave;
    // 카드 효과
    Card *mpCard;
    // 스킬
    QList<Skill *> mSkills;
};

#endif // CHARACTER_H
