#pragma once
#include <iostream>
#include <string>
#include "DanePodrozy.h"

using namespace std;




enum class TypNapedu {
    Wszystkie = 0,
    Spalinowy = 1,
    Elektryczny = 2
};

struct KosztEnergii {
    double min;
    double max;
};

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

    void setWypozyczony(bool status);

    virtual KosztEnergii obliczKosztEnergii(const Trasa& trasa, const Cennik& cennik) const = 0;
    virtual string getOpisNapedu() const = 0;
    virtual TypNapedu getTypNapedu() const = 0;
    virtual string zapiszDoLinii() const = 0;
    virtual void wypiszSzczegoly(ostream& os) const;

    bool operator==(const Pojazd& inny) const;
};

ostream& operator<<(ostream& os, const Pojazd& pojazd);

class PojazdSpalinowy : public Pojazd {
private:
    string rodzajPaliwa;
    double spalanieMiasto;
    double spalanieTrasa;
    double spalanieAutostrada;

public:
    PojazdSpalinowy();
    PojazdSpalinowy(int id, const string& marka, const string& model, double stawkaDobowa,
                    const string& rodzajPaliwa, double spalanieMiasto,
                    double spalanieTrasa, double spalanieAutostrada);

    KosztEnergii obliczKosztEnergii(const Trasa& trasa, const Cennik& cennik) const override;
    string getOpisNapedu() const override;
    TypNapedu getTypNapedu() const override;
    string zapiszDoLinii() const override;
    void wypiszSzczegoly(ostream& os) const override;
};

class PojazdElektryczny : public Pojazd {
private:
    double zuzycieMiasto;
    double zuzycieTrasa;
    double zuzycieAutostrada;
    double pojemnoscBaterii;

    double obliczZuzyteKwh(const Trasa& trasa) const;

public:
    PojazdElektryczny();
    PojazdElektryczny(int id, const string& marka, const string& model, double stawkaDobowa,
                      double zuzycieMiasto, double zuzycieTrasa,
                      double zuzycieAutostrada, double pojemnoscBaterii);

    KosztEnergii obliczKosztEnergii(const Trasa& trasa, const Cennik& cennik) const override;
    string getOpisNapedu() const override;
    TypNapedu getTypNapedu() const override;
    string zapiszDoLinii() const override;
    void wypiszSzczegoly(ostream& os) const override;
};