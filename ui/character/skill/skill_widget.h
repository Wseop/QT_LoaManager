#ifndef SKILL_WIDGET_H
#define SKILL_WIDGET_H

#include <QWidget>

class Skill;
class QLabel;
class QNetworkAccessManager;

namespace Ui {
class SkillWidget;
}

class SkillWidget : public QWidget
{
    Q_OBJECT

public:
    SkillWidget(QWidget* pParent, const Skill* pSkill);
    ~SkillWidget();

private:
    void addIcon();
    void addLabels();

private:
    Ui::SkillWidget *ui;

    const int LABEL_WIDTH = 200;
    const int LABEL_HEIGHT = 25;

    const Skill* m_pSkill;
    QList<QLabel*> m_labels;
    QNetworkAccessManager* m_pNetworkManager;
};

#endif // SKILL_WIDGET_H