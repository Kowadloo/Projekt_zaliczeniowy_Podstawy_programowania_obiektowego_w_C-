#include "DanePodrozy.h"
using namespace std;

Cennik::Cennik()
    : cenaBenzyny(6.50), cenaDiesla(6.65), cenaPraduMin(1.50), cenaPraduMax(3.50) {}

Cennik::Cennik(double benzyna, double diesel, double pradMin, double pradMax) {
    ustawCeny(benzyna, diesel, pradMin, pradMax);
}



double Cennik::getCenaBenzyny() const { return cenaBenzyny; }
double Cennik::getCenaDiesla() const { return cenaDiesla; }
double Cennik::getCenaPraduMin() const { return cenaPraduMin; }
double Cennik::getCenaPraduMax() const { return cenaPraduMax; }


void Cennik::ustawCeny(double benzyna, double diesel, double pradMin, double pradMax) {
    cenaBenzyny = benzyna;
    cenaDiesla = diesel;
    cenaPraduMin = pradMin;
    cenaPraduMax = pradMax;
}

Trasa::Trasa()
    : dystansMiasto(0.0), dystansTrasa(0.0), dystansAutostrada(0.0), liczbaDni(1) {}

Trasa::Trasa(double miasto, double trasa, double autostrada, int dni)
    : dystansMiasto(miasto), dystansTrasa(trasa), dystansAutostrada(autostrada), liczbaDni(dni) {}

Trasa::Trasa(double calkowityDystans, ProfilTrasy profil, int dni)
    : dystansMiasto(0.0), dystansTrasa(0.0), dystansAutostrada(0.0), liczbaDni(dni) {
    if (profil == ProfilTrasy::Miasto) {
        dystansMiasto = calkowityDystans * 0.70;
        dystansTrasa = calkowityDystans * 0.30;
        dystansAutostrada = 0.0;
    } else if (profil == ProfilTrasy::Mieszana) {
        dystansMiasto = calkowityDystans * 0.30;
        dystansTrasa = calkowityDystans * 0.50;
        dystansAutostrada = calkowityDystans * 0.20;
    } else if (profil == ProfilTrasy::Autostrada) {
        dystansMiasto = calkowityDystans * 0.20;
        dystansTrasa = calkowityDystans * 0.20;
        dystansAutostrada = calkowityDystans * 0.60;
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