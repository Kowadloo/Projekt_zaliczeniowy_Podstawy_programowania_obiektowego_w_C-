#include "Wypozyczalnia.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;



Wypozyczalnia::Wypozyczalnia(const string& nazwa, const string& plikFloty)
    : nazwa(nazwa), plikFloty(plikFloty) {
    wczytajFloteZPliku();
}

Wypozyczalnia::~Wypozyczalnia() {
    wyczyscFlote();
}

Pojazd* Wypozyczalnia::znajdzPoId(int id) const {
    for (Pojazd* p : flota) {
        if (p->getId() == id) {
            return p;
        }
    }
    return nullptr;
}

void Wypozyczalnia::wyczyscFlote() {
    for (Pojazd* p : flota) {
        delete p;
    }
    flota.clear();
}

const Cennik& Wypozyczalnia::getCennik() const {
    return cennik;
}

void Wypozyczalnia::aktualizujCennik(double benzyna, double diesel, double prad) {
    cennik.ustawCeny(benzyna, diesel, prad);
}

void Wypozyczalnia::zapiszFloteDoPliku() const {
    ofstream plik(plikFloty);
    if (!plik.is_open()) {
        throw runtime_error("Nie udalo sie zapisac zmian do pliku: " + plikFloty);
    }
    plik << fixed;
    plik.precision(2);
    for (const Pojazd* p : flota) {
        p->zapiszDoPliku(plik);
        plik << "\n";
    }
}

void Wypozyczalnia::wczytajFloteZPliku() {
    ifstream plik(plikFloty);
    if (!plik.is_open()) {
        throw runtime_error("Nie udalo sie otworzyc pliku bazy: " + plikFloty);
    }
    try {
        char typ;
        while (plik >> typ) {
            int id;
            string marka, model;
            double stawka, kaucja;
            bool zajety;

            if (!(plik >> id >> marka >> model >> stawka >> kaucja >> zajety)) {
                throw runtime_error("Uszkodzony rekord w pliku: " + plikFloty);
            }

            Pojazd* p = nullptr;
            if (typ == 'S') {
                string paliwo;
                double sM, sT, sA;
                if (!(plik >> paliwo >> sM >> sT >> sA)) {
                    throw runtime_error("Uszkodzone dane pojazdu spalinowego w pliku: " + plikFloty);
                }
                p = new PojazdSpalinowy(id, marka, model, stawka, kaucja, paliwo, sM, sT, sA);
            } else if (typ == 'E') {
                double zM, zT, zA, bat;
                if (!(plik >> zM >> zT >> zA >> bat)) {
                    throw runtime_error("Uszkodzone dane pojazdu elektrycznego w pliku: " + plikFloty);
                }
                p = new PojazdElektryczny(id, marka, model, stawka, kaucja, zM, zT, zA, bat);
            } else {
                throw runtime_error("Nieznany typ pojazdu w pliku: " + plikFloty);
            }

            p->setWypozyczony(zajety);
            flota.push_back(p);
        }

        if (!plik.eof()) {
            throw runtime_error("Blad odczytu danych w pliku: " + plikFloty);
        }
    } catch (const exception&) {
        wyczyscFlote();
        throw;
    }
}

void Wypozyczalnia::dodajPojazd(Pojazd* nowyPojazd) {
    for (const Pojazd* p : flota) {
        if (*p == *nowyPojazd) {
            delete nowyPojazd;
            throw invalid_argument("Pojazd o podanym ID juz istnieje we flocie.");
        }
    }
    flota.push_back(nowyPojazd);
    zapiszFloteDoPliku();
}

void Wypozyczalnia::usunPojazd(int id) {
    for (size_t i = 0; i < flota.size(); ++i) {
        if (flota[i]->getId() == id) {
            if (flota[i]->isWypozyczony()) {
                throw runtime_error("Nie mozna usunac pojazdu, ktory jest aktualnie wypozyczony.");
            }
            delete flota[i];
            flota.erase(flota.begin() + i);
            zapiszFloteDoPliku();
            return;
        }
    }
    throw runtime_error("Nie znaleziono pojazdu o podanym ID.");
}

void Wypozyczalnia::wypozyczPojazd(int id) {
    Pojazd* p = znajdzPoId(id);
    if (p == nullptr) {
        throw runtime_error("Nie znaleziono pojazdu o podanym ID.");
    }
    if (p->isWypozyczony()) {
        throw runtime_error("Ten pojazd jest juz wypozyczony.");
    }
    p->setWypozyczony(true);
    zapiszFloteDoPliku();
}

void Wypozyczalnia::zwrocPojazd(int id) {
    Pojazd* p = znajdzPoId(id);
    if (p == nullptr) {
        throw runtime_error("Nie znaleziono pojazdu o podanym ID.");
    }
    if (!p->isWypozyczony()) {
        throw runtime_error("Ten pojazd nie jest wypozyczony.");
    }
    p->setWypozyczony(false);
    zapiszFloteDoPliku();
}

void Wypozyczalnia::wyswietlFlote() const {
    cout << "\nAktualna flota (" << nazwa << "):\n";
    if (flota.empty()) {
        cout << "Brak pojazdow we flocie.\n";
        return;
    }
    for (const Pojazd* p : flota) {
        cout << *p << "\n";
    }
}

void Wypozyczalnia::wyswietlWypozyczone() const {
    int licznik = 0;
    for (const Pojazd* p : flota) {
        if (p->isWypozyczony()) {
            if (licznik == 0) {
                cout << "\nWypozyczone pojazdy:\n";
            }
            cout << *p << "\n";
            licznik++;
        }
    }
    if (licznik == 0) {
        throw runtime_error("Aktualnie zaden pojazd nie jest wypozyczony.");
    }
}

void Wypozyczalnia::dobierzAutoDlaKlienta(const Trasa& trasa, int preferencjaNapedu) const {
    if (preferencjaNapedu < 0 || preferencjaNapedu > 2) {
        throw invalid_argument("Niepoprawna preferencja napedu.");
    }

    vector<const Pojazd*> pasujace;
    for (const Pojazd* p : flota) {
        if (p->isWypozyczony()) {
            continue;
        }
        if (preferencjaNapedu != 0 && p->getKodNapedu() != preferencjaNapedu) {
            continue;
        }
        pasujace.push_back(p);
    }

    if (pasujace.empty()) {
        throw runtime_error("Brak dostepnych pojazdow spelniajacych wybrane kryteria.");
    }

    const Pojazd* najlepszy = nullptr;
    double najnizszyKoszt = -1.0;

    cout << "\nKalkulacja oferty:\n";
    cout << trasa << "\n\n";

    for (const Pojazd* p : pasujace) {
        double najem = p->getStawkaDobowa() * trasa.getLiczbaDni();
        double energia = p->obliczKosztEnergii(trasa, cennik);
        double razem = najem + energia;

        cout << "- [ID: " << p->getId() << "] " << p->getMarka() << " " << p->getModel()
             << " (" << p->getOpisNapedu() << ")\n"
             << "  Najem: " << najem << " PLN | Paliwo/prad: " << energia
             << " PLN | razem: " << razem << " PLN (kaucja zwrotna: " << p->getKaucja() << " PLN)\n";

        string infoDodatkowe = p->getDodatkoweInfoTrasy(trasa);
        if (!infoDodatkowe.empty()) {
            cout << "  " << infoDodatkowe << "\n";
        }
        cout << "\n";

        if (najlepszy == nullptr || razem < najnizszyKoszt) {
            najlepszy = p;
            najnizszyKoszt = razem;
        }
    }

    cout << "Rekomendacja dla klienta: " << najlepszy->getMarka() << " "
         << najlepszy->getModel() << " (ID: " << najlepszy->getId() << ") - lacznie: "
         << najnizszyKoszt << " PLN (kaucja zwrotna: " << najlepszy->getKaucja() << " PLN)\n";
}