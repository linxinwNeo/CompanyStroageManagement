#include "Excel_IO/Excel.h"
#include "GlobalVars.h"
#include <QMessageBox>
#include <QVector>

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
            QXlsx::Document xlsxR(this->fpath);
            if(xlsxR.load()){
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

}

void Excel::read_excel()
{
    using namespace QXlsx;

    if(this->fpath.isEmpty()){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
    }

    Document xlsxR(this->fpath);
    if(!xlsxR.load()){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        exit(1);
    }

    QStringList sheetNames = xlsxR.sheetNames();
    const UI num_sheets = xlsxR.sheetNames().size();
    qDebug() << sheetNames;
    for(UI i = 0; i < sheetNames.size(); i++){
        const QString cur_sheet_name = sheetNames[i];
        qDebug() << "Reading worksheet:" << cur_sheet_name;
        xlsxR.selectSheet(cur_sheet_name); // switch sheet
        Worksheet* cur_sheet = xlsxR.currentWorksheet();
        if( cur_sheet == nullptr || cur_sheet->isHidden() || cur_sheet->sheetType() != AbstractSheet::SheetType::ST_WorkSheet ) continue;
        int maxRow = cur_sheet->dimension().lastRow();
        int maxCol = -1;
        int col_of_MODELO = -1;
        int col_of_VENTAS = -1;
        int row_of_VENTAS = -1;

        // find VENTA's position
        for(UI j = 1; j < 100; j++){ // row
            if(xlsxR.isRowHidden(j)) continue;
            for(UI i = 1; i < 10; i++){ // col
                if(xlsxR.isColumnHidden(i)) continue;

                Cell* cell = xlsxR.cellAt(j, i);
                if(cell == nullptr) continue;
                QVariant var = cell->value();
                QString str = var.toString();
                if(str.contains("VENTAS")){
                    maxCol = i+1;
                    col_of_VENTAS = i;
                    row_of_VENTAS = j;
                }
            }
        }

        // find position of modelo
        for(UI i = 1; i < maxCol; i++){ // col
            Cell* cell = xlsxR.cellAt(row_of_VENTAS, i);
            if(cell == nullptr) continue;
            QVariant var = cell->value();
            QString str = var.toString();
            if(str.contains("MODELO")){
                col_of_MODELO = i;
                break;
            }
        }

        // error handling for not finding the modelo position
        if(col_of_MODELO == -1){
            QMessageBox Msgbox(nullptr);
            Msgbox.setText(FATAL_ERROR_MESSAGE);
            Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
            exit(-1);
        }

        // find max_row
//        bool found = false;
//        for(UI j = row_of_VENTAS+1; j < 50000; j++){ // row
//            if(xlsxR.isRowHidden(j)) continue;
//            for(UI i = 1; i < maxCol; i++){ // col
//                if(xlsxR.isColumnHidden(i)) continue;

//                Cell* cell = xlsxR.cellAt(j, col_of_MODELO);
//                if(cell == nullptr) continue;
//                QVariant var = cell->value();
//                QString str = var.toString();
//                if(str.isEmpty()) {
//                    maxRow = j;
//                    found = true;
//                    break;
//                }
//            }
//            if(found) break;
//        }

//        qDebug() << row_of_VENTAS << col_of_VENTAS << maxRow << maxCol;


        // reading models
        for(UI i = row_of_VENTAS+1; i < maxRow; i++){
            if(xlsxR.isRowHidden(i)) continue;

            Cell* cell = xlsxR.cellAt(i, col_of_MODELO);
            if(cell == nullptr) continue;
            QVariant var = cell->value();
            QString str = var.toString();
            if( str.isEmpty() ||
                    str.contains("CONT") ||
                    str.contains("PEDIMENTO") ||
                    str.contains("CONTENEDOR") ||
                    str.contains("CUBREBOCAS")) continue; // not model
            // we found model
            QString model_name = str;
            double PZSXCJA, INICIAL, EXISTENCIAS, VENTAS;
            PZSXCJA = INICIAL = EXISTENCIAS = VENTAS = 0;

            auto c = xlsxR.cellAt(i, col_of_MODELO+1);
            if(c != nullptr)  PZSXCJA = c->value().toDouble();

            c = xlsxR.cellAt(i, col_of_MODELO+2);
            if(c != nullptr)  INICIAL = c->value().toDouble();

            c = xlsxR.cellAt(i, col_of_MODELO+3);
            if(c != nullptr)  EXISTENCIAS = c->value().toDouble();

            c = xlsxR.cellAt(i, col_of_MODELO+4);
            if(c != nullptr)  VENTAS = c->value().toDouble();

            Model* model = new Model(model_name, cur_sheet_name,
                                     PZSXCJA, INICIAL, EXISTENCIAS, VENTAS,
                                     i, col_of_MODELO);
            stroage.add_model(model);
        }
    }
}

void Excel::set_value(const QString sheetName, const UI row, const UI col, const QString new_val)
{
    using namespace QXlsx;

    Document xlsxW(this->fpath);
    if(!xlsxW.load()){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        Msgbox.show();
        exit(0);
    }

    // switch to the correct sheet
    xlsxW.selectSheet(sheetName); // switch sheet
    Worksheet* cur_sheet = xlsxW.currentWorksheet();
    if(cur_sheet == nullptr || cur_sheet->sheetName() != sheetName){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        Msgbox.show();
        exit(0);
    }

    CellReference c(row, col);
    QVariant val(new_val);
    bool b = xlsxW.write(c, val);
    if(b == false){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        Msgbox.show();
        exit(0);
    }


    b = xlsxW.save();
    if(b == false){
        QMessageBox Msgbox(nullptr);
        Msgbox.setText(FATAL_ERROR_MESSAGE);
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        Msgbox.show();
        exit(0);
    }
}

void Excel::reset_path()
{
    QFile file("./last_opened_xlsx.txt");
    file.remove();
    this->fpath = QString();
}
