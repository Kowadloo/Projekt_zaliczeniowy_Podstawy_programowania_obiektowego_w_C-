#pragma once
#include <iostream>
#include <string>
#include "DanePodrozy.h"



class Pojazd {
protected:
    int id;
    std::string marka;
    std::string model;
    double stawkaDobowa;
    double kaucja;
    bool wypozyczony;

    virtual char getTypPliku() const = 0;
    virtual void dopiszSzczegolyDoLinii(std::ostream& os) const = 0;

public:
    Pojazd(int id, const std::string& marka, const std::string& model, double stawkaDobowa, double kaucja);
    virtual ~Pojazd() = default;

    int getId() const;
    std::string getMarka() const;
    std::string getModel() const;
    double getStawkaDobowa() const;
    double getKaucja() const;
    bool isWypozyczony() const;

    void setWypozyczony(bool status);

    virtual double obliczKosztEnergii(const Trasa& trasa, const Cennik& cennik) const = 0;
    virtual std::string getOpisNapedu() const = 0;
    virtual int getKodNapedu() const = 0;
    virtual std::string getDodatkoweInfoTrasy(const Trasa& trasa) const;
    virtual void wypiszSzczegoly(std::ostream& os) const;

    void zapiszDoPliku(std::ostream& os) const;
    bool operator==(const Pojazd& inny) const;
};

std::ostream& operator<<(std::ostream& os, const Pojazd& pojazd);

class PojazdSpalinowy : public Pojazd {
private:
    std::string rodzajPaliwa;
    double spalanieMiasto;
    double spalanieTrasa;
    double spalanieAutostrada;

protected:
    char getTypPliku() const override;
    void dopiszSzczegolyDoLinii(std::ostream& os) const override;

public:
    PojazdSpalinowy(int id, const std::string& marka, const std::string& model, double stawkaDobowa,
                    double kaucja, const std::string& rodzajPaliwa, double spalanieMiasto,
                    double spalanieTrasa, double spalanieAutostrada);

    double obliczKosztEnergii(const Trasa& trasa, const Cennik& cennik) const override;
    std::string getOpisNapedu() const override;
    int getKodNapedu() const override;
    void wypiszSzczegoly(std::ostream& os) const override;
};

class PojazdElektryczny : public Pojazd {
private:
    double zuzycieMiasto;
    double zuzycieTrasa;
    double zuzycieAutostrada;
    double pojemnoscBaterii;

    double obliczZuzyteKwh(const Trasa& trasa) const;

protected:
    char getTypPliku() const override;
    void dopiszSzczegolyDoLinii(std::ostream& os) const override;

public:
    PojazdElektryczny(int id, const std::string& marka, const std::string& model, double stawkaDobowa,
                      double kaucja, double zuzycieMiasto, double zuzycieTrasa,
                      double zuzycieAutostrada, double pojemnoscBaterii);

    double obliczKosztEnergii(const Trasa& trasa, const Cennik& cennik) const override;
    std::string getOpisNapedu() const override;
    int getKodNapedu() const override;
    std::string getDodatkoweInfoTrasy(const Trasa& trasa) const override;
    void wypiszSzczegoly(std::ostream& os) const override;
};