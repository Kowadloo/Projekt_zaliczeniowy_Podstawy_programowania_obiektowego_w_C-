#pragma once
#include <vector>
#include <string>
#include "Pojazdy.h"
#include "DanePodrozy.h"

using namespace std;





class Wypozyczalnia {
private:
    string nazwa;
    string plikFloty;
    vector<Pojazd*> flota;
    Cennik cennik;

    void wyczyscFlote();
    void zapiszFloteDoPliku() const;
    void wczytajFloteZPliku();

public:
    Wypozyczalnia(const string& nazwa = "AutoFlota PRO", const string& plikFloty = "flota.txt");
    ~Wypozyczalnia();

    Wypozyczalnia(const Wypozyczalnia&) = delete;
    Wypozyczalnia& operator=(const Wypozyczalnia&) = delete;

    Cennik& getCennik();
    const Cennik& getCennik() const;

    void dodajPojazd(Pojazd* nowyPojazd);
    void usunPojazd(int id);
    void wypozyczPojazd(int id);
    void zwrocPojazd(int id);

    void wyswietlFlote() const;
    void wyswietlKrotkaListe() const;
    void wyswietlWypozyczone() const;
    void wyswietlWolneId() const;
    void dobierzAutoDlaKlienta(const Trasa& trasa, TypNapedu preferencja) const;
};