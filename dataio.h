#ifndef DATAIO_H
#define DATAIO_H

#include <qfile.h>
#include <qglobal.h>
#include <qvector.h>

struct __attribute__((packed)) Shapka {
  quint8 rezhim;           //����� ������
  quint16 nomer_ustanov;   //� ���������
  quint8 t_produv;         //����� �������
  quint8 tok_zazhig;       //��� ���������
  quint8 t_narast;         //����� ����������
  quint8 t_spad;           //����� �����
  quint8 t_obduv;          //����� ������
  quint8 t_zaderzh_vrasch; //����� �������� ��������
  quint8 f_perem_toka;     //������� ����������� ����
  quint8 sootnosh_perem;   //����������� +/-
  quint8 kol_sectorov;     //���������� ��������
  quint8 koridor_toka;     //������� ����
  quint8 koridor_skor;     //������� ��������
  quint16 nomer_operator;  //��������
  quint32 detal;           //����� ������
  char zapis[80];          //�������� ������
  time_t date_time; //���� � ����� [0] � [1] ��������������
  quint32 vremya; //����� ����������
};
struct __attribute__((packed)) Otchet {
  quint16 tok;
  quint16 skor;
};
struct __attribute__((packed)) ShapkaSector {
  quint8 dlina_puti;      //����� ����
  quint16 skorost;        //��������
  quint16 tok_imp_plus;   //��� �������� +
  quint16 tok_imp_minus;  //��� �������� -
  quint16 tok_paus_plus;  //��� ����� +
  quint16 tok_paus_minus; //��� ����� -
  quint8 t_imp;           //����� ��������
  quint8 t_paus;          //����� �����
  quint16 otchetKol;      //���������� �������
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
