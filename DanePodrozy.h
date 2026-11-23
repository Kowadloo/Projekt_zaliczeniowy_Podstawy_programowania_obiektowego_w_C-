#pragma once
#include <iostream>



class Cennik {
private:
    double cenaBenzyny;
    double cenaDiesla;
    double cenaPradu;

public:
    Cennik();

    double getCenaBenzyny() const;
    double getCenaDiesla() const;
    double getCenaPradu() const;

    void ustawCeny(double benzyna, double diesel, double prad);
};

class Trasa {
private:
    double dystansMiasto;
    double dystansTrasa;
    double dystansAutostrada;
    int liczbaDni;

public:
    Trasa(double calkowityDystans, int profilTrasy, int dni);

    double getDystansMiasto() const;
    double getDystansTrasa() const;
    double getDystansAutostrada() const;
    double getDystansCalkowity() const;
    int getLiczbaDni() const;
};

std::ostream& operator<<(std::ostream& os, const Trasa& trasa);