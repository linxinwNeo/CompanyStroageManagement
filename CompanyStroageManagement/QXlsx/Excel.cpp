#include "QXlsx/Excel.h"
#include "GlobalVars.h"
#include <QMessageBox>
#include <QVector>
#include <QTime>
#include <QDateTime>

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
            xlsx = new QXlsx::Document(this->fpath);
            if(xlsx->load()){
                return; // exit when we got the file
            }
            QMessageBox Msgbox(nullptr);
            Msgbox.setText(FATAL_ERROR_MESSAGE);
            Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
            Msgbox.show();
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
    if(this->xlsx != nullptr) delete this->xlsx;
}

void Excel::read_excel()
{
    using namespace QXlsx;

    if(this->fpath.isEmpty()){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
    }

    if(this->xlsx == nullptr) xlsx = new Document(this->fpath);

    if(!xlsx->load()){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        exit(1);
    }

    QStringList sheetNames = xlsx->sheetNames();
    const UI num_sheets = xlsx->sheetNames().size();
    qDebug() << sheetNames;
    for(UI i = 0; i < num_sheets; i++){
        const QString cur_sheet_name = sheetNames[i];
        qDebug() << "Reading worksheet:" << cur_sheet_name;
        xlsx->selectSheet(cur_sheet_name); // switch sheet
        Worksheet* cur_sheet = xlsx->currentWorksheet();
        if( cur_sheet == nullptr || cur_sheet->isHidden() || cur_sheet->sheetType() != AbstractSheet::SheetType::ST_WorkSheet ) continue;
        int maxRow = cur_sheet->dimension().lastRow();
        int maxCol = -1;
        int col_of_MODELO = -1;
        int row_of_VENTAS = -1;

        // find VENTA's position
        for(UI j = 1; j < 100; j++){ // row
            if(xlsx->isRowHidden(j)) continue;
            for(UI i = 1; i < 10; i++){ // col
                if(xlsx->isColumnHidden(i)) continue;

                Cell* cell = xlsx->cellAt(j, i);
                if(cell == nullptr) continue;
                QVariant var = cell->value();
                QString str = var.toString();
                if(str.contains("VENTAS")){
                    maxCol = i+1;
                    row_of_VENTAS = j;
                }
            }
        }
    }
}

void Excel::set_value(const QString sheetName, const UI row, const UI col, const QString new_val)
{
    using namespace QXlsx;

    if(!xlsx->load()){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        Msgbox.exec();
        exit(0);
    }

    // switch to the correct sheet
    xlsx->selectSheet(sheetName); // switch sheet
    Worksheet* cur_sheet = xlsx->currentWorksheet();
    if(cur_sheet == nullptr || cur_sheet->sheetName() != sheetName){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        Msgbox.exec();
        exit(0);
    }

    CellReference c(row, col);
    QVariant val(new_val);
    bool b = xlsx->write(c, val);
    if(b == false){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        Msgbox.exec();
        exit(0);
    }
}

void Excel::save_excel() {
    using namespace QXlsx;

    QMessageBox Msgbox(nullptr);

    if(!xlsx->load()){
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        Msgbox.exec();
        exit(-1);
    }

    QTime t = t.currentTime();
    bool b = xlsx->save();
    qDebug() << "Saving excel takes" << t.msecsTo(t.currentTime())/1000. << "sec\n";

    if(b == false){
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        Msgbox.exec();
        exit(-2);
    }

}

void Excel::reset_path()
{
    QFile file("./last_opened_xlsx.txt");
    file.remove();
    this->fpath = QString();
}
