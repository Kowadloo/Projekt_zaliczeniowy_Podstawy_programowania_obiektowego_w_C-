#include <iostream>
#include <string>
#include "Wypozyczalnia.h"
#include "DanePodrozy.h"
#include "Pojazdy.h"
using namespace std;


void czekajNaEnter() {
    cout << "\nNacisnij Enter, aby wrocic do menu";
    string pom;
    getline(cin, pom);
}

Trasa wczytajTraseKlienta() {
    cout << "\nWybierz sposob wprowadzenia trasy:\n";
    cout << "1. Glownie miasto (70% miasto / 30% trasa / 0% autostrada)\n";
    cout << "2. Trasa mieszana (30% miasto / 50% trasa / 20% autostrada)\n";
    cout << "3. Glownie autostrada (20% miasto / 20% trasa / 60% autostrada)\n";
    cout << "4. Wlasny dokladny podzial kilometrow\n";
    cout << "Wybor profilu: ";
    int tryb;
    cin >> tryb;
    cin.ignore(10000, '\n');



    if (tryb >= 1 && tryb <= 3) {
        double km;
        int dni;
        cout << "Podaj szacowany laczny dystans podrozy [km]: ";
        cin >> km;
        cout << "Podaj liczbe dni wynajmu: ";
        cin >> dni;
        cin.ignore(10000, '\n');
        return Trasa(km, static_cast<ProfilTrasy>(tryb), dni);
    }



    double miasto, trasa, autostrada;
    int dni;
    cout << "Dystans w miescie [km]: ";
    cin >> miasto;
    cout << "Dystans w trasie [km]: ";
    cin >> trasa;
    cout << "Dystans na autostradzie [km]: ";
    cin >> autostrada;
    cout << "Podaj liczbe dni wynajmu: ";
    cin >> dni;
    cin.ignore(10000, '\n');
    return Trasa(miasto, trasa, autostrada, dni);
}




int main() {
    cout << fixed;
    cout.precision(2);

    Wypozyczalnia system("Wypozyczalnia ABC", "flota.txt");
    int wybor = -1;

    while (wybor != 0) {
        cout << "\n SYSTEM WYPOZYCZALNI\n";
        cout << "1. Wyswietl stan floty\n";
        cout << "2. Dobierz najtansze auto dla klienta\n";
        cout << "3. Wydaj pojazd klientowi (wypozycz po ID)\n";
        cout << "4. Przyjmij zwrot pojazdu (zwrot po ID)\n";
        cout << "5. Dodaj nowy pojazd do floty\n";
        cout << "6. Usun pojazd z floty\n";
        cout << "7. Zaktualizuj cennik paliw i pradu\n";
        cout << "0. Zakoncz prace\n";
        cout << "Wybierz opcje: ";
        cin >> wybor;
        cin.ignore(10000, '\n');

        switch (wybor) {
            case 1:
                system.wyswietlFlote();
                czekajNaEnter();
                break;
            case 2: {
                int pref;
                cout << "\nPreferencja napedu (0 - Wszystkie, 1 - Spalinowe, 2 - Elektryczne): ";
                cin >> pref;
                cin.ignore(10000, '\n');
                Trasa trasa = wczytajTraseKlienta();
                system.dobierzAutoDlaKlienta(trasa, static_cast<TypNapedu>(pref));
                czekajNaEnter();
                break;
            }
            case 3: {
                system.wyswietlKrotkaListe();
                int id;
                cout << "\nPodaj ID wydawanego pojazdu: ";
                cin >> id;
                cin.ignore(10000, '\n');
                system.wypozyczPojazd(id);
                czekajNaEnter();
                break;
            }
            case 4: {
                system.wyswietlWypozyczone();
                int id;
                cout << "\nPodaj ID zwracanego pojazdu (lub 0 aby anulowac): ";
                cin >> id;
                cin.ignore(10000, '\n');
                if (id != 0) {
                    system.zwrocPojazd(id);
                }
                czekajNaEnter();
                break;
            }
            case 5: {
                int typ, id;
                string marka, model;
                double stawka;
                cout << "\nTyp napedu (1 - Spalinowy, 2 - Elektryczny): ";
                cin >> typ;
                cin.ignore(10000, '\n');
                system.wyswietlWolneId();
                cout << "Nadaj unikalne ID: ";
                cin >> id;
                cin.ignore(10000, '\n');
                cout << "Marka (bez spacji): ";
                cin >> marka;
                cin.ignore(10000, '\n');
                cout << "Model (uzyj _ zamiast spacji, np. Karoq_2.0): ";
                cin >> model;
                cin.ignore(10000, '\n');
                cout << "Stawka dobowa [PLN]: ";
                cin >> stawka;
                cin.ignore(10000, '\n');

                if (typ == 1) {
                    int wyborPaliwa;
                    double sM, sT, sA;
                    cout << "Rodzaj paliwa (1 - Benzyna, 2 - Diesel): ";
                    cin >> wyborPaliwa;
                    string paliwo = (wyborPaliwa == 2) ? "Diesel" : "Benzyna";
                    cout << "Spalanie miasto [l/100km]: ";
                    cin >> sM;
                    cout << "Spalanie trasa [l/100km]: ";
                    cin >> sT;
                    cout << "Spalanie autostrada [l/100km]: ";
                    cin >> sA;
                    cin.ignore(10000, '\n');
                    system.dodajPojazd(new PojazdSpalinowy(id, marka, model, stawka, paliwo, sM, sT, sA));
                } else {
                    double zM, zT, zA, bat;
                    cout << "Zuzycie miasto [kWh/100km]: ";
                    cin >> zM;
                    cout << "Zuzycie trasa [kWh/100km]: ";
                    cin >> zT;
                    cout << "Zuzycie autostrada [kWh/100km]: ";
                    cin >> zA;
                    cout << "Pojemnosc baterii [kWh]: ";
                    cin >> bat;
                    cin.ignore(10000, '\n');
                    system.dodajPojazd(new PojazdElektryczny(id, marka, model, stawka, zM, zT, zA, bat));
                }
                czekajNaEnter();
                break;
            }
            case 6: {
                system.wyswietlKrotkaListe();
                int id;
                cout << "\nPodaj ID pojazdu do usuniecia: ";
                cin >> id;
                cin.ignore(10000, '\n');
                system.usunPojazd(id);
                czekajNaEnter();
                break;
            }
            case 7: {
                Cennik& c = system.getCennik();
                cout << "\nAktualne stawki -> Benzyna: " << c.getCenaBenzyny()
                     << ", Diesel: " << c.getCenaDiesla()
                     << ", Prad min: " << c.getCenaPraduMin()
                     << ", Prad max: " << c.getCenaPraduMax() << "\n";
                double pb, on, pMin, pMax;
                cout << "Nowa cena benzyny [PLN/l]: ";
                cin >> pb;
                cout << "Nowa cena diesla [PLN/l]: ";
                cin >> on;
                cout << "Nowa minimalna cena pradu [PLN/kWh]: ";
                cin >> pMin;
                cout << "Nowa maksymalna cena pradu [PLN/kWh]: ";
                cin >> pMax;
                cin.ignore(10000, '\n');
                c.ustawCeny(pb, on, pMin, pMax);
                cout << "Cennik zaktualizowany.\n";
                czekajNaEnter();
                break;
            }
            case 0:
                cout << "Zamykanie systemu.\n";
                break;
            default:
                cout << "Nieznana opcja menu.\n";
                czekajNaEnter();
                break;
        }
    }

    return 0;
}