/*
    Design pattern-ul folosit este Prototype. Se potriveste proiectului deoarece se pot crea mai multe instrumente muzicale care
    sunt de acelasi tip, prototipul acesta continand si metoda Clone.

 */



#include<iostream>
#include<typeinfo>
#include<exception>
#include<vector>
#include<memory>
#include<map>

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
    return out;
}



template<Instrument I>
class Id_gen{
    static int id_curent;

    std::string ID;
public:
    Id_gen();

    Id_gen(const Id_gen &ob);

    std::string get_ID() const;
};

template<Instrument I>
int Id_gen<I>::id_curent=0;

template<Instrument I>
Id_gen<I>::Id_gen(): ID("Instrument"+std::to_string(id_curent++)){}

template<Instrument I>
Id_gen<I>::Id_gen(const Id_gen &ob): ID("Instrument"+std::to_string(id_curent++)){}

template<Instrument I>
std::string Id_gen<I>::get_ID() const{
    return ID;
}

template<>
Id_gen<Instrument::corzi>::Id_gen():ID("In_C"+std::to_string(id_curent++)) {}

template<>              //Pt Prototype/Metoda Clone
Id_gen<Instrument::corzi>::Id_gen(const Id_gen<Instrument::corzi> &ob):ID("In_C"+std::to_string(id_curent++)) {}


template<>
Id_gen<Instrument::percutie>::Id_gen():ID("In_P"+std::to_string(id_curent++)) {}

template<>              //Pt Prototype/Metoda Clone
Id_gen<Instrument::percutie>::Id_gen(const Id_gen<Instrument::percutie> &ob):ID("In_P"+std::to_string(id_curent++)) {}


template<>
Id_gen<Instrument::clape>::Id_gen():ID("In_Cl"+std::to_string(id_curent++)) {}

template<>              //Pt Prototype/Metoda Clone
Id_gen<Instrument::clape>::Id_gen(const Id_gen<Instrument::clape> &ob):ID("In_Cl"+std::to_string(id_curent++)) {}




class Instrumente:public Printare{
    float pret_producator;
    std::string nume_producator,tip_electr_acus;
public:
    Instrumente(std::string nume_producator,std::string tip, float pret_producator);

    void afisare(std::ostream &out) const override;

    const std::string &getNumeProducator() const;

    void setNumeProducator(const std::string numeProducator);

    const std::string &getTipElectrAcus() const;

    void setTipElectrAcus(const std::string tipElectrAcus);

    float get_Pret_produc() const;

    virtual void set_Pret(float pret);

    virtual std::shared_ptr<Instrumente> Clone() const=0;

    virtual ~Instrumente();
};

Instrumente::Instrumente(std::string nume_producator, std::string tip, float pret_producator):nume_producator(nume_producator), tip_electr_acus(tip), pret_producator(pret_producator){}

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

float Instrumente::get_Pret_produc() const {
    return pret_producator;
}

void Instrumente::set_Pret(float pret) {
    if(pret<=0)
        throw Numar_invalid();
    pret_producator=pret;
}


Instrumente::~Instrumente()=default;





template<Calitate C>
class Instr_corzi: public Instrumente, public Id_gen<Instrument::corzi>{
    int nr_corzi;
public:
    Instr_corzi(const std::string numeproducator, const std::string tip, int nr_corzi, float pret);

    int getNrCorzi() const;

    void setNrCorzi(int nrcorzi);

    void afisare(std::ostream &out) const override;

    std::shared_ptr<Instrumente>Clone() const override;

};

template<Calitate C>
Instr_corzi<C>::Instr_corzi(const std::string numeproducator, const std::string tip, int nr_corzi, float pret):
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
    Instrumente::afisare(out);
    out<<nr_corzi<<" "<<Id_gen<Instrument::corzi>::get_ID();
}

template<>
std::shared_ptr<Instrumente> Instr_corzi<Calitate::Joasa>::Clone() const {
    return std::make_shared<Instr_corzi<Calitate::Joasa>>(*this);
}

template<>
std::shared_ptr<Instrumente> Instr_corzi<Calitate::Medie>::Clone() const {
    return std::make_shared<Instr_corzi<Calitate::Medie>>(*this);
}

template<>
std::shared_ptr<Instrumente> Instr_corzi<Calitate::Inalta>::Clone() const {
    return std::make_shared<Instr_corzi<Calitate::Inalta>>(*this);
}


template<Calitate C>
class Instr_percutie: public Instrumente, public Id_gen<Instrument::percutie>{
        int nr_tobe;
    public:
    Instr_percutie(const std::string numeproducator, const std::string tip, int nr_corzi, float pret);

    int getNrTobe() const;

    void setNrTobe(int nrtobe);

    void afisare(std::ostream &out) const override;

    std::shared_ptr<Instrumente>Clone() const override;

};

template<Calitate C>
Instr_percutie<C>::Instr_percutie(const std::string numeproducator, const std::string tip, int nr_tobe, float pret):
        Instrumente(numeproducator,tip,pret), nr_tobe(nr_tobe){}

template<Calitate C>
int Instr_percutie<C>::getNrTobe() const {
    return nr_tobe;
}

template<Calitate C>
void Instr_percutie<C>::setNrTobe(int nrtobe){
    if(nrtobe<=0)
        throw Numar_invalid();
    nr_tobe=nrtobe;
}

template<Calitate C>
void Instr_percutie<C>::afisare(std::ostream &out) const {
    Instrumente::afisare(out);
    out<<nr_tobe<<" ";
}

template<Calitate C>
std::shared_ptr<Instrumente> Instr_percutie<C>::Clone() const {
    return std::make_shared<Instr_percutie<C>>(*this);
}

template<Calitate C>
class Instr_clape: public Instrumente, public Id_gen<Instrument::clape>{
    int nr_clape;
public:
    Instr_clape(const std::string numeproducator, const std::string tip, int nr_clape, float pret);

    int getNrClape() const;

    void setNrClape(int nrtobe);

    void afisare(std::ostream &out) const override;

    std::shared_ptr<Instrumente>Clone() const override;

};


template<Calitate C>
Instr_clape<C>::Instr_clape(const std::string numeproducator, const std::string tip, int nr_clape, float pret):
        Instrumente(numeproducator,tip,pret), nr_clape(nr_clape){}

template<Calitate C>
int Instr_clape<C>::getNrClape() const {
    return nr_clape;
}

template<Calitate C>
void Instr_clape<C>::setNrClape(int nrclape){
    if(nrclape<=0)
        throw Numar_invalid();
    nr_clape=nrclape;
}

template<Calitate C>
void Instr_clape<C>::afisare(std::ostream &out) const {
    Instrumente::afisare(out);
    out<<nr_clape<<" ";
}

template<Calitate C>
std::shared_ptr<Instrumente> Instr_clape<C>::Clone() const {
    return std::make_shared<Instr_clape<C>>(*this);
}



class Magazin_unic:public Printare{
        std::vector<std::shared_ptr<Instrumente>> stocul_instrumentelor;
        std::map<std::string,float>preturi;
        static Magazin_unic *instance;
        Magazin_unic();
    public:
        static Magazin_unic* getInstance();

        void add_instrument(std::shared_ptr<Instrumente> instr);

        void afisare(std::ostream &out) const override;

        Magazin_unic(Magazin_unic &ob)=delete;

        Magazin_unic &operator=(const Magazin_unic &ob)=delete;
};

Magazin_unic* Magazin_unic::instance= nullptr;

Magazin_unic::Magazin_unic()=default;

Magazin_unic *Magazin_unic::getInstance() {
    if(instance== nullptr)
        instance= new Magazin_unic();
    return instance;
}

void Magazin_unic::add_instrument(std::shared_ptr<Instrumente> instr) {
    stocul_instrumentelor.push_back(instr);

    if(typeid(*instr)==typeid(Instr_corzi<Calitate::Joasa>)){
        auto auxiliar=std::dynamic_pointer_cast<Instr_corzi<Calitate::Joasa>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrCorzi()*(float(50)/100) * auxiliar->get_Pret_produc()*(float(5)/100));
    }

    if(typeid(*instr)==typeid(Instr_corzi<Calitate::Medie>)){
        auto auxiliar=std::dynamic_pointer_cast<Instr_corzi<Calitate::Medie>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrCorzi()*(float(50)/100) * auxiliar->get_Pret_produc()*(float(25)/100));
    }

    if(typeid(*instr)==typeid(Instr_corzi<Calitate::Inalta>)){
        auto auxiliar=std::dynamic_pointer_cast<Instr_corzi<Calitate::Inalta>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrCorzi()*(float(50)/100) * auxiliar->get_Pret_produc()*(float(45)/100));
    }

//Percutie

    if(typeid(*instr)==typeid(Instr_percutie<Calitate::Joasa>)){
        auto auxiliar=std::dynamic_pointer_cast<Instr_percutie<Calitate::Joasa>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrTobe()*(float(70)/100) * auxiliar->get_Pret_produc()*(float(10)/100));
    }

    if(typeid(*instr)==typeid(Instr_percutie<Calitate::Medie>)){
        auto auxiliar=std::dynamic_pointer_cast<Instr_percutie<Calitate::Medie>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrTobe()*(float(70)/100) * auxiliar->get_Pret_produc()*(float(30)/100));
    }

    if(typeid(*instr)==typeid(Instr_percutie<Calitate::Inalta>)){
        auto auxiliar=std::dynamic_pointer_cast<Instr_percutie<Calitate::Inalta>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrTobe()*(float(70)/100) * auxiliar->get_Pret_produc()*(float(50)/100));
    }

//Cu clape

    if(typeid(*instr)==typeid(Instr_clape<Calitate::Joasa>)){
        auto auxiliar=std::dynamic_pointer_cast<Instr_clape<Calitate::Joasa>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrClape()*(float(30)/100) * auxiliar->get_Pret_produc()*(float(15)/100));
    }

    if(typeid(*instr)==typeid(Instr_clape<Calitate::Medie>)){
        auto auxiliar=std::dynamic_pointer_cast<Instr_clape<Calitate::Medie>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrClape()*(float(30)/100) * auxiliar->get_Pret_produc()*(float(35)/100));
    }

    if(typeid(*instr)==typeid(Instr_clape<Calitate::Inalta>)){
        auto auxiliar=std::dynamic_pointer_cast<Instr_clape<Calitate::Inalta>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrClape()*(float(30)/100) * auxiliar->get_Pret_produc()*(float(55)/100));
    }
}

void Magazin_unic::afisare(std::ostream &out) const {
    for(auto &i:stocul_instrumentelor){
        i->afisare(out);
        std::cout<<"\n";
    }
    for(auto &i:preturi)
        std::cout<<i.first<<" "<<i.second<<"\n";
}






int main(){
    auto ob=Instr_corzi<Calitate::Joasa>("an","electrc",5,6);
    auto ob2=Instr_corzi<Calitate::Medie>("te","va",6,8);
    auto ob3=Instr_percutie<Calitate::Medie>("te","va",6,8);
    ob.setTipElectrAcus("electric");

    if(typeid(ob)== typeid(Instr_corzi<Calitate::Joasa>))
        std::cout<<"da "<<ob.get_ID()<<" "<<ob3.get_ID()<<"\n";

    std::cout<< typeid(Instr_corzi<Calitate::Joasa>).name()<<"\n";



    auto p=Magazin_unic::getInstance();
    p->add_instrument(std::make_shared<Instr_corzi<Calitate::Joasa>>("ana","vrea",5,6));
    auto pt=std::make_shared<Instr_corzi<Calitate::Medie>>("asf","asf",50,20);
    p->add_instrument(pt);
    std::cout<<*p;

    auto x=pt->Clone();
    auto y=pt->Clone();
    std::cout<<"\n"<<*x<<"\n"<<*y;


//  to be continued...
}
