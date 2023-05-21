#include "Excel_IO/Excel.h"
#include "GlobalVars.h"
#include <QMessageBox>

// default constructor
// check if previous excel exists in last_opened_xlsx.txt
Excel::Excel()
{
    QMessageBox Msgbox(nullptr);
    Msgbox.setText(EXCEL_SELECT_A_VALID_XLSX_FILE);
    Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");

    // save the full path to the resource file as well
    QFile file_read("./last_opened_xlsx.txt");
    if (file_read.open(QIODevice::ReadOnly)){
        QTextStream in(&file_read);
        QString line;
        while(!in.atEnd()){
            line = in.readLine();
            if(!line.isEmpty()) break;
        }
        file_read.close();

        if(!line.isEmpty()){
            this->fpath = line;
            return; // exit when we got the file
        }

        // the file is empty, ask the user for the input excel file
    }

    qDebug("last_opened_xlsx.txt does not exist, ask user for the file");
    QFile file_write("./last_opened_xlsx.txt");
    // the user has to provide a file in order to continue
    QString file_full_path= QFileDialog::getOpenFileName(nullptr, EXCEL_CAPTION, EXCEL_DEFAULT_DIR, EXCEL_FILTER);
    if(file_full_path.isEmpty()){
        Msgbox.exec(); // if not getting file, exit
        exit(0);
    }

    if (!file_write.open(QIODevice::WriteOnly)){
        qDebug("not able to open the file");
        return;
    }

    QTextStream out(&file_write);
    out << file_full_path;
    this->fpath = file_full_path;
    file_write.close();
    return;
}

Excel::Excel(const QString full_path_to_Excel)
{
    this->fpath = full_path_to_Excel;
}


Excel::~Excel()
{

}

void Excel::read_excel()
{
    if(this->fpath.isEmpty()){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
    }

    int row = 1; int col = 1;
    QXlsx::Document xlsxR(this->fpath);
    if(!xlsxR.load()){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        exit(0);
    }

    QStringList sheetNames = xlsxR.sheetNames();
    const UI num_sheets = xlsxR.sheetNames().size();
    for(UI i = 0; i < num_sheets; i++){
        const QString cur_sheet_name = sheetNames[i];
        qDebug() << cur_sheet_name;
        xlsxR.selectSheet(cur_sheet_name); // switch sheet
        const QXlsx::Worksheet* cur_sheet = xlsxR.currentWorksheet();
        // find the cell that contains WORD "MODELO"
    }
}
