#include "Pojazdy.h"

using namespace std;

Pojazd::Pojazd()
    : id(0), marka("Nieznana"), model("Nieznany"), stawkaDobowa(100.0), wypozyczony(false) {}

Pojazd::Pojazd(int id, const string& marka, const string& model, double stawkaDobowa)
    : id(id), marka(marka), model(model), stawkaDobowa(stawkaDobowa), wypozyczony(false) {}

int Pojazd::getId() const { return id; }
string Pojazd::getMarka() const { return marka; }
string Pojazd::getModel() const { return model; }
double Pojazd::getStawkaDobowa() const { return stawkaDobowa; }
bool Pojazd::isWypozyczony() const { return wypozyczony; }
void Pojazd::setWypozyczony(bool status) { wypozyczony = status; }

double Pojazd::obliczKosztPaliwaMax(const Trasa& trasa, const Cennik& cennik) const {
    return obliczKosztPaliwa(trasa, cennik);
}

void Pojazd::wypiszSzczegoly(ostream& os) const {
    os << "[ID: " << id << "] " << marka << " " << model
       << " | " << getTypNapedu()
       << " | " << stawkaDobowa << " PLN/doba"
       << " | " << (wypozyczony ? "WYPOZYCZONY" : "DOSTEPNY");
}

bool Pojazd::operator==(const Pojazd& inny) const {
    return id == inny.id;
}

ostream& operator<<(ostream& os, const Pojazd& pojazd) {
    pojazd.wypiszSzczegoly(os);
    return os;
}

PojazdSpalinowy::PojazdSpalinowy()
    : Pojazd(), rodzajPaliwa("Benzyna"), spalanieMiasto(7.5), spalanieTrasa(5.5), spalanieAutostrada(6.8) {}

PojazdSpalinowy::PojazdSpalinowy(int id, const string& marka, const string& model, double stawkaDobowa,
                                 const string& rodzajPaliwa, double spalanieMiasto,
                                 double spalanieTrasa, double spalanieAutostrada)
    : Pojazd(id, marka, model, stawkaDobowa), rodzajPaliwa(rodzajPaliwa),
      spalanieMiasto(spalanieMiasto), spalanieTrasa(spalanieTrasa), spalanieAutostrada(spalanieAutostrada) {}

double PojazdSpalinowy::obliczKosztPaliwa(const Trasa& trasa, const Cennik& cennik) const {
    double litry = (trasa.getDystansMiasto() / 100.0) * spalanieMiasto
                 + (trasa.getDystansTrasa() / 100.0) * spalanieTrasa
                 + (trasa.getDystansAutostrada() / 100.0) * spalanieAutostrada;
    double cena = (rodzajPaliwa == "Diesel") ? cennik.getCenaDiesla() : cennik.getCenaBenzyny();
    return litry * cena;
}

string PojazdSpalinowy::getTypNapedu() const {
    return "Spalinowy (" + rodzajPaliwa + ")";
}

void PojazdSpalinowy::wypiszSzczegoly(ostream& os) const {
    Pojazd::wypiszSzczegoly(os);
    os << " | Spalanie: " << spalanieMiasto << "/" << spalanieTrasa << "/" << spalanieAutostrada << " l/100km";
}

PojazdElektryczny::PojazdElektryczny()
    : Pojazd(), zuzycieMiasto(14.0), zuzycieTrasa(16.0), zuzycieAutostrada(22.0), pojemnoscBaterii(60.0) {}

PojazdElektryczny::PojazdElektryczny(int id, const string& marka, const string& model, double stawkaDobowa,
                                     double zuzycieMiasto, double zuzycieTrasa,
                                     double zuzycieAutostrada, double pojemnoscBaterii)
    : Pojazd(id, marka, model, stawkaDobowa), zuzycieMiasto(zuzycieMiasto),
      zuzycieTrasa(zuzycieTrasa), zuzycieAutostrada(zuzycieAutostrada), pojemnoscBaterii(pojemnoscBaterii) {}

double PojazdElektryczny::obliczZuzyteKwh(const Trasa& trasa) const {
    return ((trasa.getDystansMiasto() / 100.0) * zuzycieMiasto)
         + ((trasa.getDystansTrasa() / 100.0) * zuzycieTrasa)
         + ((trasa.getDystansAutostrada() / 100.0) * zuzycieAutostrada);
}

double PojazdElektryczny::obliczKosztPaliwa(const Trasa& trasa, const Cennik& cennik) const {
    return obliczZuzyteKwh(trasa) * cennik.getCenaPraduMin();
}

double PojazdElektryczny::obliczKosztPaliwaMax(const Trasa& trasa, const Cennik& cennik) const {
    return obliczZuzyteKwh(trasa) * cennik.getCenaPraduMax();
}

string PojazdElektryczny::getTypNapedu() const {
    return "Elektryczny (EV)";
}

void PojazdElektryczny::wypiszSzczegoly(ostream& os) const {
    Pojazd::wypiszSzczegoly(os);
    os << " | Bateria: " << pojemnoscBaterii << " kWh | Zuzycie: "
       << zuzycieMiasto << "/" << zuzycieTrasa << "/" << zuzycieAutostrada << " kWh/100km";
}