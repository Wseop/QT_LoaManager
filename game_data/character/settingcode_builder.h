#ifndef SETTINGCODEBUILDER_H
#define SETTINGCODEBUILDER_H

#include "game_data/character/profile/enum/ability.h"
#include "game_data/character/item/enum/set_effect.h"
#include "game_data/character/engrave/engrave.h"
#include <QObject>

class SettingcodeBuilder : public QObject
{
    Q_OBJECT

public:
    static QString buildSettingCode(QList<Ability> abilities, QList<SetEffect> setEffects, QList<PairEngraveValue> engraves);
    static QList<Ability> parseAbility(QString settingCode);
    static QList<SetEffect> parseSetEffect(QString settingCode);
    static QList<QList<PairEngraveValue>> parseClassEngrave(QString settingCode);
    static QList<QList<PairEngraveValue>> parseNormalEngrave(QString settingCode);

private:
    static QString buildAbilityCode(const QList<Ability>& abilities);
    static QString buildSetEffectCode(const QList<SetEffect>& setEffects);
    static QString buildEngraveCode(const QList<PairEngraveValue>& engraves);
};

#endif // SETTINGCODEBUILDER_H