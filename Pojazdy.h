#pragma once
#include <string>
#include "DanePodrozy.h"
using namespace std;

class Pojazd {
protected:
    int id;
    string marka;
    string model;
    double stawkaDobowa;
    bool wypozyczony;

public:
    Pojazd();
    Pojazd(int id, const string& marka, const string& model, double stawkaDobowa);
    virtual ~Pojazd() = default;

    int getId() const;
    string getMarka() const;
    string getModel() const;
    double getStawkaDobowa() const;
    bool isWypozyczony() const;

    void setStawkaDobowa(double stawka);
    void setWypozyczony(bool status);

    virtual double obliczKosztPaliwa(const Trasa& trasa, const Cennik& cennik) const = 0;
    virtual string getTypNapedu() const = 0;
    virtual void wyswietl() const;
};

class PojazdSpalinowy : public Pojazd {
private:
    string rodzajPaliwa; // Benzyna lub DDisel
    double spalanieMiasto;
    double spalanieTrasa;
    double spalanieAutostrada;

public:
    PojazdSpalinowy();
    PojazdSpalinowy(int id, const string& marka, const string& model, double stawkaDobowa,
                    const string& rodzajPaliwa, double spalanieMiasto,
                    double spalanieTrasa, double spalanieAutostrada);

    double obliczKosztPaliwa(const Trasa& trasa, const Cennik& cennik) const override;
    string getTypNapedu() const override;
    void wyswietl() const override;
};

class PojazdElektryczny : public Pojazd {
private:
    double zuzycieMiasto;
    double zuzycieTrasa;
    double zuzycieAutostrada;
    double pojemnoscBaterii;

public:
    PojazdElektryczny();
    PojazdElektryczny(int id, const string& marka, const string& model, double stawkaDobowa,
                      double zuzycieMiasto, double zuzycieTrasa,
                      double zuzycieAutostrada, double pojemnoscBaterii);

    double obliczKosztPaliwa(const Trasa& trasa, const Cennik& cennik) const override;
    string getTypNapedu() const override;
    void wyswietl() const override;
};