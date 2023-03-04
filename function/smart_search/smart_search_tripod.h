#ifndef SMART_SEARCH_TRIPOD_H
#define SMART_SEARCH_TRIPOD_H

#include "function/smart_search/smart_search_menu.h"
#include "game/skill/skill.h"

#include <QWidget>

namespace Ui {
class SmartSearchTripod;
}

class SmartSearchTripod : public QWidget, public SmartSearchMenu
{
    Q_OBJECT

public:
    SmartSearchTripod(QLayout* pLayout);
    ~SmartSearchTripod();

    void refresh() override;

private:
    void initializeClassSelector();
    void initializePriceFilter();
    void initializeSearchButton();
    void initializeResultUI();

    void clearResult();
    void searchTripod(int skillCode, int tripodCode);
    void addSkillWidget(const Skill& skill, int row);
    void addTripodWidget(const Tripod& tripod, int skillCode, int row, int col);

private:
    Ui::SmartSearchTripod *ui;

    int m_filterPrice;

    class QComboBox* m_pClassSelector;
    class QIntValidator* m_pPriceValidator;

    // search result
    QHash<int, QHash<int, class QLabel*>> m_tripodPriceLabels;
    QList<QWidget*> m_resultWidgets;
    QList<QLayout*> m_resultLayouts;

    QList<QWidget*> m_widgets;
    QList<QLayout*> m_layouts;
};

#endif // SMART_SEARCH_TRIPOD_H