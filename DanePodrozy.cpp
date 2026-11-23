#include "DanePodrozy.h"
#include <stdexcept>

using namespace std;



Cennik::Cennik()
    : cenaBenzyny(6.50), cenaDiesla(6.65), cenaPradu(2.50) {}

double Cennik::getCenaBenzyny() const { return cenaBenzyny; }
double Cennik::getCenaDiesla() const { return cenaDiesla; }
double Cennik::getCenaPradu() const { return cenaPradu; }

void Cennik::ustawCeny(double benzyna, double diesel, double prad) {
    if (benzyna <= 0.0 || diesel <= 0.0 || prad <= 0.0) {
        throw invalid_argument("Wszystkie ceny paliw i pradu musza byc wieksze od zera.");
    }
    cenaBenzyny = benzyna;
    cenaDiesla = diesel;
    cenaPradu = prad;
}

Trasa::Trasa(double calkowityDystans, int profilTrasy, int dni) {
    if (calkowityDystans <= 0.0) {
        throw invalid_argument("Calkowity dystans musi byc wiekszy od zera.");
    }
    if (dni < 1) {
        throw invalid_argument("Liczba dni wynajmu musi wynosic co najmniej 1.");
    }
    liczbaDni = dni;

    if (profilTrasy == 1) {
        dystansMiasto = calkowityDystans * 0.70;
        dystansTrasa = calkowityDystans * 0.30;
        dystansAutostrada = 0.0;
    } else if (profilTrasy == 2) {
        dystansMiasto = calkowityDystans * 0.34;
        dystansTrasa = calkowityDystans * 0.33;
        dystansAutostrada = calkowityDystans * 0.33;
    } else if (profilTrasy == 3) {
        dystansMiasto = calkowityDystans * 0.15;
        dystansTrasa = calkowityDystans * 0.35;
        dystansAutostrada = calkowityDystans * 0.50;
    } else {
        throw invalid_argument("Wybrano niepoprawny profil trasy.");
    }
}

double Trasa::getDystansMiasto() const { return dystansMiasto; }
double Trasa::getDystansTrasa() const { return dystansTrasa; }
double Trasa::getDystansAutostrada() const { return dystansAutostrada; }
double Trasa::getDystansCalkowity() const { return dystansMiasto + dystansTrasa + dystansAutostrada; }
int Trasa::getLiczbaDni() const { return liczbaDni; }

ostream& operator<<(ostream& os, const Trasa& trasa) {
    os << "Trasa (" << trasa.getLiczbaDni() << " dni, " << trasa.getDystansCalkowity() << " km) | "
       << "Miasto: " << trasa.getDystansMiasto() << " km, "
       << "Trasa: " << trasa.getDystansTrasa() << " km, "
       << "Autostrada: " << trasa.getDystansAutostrada() << " km";
    return os;
}