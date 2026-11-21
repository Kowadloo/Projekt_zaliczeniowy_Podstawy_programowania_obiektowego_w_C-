#pragma once
#include <iostream>
#include <string>

using namespace std;

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
    Trasa(double calkowityDystans, int profilTrasy, int dni);

    double getDystansMiasto() const;
    double getDystansTrasa() const;
    double getDystansAutostrada() const;
    double getDystansCalkowity() const;
    int getLiczbaDni() const;

    friend ostream& operator<<(ostream& os, const Trasa& trasa);
};