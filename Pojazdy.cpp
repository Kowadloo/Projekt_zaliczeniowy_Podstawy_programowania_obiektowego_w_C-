#include "Pojazdy.h"
#include <stdexcept>

using namespace std;

const double UZYTECZNA_CZESC_BATERII = 0.80;
const double MOC_LADOWARKI_KW = 50.0;

Pojazd::Pojazd(int id, const string& marka, const string& model, double stawkaDobowa, double kaucja)
    : id(id), marka(marka), model(model), stawkaDobowa(stawkaDobowa), kaucja(kaucja), wypozyczony(false) {
    if (id <= 0 || stawkaDobowa <= 0.0) {
        throw invalid_argument("ID pojazdu i stawka dobowa musza byc wieksze od zera.");
    }
    if (kaucja < 0.0) {
        throw invalid_argument("Kaucja nie moze byc ujemna.");
    }
    if (marka.empty() || model.empty()) {
        throw invalid_argument("Marka i model nie moga byc puste.");
    }
    if (marka.find(' ') != string::npos || model.find(' ') != string::npos) {
        throw invalid_argument("Marka i model nie moga zawierac spacji (uzyj znaku '_').");
    }
}

int Pojazd::getId() const { return id; }
string Pojazd::getMarka() const { return marka; }
string Pojazd::getModel() const { return model; }
double Pojazd::getStawkaDobowa() const { return stawkaDobowa; }
double Pojazd::getKaucja() const { return kaucja; }
bool Pojazd::isWypozyczony() const { return wypozyczony; }
void Pojazd::setWypozyczony(bool status) { wypozyczony = status; }

string Pojazd::getDodatkoweInfoTrasy(const Trasa&) const {
    return "";
}

void Pojazd::wypiszSzczegoly(ostream& os) const {
    os << "[ID: " << id << "] " << marka << " " << model
       << " | " << getOpisNapedu()
       << " | " << stawkaDobowa << " PLN/doba"
       << " | Kaucja: " << kaucja << " PLN"
       << " | " << (wypozyczony ? "wypozyczony" : "dostepny");
}

void Pojazd::zapiszDoPliku(ostream& os) const {
    os << getTypPliku() << " " << id << " " << marka << " " << model << " "
       << stawkaDobowa << " " << kaucja << " " << (wypozyczony ? 1 : 0) << " ";
    dopiszSzczegolyDoLinii(os);
}

bool Pojazd::operator==(const Pojazd& inny) const {
    return id == inny.id;
}

ostream& operator<<(ostream& os, const Pojazd& pojazd) {
    pojazd.wypiszSzczegoly(os);
    return os;
}

PojazdSpalinowy::PojazdSpalinowy(int id, const string& marka, const string& model, double stawkaDobowa,
                                 double kaucja, const string& rodzajPaliwa, double spalanieMiasto,
                                 double spalanieTrasa, double spalanieAutostrada)
    : Pojazd(id, marka, model, stawkaDobowa, kaucja), rodzajPaliwa(rodzajPaliwa),
      spalanieMiasto(spalanieMiasto), spalanieTrasa(spalanieTrasa), spalanieAutostrada(spalanieAutostrada) {
    if (rodzajPaliwa != "Benzyna" && rodzajPaliwa != "Diesel") {
        throw invalid_argument("Rodzaj paliwa musi byc 'Benzyna' lub 'Diesel'.");
    }
    if (spalanieMiasto <= 0.0 || spalanieTrasa <= 0.0 || spalanieAutostrada <= 0.0) {
        throw invalid_argument("Wartosci spalania musza byc wieksze od zera.");
    }
}

char PojazdSpalinowy::getTypPliku() const { return 'S'; }

void PojazdSpalinowy::dopiszSzczegolyDoLinii(ostream& os) const {
    os << rodzajPaliwa << " " << spalanieMiasto << " " << spalanieTrasa << " " << spalanieAutostrada;
}

double PojazdSpalinowy::obliczKosztEnergii(const Trasa& trasa, const Cennik& cennik) const {
    double litry = (trasa.getDystansMiasto() / 100.0) * spalanieMiasto
                 + (trasa.getDystansTrasa() / 100.0) * spalanieTrasa
                 + (trasa.getDystansAutostrada() / 100.0) * spalanieAutostrada;
    double cena = (rodzajPaliwa == "Diesel") ? cennik.getCenaDiesla() : cennik.getCenaBenzyny();
    return litry * cena;
}

string PojazdSpalinowy::getOpisNapedu() const {
    return "Spalinowy (" + rodzajPaliwa + ")";
}

int PojazdSpalinowy::getKodNapedu() const { return 1; }

void PojazdSpalinowy::wypiszSzczegoly(ostream& os) const {
    Pojazd::wypiszSzczegoly(os);
    os << " | Spalanie: " << spalanieMiasto << "/" << spalanieTrasa << "/" << spalanieAutostrada << " l/100km";
}

PojazdElektryczny::PojazdElektryczny(int id, const string& marka, const string& model, double stawkaDobowa,
                                     double kaucja, double zuzycieMiasto, double zuzycieTrasa,
                                     double zuzycieAutostrada, double pojemnoscBaterii)
    : Pojazd(id, marka, model, stawkaDobowa, kaucja), zuzycieMiasto(zuzycieMiasto),
      zuzycieTrasa(zuzycieTrasa), zuzycieAutostrada(zuzycieAutostrada), pojemnoscBaterii(pojemnoscBaterii) {
    if (zuzycieMiasto <= 0.0 || zuzycieTrasa <= 0.0 || zuzycieAutostrada <= 0.0 || pojemnoscBaterii <= 0.0) {
        throw invalid_argument("Zuzycie energii i pojemnosc baterii musza byc wieksze od zera.");
    }
}

char PojazdElektryczny::getTypPliku() const { return 'E'; }

void PojazdElektryczny::dopiszSzczegolyDoLinii(ostream& os) const {
    os << zuzycieMiasto << " " << zuzycieTrasa << " " << zuzycieAutostrada << " " << pojemnoscBaterii;
}

double PojazdElektryczny::obliczZuzyteKwh(const Trasa& trasa) const {
    return ((trasa.getDystansMiasto() / 100.0) * zuzycieMiasto)
         + ((trasa.getDystansTrasa() / 100.0) * zuzycieTrasa)
         + ((trasa.getDystansAutostrada() / 100.0) * zuzycieAutostrada);
}

double PojazdElektryczny::obliczKosztEnergii(const Trasa& trasa, const Cennik& cennik) const {
    return obliczZuzyteKwh(trasa) * cennik.getCenaPradu();
}

string PojazdElektryczny::getOpisNapedu() const {
    return "Elektryczny (EV)";
}

int PojazdElektryczny::getKodNapedu() const { return 2; }

string PojazdElektryczny::getDodatkoweInfoTrasy(const Trasa& trasa) const {
    double zuzyteKwh = obliczZuzyteKwh(trasa);
    double uzyteczneKwh = UZYTECZNA_CZESC_BATERII * pojemnoscBaterii;
    double dostepnaEnergia = uzyteczneKwh;
    int liczbaLadowan = 0;

    while (dostepnaEnergia < zuzyteKwh) {
        liczbaLadowan++;
        dostepnaEnergia += uzyteczneKwh;
    }

    if (liczbaLadowan == 0) {
        return "Ladowanie na stacji: brak (wystarczy energia z baterii startowej)";
    }

    double energiaDoUzupelnienia = liczbaLadowan * uzyteczneKwh;
    int laczneMinuty = static_cast<int>((energiaDoUzupelnienia / MOC_LADOWARKI_KW) * 60.0 + 0.5);
    int godziny = laczneMinuty / 60;
    int minuty = laczneMinuty % 60;

    return "Ladowanie na stacji (20-100%, charger 50 kW): "
         + to_string(liczbaLadowan) + "x (czas postoju: "
         + to_string(godziny) + " h " + to_string(minuty) + " min)";
}

void PojazdElektryczny::wypiszSzczegoly(ostream& os) const {
    Pojazd::wypiszSzczegoly(os);
    os << " | Bateria: " << pojemnoscBaterii << " kWh | Zuzycie: "
       << zuzycieMiasto << "/" << zuzycieTrasa << "/" << zuzycieAutostrada << " kWh/100km";
}