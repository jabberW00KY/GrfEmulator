#include "grfgenerator.h"
#include <qglobal.h>
#include <QDateTime>
#include <stdlib.h>

Zapis z;
Shapka s;
ShapkaSector sec;
Sector sector;
min_max_sector mm;

GrfGenerator::GrfGenerator() {}

Zapis &GrfGenerator::GenerateZapis(){
    z.shapka_sectorov = GenShapka();
    z.massiv_sector.resize(z.shapka_sectorov.kol_sectorov);
    for(int i = 0; i < z.shapka_sectorov.kol_sectorov; ++i)
        z.massiv_sector[i] = GenSector();
    return z;
}

Shapka &GrfGenerator::GenShapka(){
    s.date_time = QDateTime::currentDateTime().toTime_t();
    srand(100000000);
    s.detal = rand() % 9999999;
    srand(60);
    s.f_perem_toka = (40 + rand()) % 60;
    srand(6);
    s.kol_sectorov = rand() % 7;
    s.koridor_skor = 2;
    s.koridor_skor = 3;
    srand(999);
    s.nomer_operator = rand() % 999;
    s.nomer_ustanov = rand() % 999;
    srand(3);
    s.rezhim = rand() % 3;
    srand(100);
    s.sootnosh_perem = rand() % 99;
    srand(31);
    s.tok_zazhig = rand() % 30;
    srand(10);
    s.t_narast = rand() % 9;
    srand(6);
    s.t_obduv = rand() % 5;
    s.t_produv = rand() % 5;
    s.t_spad = rand() % 5;
}

ShapkaSector &GrfGenerator::GenShapkaSector(){
    srand(21);
    sec.dlina_puti = rand() % 20;
    srand(1000);
    sec.otchetKol = rand() % 999;
    sec.skorost = rand() % 500;
    srand(100);
    sec.tok_imp_minus = (10 + rand()) % 50;
    sec.tok_imp_plus = (50 + rand()) % 99;
    srand(31);
    sec.tok_paus_minus = (5 + rand()) % 15;
    sec.tok_paus_plus = (15 + rand()) % 30;
}

bool MaxTok(Otchet &o1, Otchet &o2){
    return o1.tok >= o2.tok;
}

bool MaxSkor(Otchet &o1, Otchet &o2){
    return o1.skor >= o2.skor;
}

bool MinTok(Otchet &o1, Otchet &o2){
    return o1.tok <= o2.tok;
}

bool MinSkor(Otchet &o1, Otchet &o2){
    return o1.skor <= o2.skor;
}

Sector &GrfGenerator::GenSector(){
    sector.shapka = GenShapkaSector();
    sector.otchety.resize(sector.shapka.otchetKol);
    for(int i = 0; i < sector.shapka.otchetKol; ++i){
        srand(1000);
        sector.otchety[i].tok = (sector.shapka.tok_imp_minus + rand()) % sector.shapka.tok_imp_plus;
        sector.otchety[i].skor = (sector.shapka.skorost - 10 + rand()) % (sector.shapka.skorost + 10);
    }
    sector.min_max_sectora.skor_max = (*std::max_element(sector.otchety.begin(), sector.otchety.end(), MaxSkor)).skor;
    sector.min_max_sectora.skor_min = (*std::min_element(sector.otchety.begin(), sector.otchety.end(), MinSkor)).skor;
    sector.min_max_sectora.tok_imp_max = (*std::max_element(sector.otchety.begin(), sector.otchety.end(), MaxTok)).tok;
    sector.min_max_sectora.tok_imp_min = (*std::min_element(sector.otchety.begin(), sector.otchety.end(), MinTok)).tok;
    sector.min_max_sectora.tok_paus_max = 0;
    sector.min_max_sectora.tok_paus_min = 0;
}
