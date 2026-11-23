#pragma once
#include <vector>
#include <string>
#include "Pojazdy.h"
#include "DanePodrozy.h"



class Wypozyczalnia {
private:
    std::string nazwa;
    std::string plikFloty;
    std::vector<Pojazd*> flota;
    Cennik cennik;

    Pojazd* znajdzPoId(int id) const;
    void wyczyscFlote();
    void zapiszFloteDoPliku() const;
    void wczytajFloteZPliku();

public:
    Wypozyczalnia(const std::string& nazwa = "Wypozyczalnia ABC", const std::string& plikFloty = "flota.txt");
    ~Wypozyczalnia();

    Wypozyczalnia(const Wypozyczalnia&) = delete;
    Wypozyczalnia& operator=(const Wypozyczalnia&) = delete;

    const Cennik& getCennik() const;
    void aktualizujCennik(double benzyna, double diesel, double prad);

    void dodajPojazd(Pojazd* nowyPojazd);
    void usunPojazd(int id);
    void wypozyczPojazd(int id);
    void zwrocPojazd(int id);

    void wyswietlFlote() const;
    void wyswietlWypozyczone() const;
    void dobierzAutoDlaKlienta(const Trasa& trasa, int preferencjaNapedu) const;
};