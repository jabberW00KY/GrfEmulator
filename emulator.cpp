#include "emulator.h"
#include "ui_emulator.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>

#define __tr(S)(QTextCodec::codecForName("Windows-1251")->toUnicode(S))

Emulator::Emulator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Emulator)
{
    ui->setupUi(this);

    io = new DataIO();
    connect(io, SIGNAL(ZapisRead(Zapis&)), this, SLOT(zapisReaded(Zapis&)));
    connect(io, SIGNAL(ZapisWritten(Zapis&)), this, SLOT(zapisWritten(Zapis&)));
}

Emulator::~Emulator()
{
    delete ui;
}

void Emulator::on_actSozdFile_triggered()
{
    if(isFileEdited() && QMessageBox::question(0, __tr("Сохранить файл"),
            __tr("Сохранить предыдущий\nфайл перед созданием нового?"), __tr("Да"), __tr("Нет")) ==
            QMessageBox::Accepted){
        unsetEditedInTitle();
        io->Save();
    }

    createNewFile();
    setEditedInTitle();
}

void Emulator::setEditedInTitle(){
    if(!this->windowTitle().contains("*"))
        this->setWindowTitle("*" + this->windowTitle());
}

void Emulator::unsetEditedInTitle(){
    if(!this->windowTitle().contains("*"))
        this->setWindowTitle(this->windowTitle().remove("*"));
}

bool Emulator::isFileEdited(){
    return *this->windowTitle().begin() == '*';
}

void Emulator::createNewFile(){
    QFileDialog *fileDialog = new QFileDialog(0, __tr("Создать новый файл"),
                                              QApplication::applicationDirPath());

    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setNameFilter(__tr("Файлы отгрузки(*.grf)"));

    if(fileDialog->exec() == QDialog::Accepted){
        io->SetFileName(fileDialog->selectedFiles().first());
    }

    fileDialog->deleteLater();
}

void Emulator::zapisReaded(Zapis &z){

}

void Emulator::zapisWritten(Zapis &z){

}
