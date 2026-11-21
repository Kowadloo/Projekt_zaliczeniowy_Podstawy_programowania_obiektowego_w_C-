#pragma once
#include <iostream>
#include <string>
using namespace std;

enum class ProfilTrasy {
    Miasto = 1,
    Mieszana = 2,
    Autostrada = 3
};


class Cennik {
private:
    double cenaBenzyny;
    double cenaDiesla;
    double cenaPraduMin;
    double cenaPraduMax;

public:
    Cennik();
    Cennik(double benzyna, double diesel, double pradMin, double pradMax);

    double getCenaBenzyny() const;
    double getCenaDiesla() const;
    double getCenaPraduMin() const;
    double getCenaPraduMax() const;

    void ustawCeny(double benzyna, double diesel, double pradMin, double pradMax);
};



class Trasa {
private:
    double dystansMiasto;
    double dystansTrasa;
    double dystansAutostrada;
    int liczbaDni;

public:
    Trasa();
    Trasa(double miasto, double trasa, double autostrada, int dni);
    Trasa(double calkowityDystans, ProfilTrasy profil, int dni);

    double getDystansMiasto() const;
    double getDystansTrasa() const;
    double getDystansAutostrada() const;
    double getDystansCalkowity() const;
    int getLiczbaDni() const;
};



ostream& operator<<(ostream& os, const Trasa& trasa);