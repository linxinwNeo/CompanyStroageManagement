#ifndef SEARCH_LIST_WIN_H
#define SEARCH_LIST_WIN_H

#include <QWidget>

namespace Ui {
class Search_List_Win;
}

class Search_List_Win : public QWidget
{
    Q_OBJECT

public:
    explicit Search_List_Win(QWidget *parent = nullptr);
    ~Search_List_Win();

private:
    Ui::Search_List_Win *ui;
};

#endif // SEARCH_LIST_WIN_H
