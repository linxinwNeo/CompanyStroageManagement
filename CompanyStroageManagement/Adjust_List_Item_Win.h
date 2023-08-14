#ifndef ADJUST_LIST_ITEM_WIN_H
#define ADJUST_LIST_ITEM_WIN_H

#include <QWidget>

namespace Ui {
class Adjust_List_Item_Win;
}

class Adjust_List_Item_Win : public QWidget
{
    Q_OBJECT

public:
    explicit Adjust_List_Item_Win(QWidget *parent = nullptr);
    ~Adjust_List_Item_Win();

    void set_values(const double MAX_NUM_BOXES,
                    const long unsigned int MAX_NUM_ITEMS,
                    const long unsigned int NUM_ITEMS_PER_BOX,
                    const QString MODEL_CODE, const QString Description,
                    const double PRIZE, const double TOTAL);

private slots:
    void on_finish_btn_clicked();

private:
    QWidget* parent_win = nullptr;
    Ui::Adjust_List_Item_Win *ui;

    double MAX_NUM_BOXES;
    long unsigned int MAX_NUM_ITEMS;

    double NUM_BOXES;
    long unsigned int NUM_ITEMS;

    long unsigned int NUM_ITEMS_PER_BOX;

    QString MODEL_CODE;

    QString DESCRIPTION;

    double PRIZE;

    double TOTAL;
};

#endif // ADJUST_LIST_ITEM_WIN_H
