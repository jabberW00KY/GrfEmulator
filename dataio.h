#ifndef DATAIO_H
#define DATAIO_H

#include <qfile.h>
#include <qglobal.h>
#include <qvector.h>

struct __attribute__((packed)) Shapka {
  quint8 rezhim;           //Режим сварки
  quint16 nomer_ustanov;   //№ установки
  quint8 t_produv;         //Время продува
  quint8 tok_zazhig;       //Ток зажигания
  quint8 t_narast;         //Время нарастания
  quint8 t_spad;           //Время спада
  quint8 t_obduv;          //Время обдува
  quint8 t_zaderzh_vrasch; //Время задержки вращения
  quint8 f_perem_toka;     //Частота переменного тока
  quint8 sootnosh_perem;   //Соотношение +/-
  quint8 kol_sectorov;     //Количество секторов
  quint8 koridor_toka;     //Коридор тока
  quint8 koridor_skor;     //Коридор скорости
  quint16 nomer_operator;  //Оператор
  quint32 detal;           //Номер детали
  char zapis[80];          //Название записи
  time_t date_time; //Дата и время [0] и [1] соответственно
  quint32 vremya; //Время выполнения
};
struct __attribute__((packed)) Otchet {
  quint16 tok;
  quint16 skor;
};
struct __attribute__((packed)) ShapkaSector {
  quint8 dlina_puti;      //Длина пути
  quint16 skorost;        //Скорость
  quint16 tok_imp_plus;   //Ток импульса +
  quint16 tok_imp_minus;  //Ток импульса -
  quint16 tok_paus_plus;  //Ток паузы +
  quint16 tok_paus_minus; //Ток паузы -
  quint8 t_imp;           //Время импульса
  quint8 t_paus;          //Время паузы
  quint16 otchetKol;      //Количество отчетов
};
struct __attribute__((packed)) min_max_sector {
  quint16 tok_imp_min;
  quint16 tok_imp_max;
  quint16 tok_paus_min;
  quint16 tok_paus_max;
  quint16 skor_min;
  quint16 skor_max;
};
struct Sector {
  ShapkaSector shapka;
  QVector<Otchet> otchety;
  min_max_sector min_max_sectora;
};
struct Zapis {
  Shapka shapka_sectorov;
  QVector<Sector> massiv_sector;
};

class DataIO : public QObject {
  Q_OBJECT

public:
  DataIO(QObject *p = 0) : QObject(p) { recordCount = 0; }
  void SetFileName(QString fileName);
  void Read();
  void Save();
  void AppendZapis(Zapis &zap);
  void RemoveAt(int i);
  int Count();
  bool isOpen() { return srcFile.isOpen(); }
  void MergeFile(QString fileName);
  QString fileName() { return srcFile.fileName(); }
  Zapis &At(int i);

signals:
  void ZapisRead(Zapis &z);
  void ZapisWritten(Zapis &z);

private:
  void OpenFile();
  void ReadZapis();
  void WriteZapis();

private:
  QFile srcFile;
  QList<Zapis> spisZap;
  quint16 recordCount;
};

#endif // DATAIO_H
