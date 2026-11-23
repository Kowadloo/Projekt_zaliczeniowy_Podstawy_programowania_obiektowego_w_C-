#include <iostream>
#include <string>
#include <stdexcept>
#include "Wypozyczalnia.h"
#include "DanePodrozy.h"
#include "Pojazdy.h"

using namespace std;



int wczytajInt(const string& tekst) {
    int wartosc;
    cout << tekst;
    if (!(cin >> wartosc)) {
        cin.clear();
        cin.ignore(10000, '\n');
        throw invalid_argument("Wprowadzono niepoprawna liczbe calkowita.");
    }
    cin.ignore(10000, '\n');
    return wartosc;
}

string wczytajTekst(const string& tekst) {
    string linia;
    cout << tekst;
    getline(cin, linia);
    return linia;
}

double wczytajDouble(const string& tekst) {
    string linia = wczytajTekst(tekst);
    for (char& c : linia) {
        if (c == ',') {
            c = '.';
        }
    }
    size_t ile = 0;
    double wartosc = 0.0;
    try {
        wartosc = stod(linia, &ile);
    } catch (const exception&) {
        throw invalid_argument("Wprowadzono niepoprawna wartosc liczbowa.");
    }
    if (ile != linia.size()) {
        throw invalid_argument("Wprowadzono niepoprawna wartosc liczbowa.");
    }
    return wartosc;
}

void czekajNaEnter() {
    cout << "\nNacisnij Enter, aby wrocic do menu...";
    string pom;
    getline(cin, pom);
}

void dodajPojazdInteraktywnie(Wypozyczalnia& wypozyczalnia) {
    wypozyczalnia.wyswietlFlote();
    int id = wczytajInt("\nNadaj unikalne ID: ");
    string marka = wczytajTekst("Marka (bez spacji): ");
    string model = wczytajTekst("Model (uzyj _ zamiast spacji, np. Karoq_2.0): ");
    double stawka = wczytajDouble("Stawka dobowa [PLN]: ");
    double kaucja = wczytajDouble("Kaucja zwrotna [PLN]: ");

    int zasilanie = wczytajInt("Rodzaj zasilania (1 - Benzyna, 2 - Diesel, 3 - Elektryczny): ");
    if (zasilanie < 1 || zasilanie > 3) {
        throw invalid_argument("Niepoprawny wybor rodzaju zasilania.");
    }

    double zM = wczytajDouble("Zuzycie miasto [l lub kWh / 100km]: ");
    double zT = wczytajDouble("Zuzycie trasa [l lub kWh / 100km]: ");
    double zA = wczytajDouble("Zuzycie autostrada [l lub kWh / 100km]: ");

    if (zasilanie == 3) {
        double bat = wczytajDouble("Pojemnosc baterii [kWh]: ");
        wypozyczalnia.dodajPojazd(new PojazdElektryczny(id, marka, model, stawka, kaucja, zM, zT, zA, bat));
    } else {
        string paliwo = (zasilanie == 1) ? "Benzyna" : "Diesel";
        wypozyczalnia.dodajPojazd(new PojazdSpalinowy(id, marka, model, stawka, kaucja, paliwo, zM, zT, zA));
    }
    cout << "Pojazd dodany i zapisany w bazie.\n";
}

int main() {
    cout << fixed;
    cout.precision(2);

    try {
        Wypozyczalnia wypozyczalnia;
        int wybor = -1;

        while (wybor != 0 && cin) {
            cout << "\nSystem pracownika wypozyczalni:\n";
            cout << "1. Wyswietl stan floty\n";
            cout << "2. Dobierz najtansze auto dla klienta\n";
            cout << "3. Wydaj pojazd klientowi (wypozycz po ID)\n";
            cout << "4. Przyjmij zwrot pojazdu (zwrot po ID)\n";
            cout << "5. Dodaj nowy pojazd do floty\n";
            cout << "6. Usun pojazd z floty\n";
            cout << "7. Zaktualizuj cennik paliw i pradu\n";
            cout << "0. Zakoncz prace\n";

            try {
                wybor = wczytajInt("Wybierz opcje: ");

                switch (wybor) {
                    case 1:
                        wypozyczalnia.wyswietlFlote();
                        czekajNaEnter();
                        break;
                    case 2: {
                        int preferencja = wczytajInt("\nPreferencja napedu (0 - Wszystkie, 1 - Spalinowe, 2 - Elektryczne): ");
                        cout << "\nProfil trasy:\n";
                        cout << "1. Miasto (70% miasto / 30% trasa / 0% autostrada)\n";
                        cout << "2. Ogolny (34% miasto / 33% trasa / 33% autostrada)\n";
                        cout << "3. Miedzymiastowa (15% miasto / 35% trasa / 50% autostrada)\n";
                        int profil = wczytajInt("Wybor profilu: ");
                        double km = wczytajDouble("Szacowany dystans podrozy [km]: ");
                        int dni = wczytajInt("Liczba dni wynajmu: ");

                        Trasa trasa(km, profil, dni);
                        wypozyczalnia.dobierzAutoDlaKlienta(trasa, preferencja);
                        czekajNaEnter();
                        break;
                    }
                    case 3: {
                        wypozyczalnia.wyswietlFlote();
                        int id = wczytajInt("\nPodaj ID wydawanego pojazdu: ");
                        wypozyczalnia.wypozyczPojazd(id);
                        cout << "Pojazd o ID " << id << " zostal wydany klientowi.\n";
                        czekajNaEnter();
                        break;
                    }
                    case 4: {
                        wypozyczalnia.wyswietlWypozyczone();
                        int id = wczytajInt("\nPodaj ID zwracanego pojazdu: ");
                        wypozyczalnia.zwrocPojazd(id);
                        cout << "Pojazd o ID " << id << " wrocil na plac.\n";
                        czekajNaEnter();
                        break;
                    }
                    case 5:
                        dodajPojazdInteraktywnie(wypozyczalnia);
                        czekajNaEnter();
                        break;
                    case 6: {
                        wypozyczalnia.wyswietlFlote();
                        int id = wczytajInt("\nPodaj ID pojazdu do usuniecia: ");
                        wypozyczalnia.usunPojazd(id);
                        cout << "Pojazd o ID " << id << " zostal usuniety z bazy.\n";
                        czekajNaEnter();
                        break;
                    }
                    case 7: {
                        const Cennik& c = wypozyczalnia.getCennik();
                        cout << "\nAktualne stawki -> Benzyna: " << c.getCenaBenzyny()
                             << " PLN, Diesel: " << c.getCenaDiesla()
                             << " PLN, Prad: " << c.getCenaPradu() << " PLN\n";
                        double pb = wczytajDouble("Nowa cena benzyny [PLN/l]: ");
                        double on = wczytajDouble("Nowa cena diesla [PLN/l]: ");
                        double pr = wczytajDouble("Nowa cena pradu [PLN/kWh]: ");
                        wypozyczalnia.aktualizujCennik(pb, on, pr);
                        cout << "Cennik zostal zaktualizowany.\n";
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
            } catch (const exception& e) {
                cout << "\n[BLAD] " << e.what() << "\n";
                czekajNaEnter();
            }
        }
    } catch (const exception& e) {
        cout << "\n[BLAD KRYTYCZNY] " << e.what() << "\n";
        return 1;
    }

    return 0;
}