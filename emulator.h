#ifndef EMULATOR_H
#define EMULATOR_H

#include <QMainWindow>
#include "dataio.h"

namespace Ui {
class Emulator;
}

class Emulator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Emulator(QWidget *parent = 0);
    ~Emulator();

private slots:
    void on_actSozdFile_triggered();
    void zapisWritten(Zapis &z);
    void zapisReaded(Zapis &z);

private:
    void setEditedInTitle();
    void unsetEditedInTitle();
    bool isFileEdited();
    void createNewFile();
    void addZapisToTable(Zapis &z);

private:
    Ui::Emulator *ui;
    DataIO *io;
};

#endif // EMULATOR_H
