#include "dataio.h"
#include <QMessageBox>
#include <QTextCodec>

#define __tr(s) (QTextCodec::codecForName("Windows-1251")->toUnicode(s))

const quint16 defaultHeader = 0xff00 & ('Z' << 8) | 'Z';

void DataIO::SetFileName(QString fileName) {
  if (!fileName.isEmpty()) {
    this->srcFile.setFileName(!fileName.contains(".grf") ? (fileName + ".grf")
                                                         : fileName);
    OpenFile();
  }
}

void DataIO::OpenFile() { srcFile.open(QIODevice::ReadWrite); }

void DataIO::Read() {
  quint16 header = 0x0000;

  if (srcFile.isOpen()) {
    srcFile.read((char *)&header, sizeof(header));

    try {
      if (header == defaultHeader) {
        srcFile.read((char *)&recordCount, sizeof(recordCount));
        for (int i = 0; i < recordCount; i++)
          ReadZapis();
      } else
        throw 1;
    } catch (int e) {
      if (e == 1)
        QMessageBox::critical(0, __tr("Ошибка!"),
                              __tr("Некорректный файл отгрузки!"));
    }
  }
}

void DataIO::ReadZapis() {
  Zapis zap;

  srcFile.read((char *)&zap.shapka_sectorov, sizeof(zap.shapka_sectorov));
  for (int i = 0; i < zap.shapka_sectorov.kol_sectorov; ++i) {
    Sector sector;

    srcFile.read((char *)&sector.shapka, sizeof(sector.shapka));
    sector.otchety.resize(sector.shapka.otchetKol);
    srcFile.read((char *)sector.otchety.data(),
                 sector.shapka.otchetKol * sizeof(Otchet));
    srcFile.read((char *)&sector.min_max_sectora,
                 sizeof(sector.min_max_sectora));

    zap.massiv_sector.append(sector);
  }

  spisZap.append(zap);

  emit ZapisRead(zap);
}

void DataIO::WriteZapis() {
  if (srcFile.isOpen()) {
    int spis_len = spisZap.length();
    for (int i = 0; i < spis_len; i++) {
      srcFile.write((char *)&spisZap[i].shapka_sectorov,
                    sizeof(spisZap[i].shapka_sectorov));
      for (int j = 0; j < spisZap[i].shapka_sectorov.kol_sectorov; j++) {
        srcFile.write((char *)&spisZap[i].massiv_sector[j].shapka,
                      sizeof(spisZap[i].massiv_sector[j].shapka));
        srcFile.write((char *)spisZap[i].massiv_sector[j].otchety.data(),
                      sizeof(Otchet) *
                          spisZap[i].massiv_sector[j].shapka.otchetKol);
        srcFile.write((char *)&spisZap[i].massiv_sector[j].min_max_sectora,
                      sizeof(spisZap[i].massiv_sector[j].min_max_sectora));
      }

      emit ZapisWritten(spisZap[i]);
    }
  }
}

void DataIO::Save() {
  if (spisZap.count()) {
    srcFile.resize(0);
    srcFile.write((char *)&defaultHeader, sizeof(defaultHeader));
    srcFile.write((char *)&recordCount, sizeof(recordCount));
    WriteZapis();
  } else {
    srcFile.remove();
  }
  srcFile.close();
}

void DataIO::AppendZapis(Zapis &zap) {
  spisZap.append(zap);
  ++recordCount;
}

void DataIO::RemoveAt(int i) {
  spisZap.removeAt(i);
  --recordCount;
}

int DataIO::Count() { return recordCount; }

void DataIO::MergeFile(QString fileName) {}

Zapis &DataIO::At(int i) { return spisZap[i]; }
