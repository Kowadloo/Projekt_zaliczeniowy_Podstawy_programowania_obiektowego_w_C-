#pragma once
#include <iostream>
using namespace std;

class Cennik {
private:
    double cenaBenzyny;
    double cenaDiesla;
    double cenaPraduAC;
    double cenaPraduDC;

public:
    Cennik();
    Cennik(double benzyna, double diesel, double pradAC, double pradDC);

    double getCenaBenzyny() const;
    double getCenaDiesla() const;
    double getCenaPraduAC() const;
    double getCenaPraduDC() const;

    bool setCenaBenzyny(double cena);
    bool setCenaDiesla(double cena);
    bool setCenaPraduAC(double cena);
    bool setCenaPraduDC(double cena);
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

    double getDystansMiasto() const;
    double getDystansTrasa() const;
    double getDystansAutostrada() const;
    double getDystansCalkowity() const;
    int getLiczbaDni() const;

    bool setDystansMiasto(double km);
    bool setDystansTrasa(double km);
    bool setDystansAutostrada(double km);
    bool setLiczbaDni(int dni);
};