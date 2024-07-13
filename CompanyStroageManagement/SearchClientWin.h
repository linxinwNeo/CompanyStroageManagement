#ifndef SEARCHCLIENTWIN_H
#define SEARCHCLIENTWIN_H

#include <QWidget>

namespace Ui {
class SearchClientWin;
}

class SearchClientWin : public QWidget
{
    Q_OBJECT

public:
    explicit SearchClientWin(QWidget *parent = nullptr);
    ~SearchClientWin();

    void set_GUI_Language();
    void setWindow();

    void clear_table();

private slots:
    void on_pushButton_SearchByIDPrefix_clicked();

    void on_pushButton_SearchByNamePrefix_clicked();

    void on_pushButton_deleteSelectedClient_clicked();

private:
    Ui::SearchClientWin *ui;

    const float widthRatio = 0.1;
    const float heightRatio = 0.4;
};

#endif // SEARCHCLIENTWIN_H
