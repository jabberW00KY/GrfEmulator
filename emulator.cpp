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
    // ������������ � ������� ��������� ���������� ������ �� �����
    connect(io, SIGNAL(ZapisRead(Zapis&)), this, SLOT(zapisReaded(Zapis&)));
    // ������������ � ������� �������� ������ ��������� ������ � ����
    connect(io, SIGNAL(ZapisWritten(Zapis&)), this, SLOT(zapisWritten(Zapis&)));
}

Emulator::~Emulator()
{
    delete io;
    delete ui;
}

void Emulator::on_actSozdFile_triggered()
{
    // ���� ���� �������������� (����� ������� � ��������� ��������� � �������� *)
    // � ����������� ������� ����������������� �� ������ � ���������� �����
    // �� ��������� ����, ����� ��������� ��� � ������� ������� � �������������� �����.
    if(isFileEdited() && QMessageBox::question(0, __tr("��������� ����"),
            __tr("��������� ����������\n���� ����� ��������� ������?"), __tr("��"), __tr("���")) ==
            QMessageBox::Accepted){
        io->Save(); // ��������� ���� � ���������
        unsetEditedInTitle(); // ������� ������� � �������������� �����
    }

    createNewFile(); // ������� ����� ����
    setEditedInTitle(); // ������������� ������� � �������������� �����
}

/*!
 * \brief Emulator::setEditedInTitle ������������� ������� � �������������� �����
 */
void Emulator::setEditedInTitle(){
    if(!this->windowTitle().contains("*"))
        this->setWindowTitle("*" + this->windowTitle());
}

/*!
 * \brief Emulator::unsetEditedInTitle ���������� ������� � �������������� �����
 */
void Emulator::unsetEditedInTitle(){
    if(!this->windowTitle().contains("*"))
        this->setWindowTitle(this->windowTitle().remove("*"));
}

/*!
 * \brief Emulator::isFileEdited ��������� �������������� �� ���� (
 * ����� �� ������� � ��������������)
 * \return True - ���� ��������� �������� ������ *, ����� False
 */
bool Emulator::isFileEdited(){
    return *this->windowTitle().begin() == '*';
}

/*!
 * \brief Emulator::createNewFile ������� ����� ����.
 * ����������� ����������� ���� � ���������� ������ ����� �
 * �� ��������� ����� ��������� ����� ����.
 */
void Emulator::createNewFile(){
    QFileDialog *fileDialog = new QFileDialog(0, __tr("������� ����� ����"),
                                              QApplication::applicationDirPath());

    fileDialog->setAcceptMode(QFileDialog::AcceptSave); // ����� ���������� �����
    fileDialog->setNameFilter(__tr("����� ��������(*.grf)")); // ������ ������ �������� (.grf)

    if(fileDialog->exec() == QDialog::Accepted)
        io->SetFileName(fileDialog->selectedFiles().first());

    fileDialog->deleteLater();
}

/*!
 * \brief Emulator::zapisReaded ���� ��������� ������ ������ �� �����
 * \param z
 */
void Emulator::zapisReaded(Zapis &z){
    addZapisToTable(z);
}

/*!
 * \brief Emulator::zapisWritten ���� �������� ������ � ����
 * \param z
 */
void Emulator::zapisWritten(Zapis &z){
    this->statusBar()->showMessage(__tr("��������� ������: ") + QString::fromLocal8Bit(z.shapka_sectorov.zapis));
}

/*!
 * \brief Emulator::addZapisToTable ��������� � ������� �������
 * �� ����� ������ �� ��������� ������
 * \param z
 */
void Emulator::addZapisToTable(Zapis &z){
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(__tr("������_%1").arg(io->Count())));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(z.shapka_sectorov.detal)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem(QString::number(z.shapka_sectorov.nomer_operator)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 3, new QTableWidgetItem(QString::number(z.shapka_sectorov.vremya/10.0)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 4, new QTableWidgetItem(QDateTime::fromTime_t(z.shapka_sectorov.date_time).time().toString("hh:mm:ss")));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 5, new QTableWidgetItem(QDateTime::fromTime_t(z.shapka_sectorov.date_time).date().toString("dd.MM.yyyy")));
}
