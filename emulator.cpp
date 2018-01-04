#include "emulator.h"
#include "ui_emulator.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>
#include <QTableWidgetItem>
#include  <QDateTime>

#define __tr(S)(QTextCodec::codecForName("Windows-1251")->toUnicode(S))

Emulator::Emulator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Emulator)
{
    ui->setupUi(this);

    io = new DataIO();
    // Подключаемся с сигналу успешного считывания записи из файла
    connect(io, SIGNAL(ZapisRead(Zapis&)), this, SLOT(zapisReaded(Zapis&)));
    // Подключаемся к сигналу успешной записи структуры записи в файл
    connect(io, SIGNAL(ZapisWritten(Zapis&)), this, SLOT(zapisWritten(Zapis&)));
}

Emulator::~Emulator()
{
    delete io;
    delete ui;
}

void Emulator::on_actSozdFile_triggered()
{
    // Если файл отредактирован (стоит отметка в заголовке программы с символом *)
    // и пользоватль ответил удовлетворительно на вопрос о сохранении файла
    // то сохраняем файл, затем закрываем его и снимаем ответку о редактировании файла.
    if(isFileEdited() && QMessageBox::question(0, __tr("Сохранить файл"),
            __tr("Сохранить предыдущий\nфайл перед созданием нового?"), __tr("Да"), __tr("Нет")) ==
            QMessageBox::Accepted){
        io->Save(); // Сохраняем файл и закрываем
        unsetEditedInTitle(); // Убираем отметку о редактировании файла
    }

    createNewFile(); // Создаем новый файл
    setEditedInTitle(); // Устанавливаем отметку и редактировании файла
}

/*!
 * \brief Emulator::setEditedInTitle Устанавилвает отметку о редактировании файла
 */
void Emulator::setEditedInTitle(){
    if(!this->windowTitle().contains("*"))
        this->setWindowTitle("*" + this->windowTitle());
}

/*!
 * \brief Emulator::unsetEditedInTitle Сбрасывает отметку о редактировании файла
 */
void Emulator::unsetEditedInTitle(){
    if(!this->windowTitle().contains("*"))
        this->setWindowTitle(this->windowTitle().remove("*"));
}

/*!
 * \brief Emulator::isFileEdited Проверяет отредактирован ли файл (
 * стоит ли отметка о редактировании)
 * \return True - если заголовок содержит символ *, иначе False
 */
bool Emulator::isFileEdited(){
    return *this->windowTitle().begin() == '*';
}

/*!
 * \brief Emulator::createNewFile Создает новый файл.
 * Открывается диаологовое окно о сохранении нового файла и
 * по заданному имени создается новый файл.
 */
void Emulator::createNewFile(){
    QFileDialog *fileDialog = new QFileDialog(0, __tr("Создать новый файл"),
                                              QApplication::applicationDirPath());

    fileDialog->setAcceptMode(QFileDialog::AcceptSave); // Режим сохранения файла
    fileDialog->setNameFilter(__tr("Файлы отгрузки(*.grf)")); // Фильтр файлов отгрузки (.grf)

    if(fileDialog->exec() == QDialog::Accepted)
        io->SetFileName(fileDialog->selectedFiles().first());

    fileDialog->deleteLater();
}

/*!
 * \brief Emulator::zapisReaded Слот успешного чтения записи из файла
 * \param z
 */
void Emulator::zapisReaded(Zapis &z){
    addZapisToTable(z);
}

/*!
 * \brief Emulator::zapisWritten Слот успешной записи в файл
 * \param z
 */
void Emulator::zapisWritten(Zapis &z){
    this->statusBar()->showMessage(__tr("Прочитана запись: ") + QString::fromLocal8Bit(z.shapka_sectorov.zapis));
}

/*!
 * \brief Emulator::addZapisToTable Добавляет в таблицу записей
 * на форме данные из структуры записи
 * \param z
 */
void Emulator::addZapisToTable(Zapis &z){
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(__tr("Запись_%1").arg(io->Count())));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(z.shapka_sectorov.detal)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem(QString::number(z.shapka_sectorov.nomer_operator)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 3, new QTableWidgetItem(QString::number(z.shapka_sectorov.vremya/10.0)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 4, new QTableWidgetItem(QDateTime::fromTime_t(z.shapka_sectorov.date_time).time().toString("hh:mm:ss")));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 5, new QTableWidgetItem(QDateTime::fromTime_t(z.shapka_sectorov.date_time).date().toString("dd.MM.yyyy")));
}
