#include<iostream>
#include<typeinfo>
#include<exception>

enum class Calitate{
    Joasa,
    Medie,
    Inalta
};

enum class Instrument{
    corzi,
    percutie,
    clape
};


//O excpetie
//
//class nu_are_corzi:public std::exception{
//    public:
//
//};


class Numar_invalid: public std::exception{
public:
    const char * what() const noexcept override;
};

const char *Numar_invalid::what() const noexcept {
    return "Numarul ester invalid";
}


class Tip_invalid: public std::exception{
public:
    const char * what() const noexcept override;
};

const char *Tip_invalid::what() const noexcept {
    return "Tipul ester invalid";
}


class Printare{
public:
    Printare();

    virtual void afisare(std::ostream &out) const=0;

    friend std::ostream &operator<<(std::ostream &out, Printare &ob);
};

Printare::Printare()=default;

std::ostream &operator<<(std::ostream &out, Printare &ob){
    ob.afisare(out);
}



template<Instrument I>
class Id_gen{
    static int id_curent;

    std::string ID;
public:
    Id_gen();

    std::string get_ID();
};

template<Instrument I>
int Id_gen<I>::id_curent=0;

template<Instrument I>
Id_gen<I>::Id_gen(): ID("Instrument"+std::to_string(id_curent++)){}

template<Instrument I>
std::string Id_gen<I>::get_ID() {
    return ID;
}


template<>
Id_gen<Instrument::corzi>::Id_gen():ID("In_C"+std::to_string(id_curent++)) {}

template<>
Id_gen<Instrument::percutie>::Id_gen():ID("In_P"+std::to_string(id_curent++)) {}

template<>
Id_gen<Instrument::clape>::Id_gen():ID("In_Cl"+std::to_string(id_curent++)) {}



class Instrumente:public Printare{
    double pret_producator;
    std::string nume_producator,tip_electr_acus;
public:
    Instrumente(std::string nume_producator,std::string tip, double pret_producator);

    void afisare(std::ostream &out) const override;

    const std::string &getNumeProducator() const;

    void setNumeProducator(const std::string numeProducator);

    const std::string &getTipElectrAcus() const;

    void setTipElectrAcus(const std::string tipElectrAcus);

    double get_Pret_produc() const;

    virtual void set_Pret()=0;

    virtual ~Instrumente();
};

Instrumente::Instrumente(std::string nume_producator, std::string tip, double pret_producator):nume_producator(nume_producator), tip_electr_acus(tip), pret_producator(pret_producator){}

void Instrumente::afisare(std::ostream &out) const {
    out<<nume_producator<<" "<<tip_electr_acus<<" ";
}

const std::string &Instrumente::getNumeProducator() const {
    return nume_producator;
}

void Instrumente::setNumeProducator(const std::string numeProducator) {
    nume_producator = numeProducator;
}

const std::string &Instrumente::getTipElectrAcus() const {
    return tip_electr_acus;
}

void Instrumente::setTipElectrAcus(const std::string tipElectrAcus) {
    if(tipElectrAcus!="acustic" and tipElectrAcus!="electric" and tipElectrAcus!="electro-acustic")
        throw Tip_invalid();
    tip_electr_acus = tipElectrAcus;
}

double Instrumente::get_Pret_produc() const {
    return pret_producator;
}


Instrumente::~Instrumente()=default;





template<Calitate C>
class Instr_corzi: public Instrumente, public Id_gen<Instrument::corzi>{
    int nr_corzi; double pret_magazin;
public:
    Instr_corzi(const std::string numeproducator, const std::string tip, int nr_corzi, double pret);

    int getNrCorzi() const;

    void setNrCorzi(int nrcorzi);

    void afisare(std::ostream &out) const override;

    void set_Pret() override;
};

template<Calitate C>
Instr_corzi<C>::Instr_corzi(const std::string numeproducator, const std::string tip, int nr_corzi, double pret):
    Instrumente(numeproducator,tip,pret), nr_corzi(nr_corzi){}

template<Calitate C>
int Instr_corzi<C>::getNrCorzi() const {
    return nr_corzi;
}

template<Calitate C>
void Instr_corzi<C>::setNrCorzi(int nrcorzi){
    if(nrcorzi<=0)
        throw Numar_invalid();
    nr_corzi=nrcorzi;
}

template<Calitate C>
void Instr_corzi<C>::afisare(std::ostream &out) const {
    Instrumente::afisare();
    out<<nr_corzi<<" ";
}


template<>
void Instr_corzi<Calitate::Joasa>::set_Pret() {
    pret_magazin=get_Pret_produc()+get_Pret_produc()*(10/100);
}
template<>
void Instr_corzi<Calitate::Medie>::set_Pret() {
    pret_magazin=get_Pret_produc()+get_Pret_produc()*(30/100);
}
template<>
void Instr_corzi<Calitate::Inalta>::set_Pret() {
    pret_magazin=get_Pret_produc()+get_Pret_produc()*(50/100);
}




template<Calitate C>
class Instr_percutie: public Instrumente, public Id_gen<Instrument::percutie>{

};


template<Calitate C>
class Instr_clape: public Instrumente, public Id_gen<Instrument::clape>{

};



int main(){
}






//Instrumente
//Chitaristi cu template un vector ce retine id_ul chitarei, id_gen ?????
//Tobosari cu template la fel ca mai sus, id_gen
