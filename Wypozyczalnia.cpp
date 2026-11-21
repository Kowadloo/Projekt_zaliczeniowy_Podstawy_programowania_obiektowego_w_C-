#include "Wypozyczalnia.h"
#include <iostream>
#include <fstream>
using namespace std;



vector<string> rozdzielTekst(const string& tekst, char separator) {
    vector<string> wynik;
    size_t start = 0;
    size_t koniec = tekst.find(separator);
    while (koniec != string::npos) {
        wynik.push_back(tekst.substr(start, koniec - start));
        start = koniec + 1;
        koniec = tekst.find(separator, start);
    }
    wynik.push_back(tekst.substr(start));
    return wynik;
}

Wypozyczalnia::Wypozyczalnia(const string& nazwa, const string& plikFloty)
    : nazwa(nazwa), plikFloty(plikFloty) {
    wczytajFloteZPliku();
}

Wypozyczalnia::~Wypozyczalnia() {
    wyczyscFlote();
}

void Wypozyczalnia::wyczyscFlote() {
    for (Pojazd* p : flota) {
        delete p;
    }
    flota.clear();
}

Cennik& Wypozyczalnia::getCennik() { return cennik; }
const Cennik& Wypozyczalnia::getCennik() const { return cennik; }

void Wypozyczalnia::zapiszFloteDoPliku() const {
    ofstream plik(plikFloty);
    if (!plik.is_open()) {
        cout << "Blad zapisu pliku: " << plikFloty << "\n";
        return;
    }
    for (const Pojazd* p : flota) {
        plik << p->zapiszDoLinii() << "\n";
    }
}

void Wypozyczalnia::wczytajFloteZPliku() {
    ifstream plik(plikFloty);
    if (!plik.is_open()) {
        cout << "Nie udalo sie otworzyc pliku bazy: " << plikFloty << "\n";
        return;
    }

    char typ;
    int id;
    string marka, model;
    double stawka;
    bool zajety;

    while (plik >> typ >> id >> marka >> model >> stawka >> zajety) {
        Pojazd* p = nullptr;
        if (typ == 'S') {
            string paliwo;
            double sM, sT, sA;
            if (plik >> paliwo >> sM >> sT >> sA) {
                p = new PojazdSpalinowy(id, marka, model, stawka, paliwo, sM, sT, sA);
            }
        } else if (typ == 'E') {
            double zM, zT, zA, bat;
            if (plik >> zM >> zT >> zA >> bat) {
                p = new PojazdElektryczny(id, marka, model, stawka, zM, zT, zA, bat);
            }
        }

        if (p != nullptr) {
            p->setWypozyczony(zajety);
            flota.push_back(p);
        }
    }
}

void Wypozyczalnia::dodajPojazd(Pojazd* nowyPojazd) {
    for (const Pojazd* p : flota) {
        if (*p == *nowyPojazd) {
            delete nowyPojazd;
            cout << "Pojazd o podanym ID juz istnieje we flocie.\n";
            return;
        }
    }
    flota.push_back(nowyPojazd);
    zapiszFloteDoPliku();
    cout << "Pojazd dodany i zapisany w bazie.\n";
}

void Wypozyczalnia::usunPojazd(int id) {
    for (size_t i = 0; i < flota.size(); ++i) {
        if (flota[i]->getId() == id) {
            delete flota[i];
            flota.erase(flota.begin() + i);
            zapiszFloteDoPliku();
            cout << "Pojazd o ID " << id << " usuniety z bazy.\n";
            return;
        }
    }
    cout << "Nie znaleziono pojazdu o podanym ID.\n";
}

void Wypozyczalnia::wypozyczPojazd(int id) {
    for (Pojazd* p : flota) {
        if (p->getId() == id) {
            if (p->isWypozyczony()) {
                cout << "Ten pojazd jest juz wypozyczony.\n";
                return;
            }
            p->setWypozyczony(true);
            zapiszFloteDoPliku();
            cout << "Pojazd o ID " << id << " zostal wydany klientowi.\n";
            return;
        }
    }
    cout << "Nie znaleziono pojazdu o podanym ID.\n";
}

void Wypozyczalnia::zwrocPojazd(int id) {
    for (Pojazd* p : flota) {
        if (p->getId() == id) {
            if (!p->isWypozyczony()) {
                cout << "Ten pojazd nie jest wypozyczony.\n";
                return;
            }
            p->setWypozyczony(false);
            zapiszFloteDoPliku();
            cout << "Pojazd o ID " << id << " pojazd zwrocony.\n";
            return;
        }
    }
    cout << "Nie znaleziono pojazdu o podanym ID.\n";
}



void Wypozyczalnia::wyswietlFlote() const {
    cout << "\n AKTUALNA FLOTA: " << nazwa << "\n";
    if (flota.empty()) {
        cout << "Brak pojazdow we flocie.\n";
        return;
    }
    for (const Pojazd* p : flota) {
        cout << *p << "\n";
    }
}

void Wypozyczalnia::wyswietlKrotkaListe() const {
    cout << "\nLista pojazdow:\n";
    for (const Pojazd* p : flota) {
        cout << "[ID: " << p->getId() << "] " << p->getMarka() << " " << p->getModel()
             << " (" << (p->isWypozyczony() ? "WYPOZYCZONY" : "DOSTEPNY") << ")\n";
    }
}

void Wypozyczalnia::wyswietlWypozyczone() const {
    bool znaleziono = false;
    for (const Pojazd* p : flota) {
        if (p->isWypozyczony()) {
            znaleziono = true;
            break;
        }
    }
    if (!znaleziono) {
        cout << "\nAktualnie zaden pojazd nie jest wypozyczony.\n";
        return;
    }

    cout << "\nPojazdy aktualnie wypozyczone:\n";
    for (const Pojazd* p : flota) {
        if (p->isWypozyczony()) {
            cout << "[ID: " << p->getId() << "] " << p->getMarka() << " " << p->getModel() << "\n";
        }
    }
}

void Wypozyczalnia::wyswietlWolneId() const {
    if (flota.empty()) {
        cout << "Baza jest pusta | Sugerowane wolne ID: 1\n";
        return;
    }
    int maxId = 0;
    cout << "Zajete ID we flocie: ";
    for (size_t i = 0; i < flota.size(); ++i) {
        int aktualneId = flota[i]->getId();
        cout << aktualneId;
        if (i + 1 < flota.size()) {
            cout << ", ";
        }
        if (aktualneId > maxId) {
            maxId = aktualneId;
        }
    }
    cout << " | Sugerowane wolne ID: " << maxId + 1 << "\n";
}

void Wypozyczalnia::dobierzAutoDlaKlienta(const Trasa& trasa, TypNapedu preferencja) const {
    const Pojazd* najlepszy = nullptr;
    double najnizszyKosztSredni = -1.0;
    double najlepszyMin = 0.0;
    double najlepszyMax = 0.0;

    cout << "\WWYLICZANIE OFERTY\n";
    cout << trasa << "\n\n";

    for (const Pojazd* p : flota) {
        if (p->isWypozyczony()) {
            continue;
        }
        if (preferencja != TypNapedu::Wszystkie && p->getTypNapedu() != preferencja) {
            continue;
        }

        double najem = p->getStawkaDobowa() * trasa.getLiczbaDni();
        KosztEnergii energia = p->obliczKosztEnergii(trasa, cennik);
        double razemMin = najem + energia.min;
        double razemMax = najem + energia.max;
        double razemSrednia = (razemMin + razemMax) / 2.0;

        cout << "- [ID: " << p->getId() << "] " << p->getMarka() << " " << p->getModel()
             << " (" << p->getOpisNapedu() << ")\n";
        if (energia.min == energia.max) {
            cout << "  Najem: " << najem << " PLN | Paliwo: " << energia.min
                 << " PLN | RAZEM: " << razemMin << " PLN\n\n";
        } else {
            cout << "  Najem: " << najem << " PLN | Prad: od " << energia.min << " do " << energia.max
                 << " PLN | RAZEM: od " << razemMin << " do " << razemMax << " PLN\n\n";
        }

        if (najlepszy == nullptr || razemSrednia < najnizszyKosztSredni) {
            najlepszy = p;
            najnizszyKosztSredni = razemSrednia;
            najlepszyMin = razemMin;
            najlepszyMax = razemMax;
        }
    }

    if (najlepszy == nullptr) {
        cout << "Brak dostepnych pojazdow spelniajacych wybrane kryteria.\n";
        return;
    }

    cout << "REKOMENDACJA DLA KLIENTA: " << najlepszy->getMarka() << " "
         << najlepszy->getModel() << " (ID: " << najlepszy->getId() << ") - LACZNIE: ";
    if (najlepszyMin == najlepszyMax) {
        cout << najlepszyMin << " PLN <<<\n";
    } else {
        cout << "od " << najlepszyMin << " do " << najlepszyMax << " PLN <<<\n";
    }
}