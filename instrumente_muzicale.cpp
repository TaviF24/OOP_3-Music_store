/*
    Design pattern-urile folosite sunt Prototype si Factory Method.
    Prototype se potriveste proiectului deoarece se pot crea mai multe instrumente muzicale care sunt de acelasi tip, prototipul
    acesta continand si metoda Clone.
    Factory Method se potriveste proiectului deoarece un client isi poate comanda un anumit intrument direct de la furnizor,
    furnizorul ocupandu-se cu obtinerea produsului cerut.
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
    chitare,
    tobe,
    orgi
};




class Numar_invalid: public std::exception{
public:
    const char * what() const noexcept override;
};

const char *Numar_invalid::what() const noexcept {
    return "Numarul este invalid\n";
}


class Tip_invalid: public std::exception{
public:
    const char * what() const noexcept override;
};

const char *Tip_invalid::what() const noexcept {
    return "Tipul este invalid\n";
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
Id_gen<Instrument::chitare>::Id_gen():ID("In_C"+std::to_string(id_curent++)) {}

template<>              //Pt Prototype/Metoda Clone
Id_gen<Instrument::chitare>::Id_gen(const Id_gen<Instrument::chitare> &ob):ID("In_C"+std::to_string(id_curent++)) {}


template<>
Id_gen<Instrument::tobe>::Id_gen():ID("In_T"+std::to_string(id_curent++)) {}

template<>              //Pt Prototype/Metoda Clone
Id_gen<Instrument::tobe>::Id_gen(const Id_gen<Instrument::tobe> &ob):ID("In_T"+std::to_string(id_curent++)) {}


template<>
Id_gen<Instrument::orgi>::Id_gen():ID("In_O"+std::to_string(id_curent++)) {}

template<>              //Pt Prototype/Metoda Clone
Id_gen<Instrument::orgi>::Id_gen(const Id_gen<Instrument::orgi> &ob):ID("In_O"+std::to_string(id_curent++)) {}




class Instrumente:public Printare{
    float pret_Producator;
    std::string nume_Producator,tip_electr_acus;
public:
    Instrumente(std::string nume_Producator,std::string tip, float pret_Producator);

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

Instrumente::Instrumente(std::string nume_Producator, std::string tip, float pret_Producator):nume_Producator(nume_Producator), tip_electr_acus(tip), pret_Producator(pret_Producator){}

void Instrumente::afisare(std::ostream &out) const {
    out<<"Nume_producator:"<<nume_Producator<<"\nTip (electric/acustic/electro-acustic):"<<tip_electr_acus<<"\n";
}

const std::string &Instrumente::getNumeProducator() const {
    return nume_Producator;
}

void Instrumente::setNumeProducator(const std::string numeProducator) {
    nume_Producator = numeProducator;
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
    return pret_Producator;
}

void Instrumente::set_Pret(float pret) {
    if(pret<=0)
        throw Numar_invalid();
    pret_Producator=pret;
}


Instrumente::~Instrumente()=default;





template<Calitate C>
class Chitare: public Instrumente, public Id_gen<Instrument::chitare>{
    int nr_corzi;
public:
    Chitare(const std::string numeProducator, const std::string tip, int nr_corzi, float pret);

    int getNrCorzi() const;

    void setNrCorzi(int nrcorzi);

    void afisare(std::ostream &out) const override;

    std::shared_ptr<Instrumente>Clone() const override;

};

template<Calitate C>
Chitare<C>::Chitare(const std::string numeProducator, const std::string tip, int nr_corzi, float pret):
    Instrumente(numeProducator,tip,pret), nr_corzi(nr_corzi){}

template<Calitate C>
int Chitare<C>::getNrCorzi() const {
    return nr_corzi;
}

template<Calitate C>
void Chitare<C>::setNrCorzi(int nrcorzi){
    if(nrcorzi<=0)
        throw Numar_invalid();
    nr_corzi=nrcorzi;
}

template<Calitate C>
void Chitare<C>::afisare(std::ostream &out) const {
    out<<"\nChitara\n";
    Instrumente::afisare(out);
    out<<"Numar corzi:"<<nr_corzi<<"\nID chitara:"<<Id_gen<Instrument::chitare>::get_ID()<<"\n";
}

template<>
std::shared_ptr<Instrumente> Chitare<Calitate::Joasa>::Clone() const {
    return std::make_shared<Chitare<Calitate::Joasa>>(*this);
}

template<>
std::shared_ptr<Instrumente> Chitare<Calitate::Medie>::Clone() const {
    return std::make_shared<Chitare<Calitate::Medie>>(*this);
}

template<>
std::shared_ptr<Instrumente> Chitare<Calitate::Inalta>::Clone() const {
    return std::make_shared<Chitare<Calitate::Inalta>>(*this);
}


template<Calitate C>
class Tobe: public Instrumente, public Id_gen<Instrument::tobe>{
        int nr_tobe;
    public:
    Tobe(const std::string numeProducator, const std::string tip, int nr_tobe, float pret);

    int getNrTobe() const;

    void setNrTobe(int nrtobe);

    void afisare(std::ostream &out) const override;

    std::shared_ptr<Instrumente>Clone() const override;

};

template<Calitate C>
Tobe<C>::Tobe(const std::string numeProducator, const std::string tip, int nr_tobe, float pret):
        Instrumente(numeProducator,tip,pret), nr_tobe(nr_tobe){}

template<Calitate C>
int Tobe<C>::getNrTobe() const {
    return nr_tobe;
}

template<Calitate C>
void Tobe<C>::setNrTobe(int nrtobe){
    if(nrtobe<=0)
        throw Numar_invalid();
    nr_tobe=nrtobe;
}

template<Calitate C>
void Tobe<C>::afisare(std::ostream &out) const {
    out<<"\nToba\n";
    Instrumente::afisare(out);
    out<<"Numar tobe:"<<nr_tobe<<"\nID toba:"<<Id_gen<Instrument::tobe>::get_ID()<<"\n";
}

template<>
std::shared_ptr<Instrumente> Tobe<Calitate::Joasa>::Clone() const {
    return std::make_shared<Tobe<Calitate::Joasa>>(*this);
}

template<>
std::shared_ptr<Instrumente> Tobe<Calitate::Medie>::Clone() const {
    return std::make_shared<Tobe<Calitate::Medie>>(*this);
}

template<>
std::shared_ptr<Instrumente> Tobe<Calitate::Inalta>::Clone() const {
    return std::make_shared<Tobe<Calitate::Inalta>>(*this);
}

template<Calitate C>
class Orgi: public Instrumente, public Id_gen<Instrument::orgi>{
    int nr_clape;
public:
    Orgi(const std::string numeProducator, const std::string tip, int nr_clape, float pret);

    int getNrClape() const;

    void setNrClape(int nrtobe);

    void afisare(std::ostream &out) const override;

    std::shared_ptr<Instrumente>Clone() const override;

};


template<Calitate C>
Orgi<C>::Orgi(const std::string numeProducator, const std::string tip, int nr_clape, float pret):
        Instrumente(numeProducator,tip,pret), nr_clape(nr_clape){}

template<Calitate C>
int Orgi<C>::getNrClape() const {
    return nr_clape;
}

template<Calitate C>
void Orgi<C>::setNrClape(int nrclape){
    if(nrclape<=0)
        throw Numar_invalid();
    nr_clape=nrclape;
}

template<Calitate C>
void Orgi<C>::afisare(std::ostream &out) const {
    out<<"\nOrga\n";
    Instrumente::afisare(out);
    out<<"Numar clape:"<<nr_clape<<"\nID orga:"<<Id_gen<Instrument::orgi>::get_ID()<<"\n";
}

template<>
std::shared_ptr<Instrumente> Orgi<Calitate::Joasa>::Clone() const {
    return std::make_shared<Orgi<Calitate::Joasa>>(*this);
}

template<>
std::shared_ptr<Instrumente> Orgi<Calitate::Medie>::Clone() const {
    return std::make_shared<Orgi<Calitate::Medie>>(*this);
}

template<>
std::shared_ptr<Instrumente> Orgi<Calitate::Inalta>::Clone() const {
    return std::make_shared<Orgi<Calitate::Inalta>>(*this);
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

    if(typeid(*instr)==typeid(Chitare<Calitate::Joasa>)){
        auto auxiliar=std::dynamic_pointer_cast<Chitare<Calitate::Joasa>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrCorzi()*(float(50)/100) * auxiliar->get_Pret_produc()*(float(5)/100));
    }

    if(typeid(*instr)==typeid(Chitare<Calitate::Medie>)){
        auto auxiliar=std::dynamic_pointer_cast<Chitare<Calitate::Medie>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrCorzi()*(float(50)/100) * auxiliar->get_Pret_produc()*(float(25)/100));
    }

    if(typeid(*instr)==typeid(Chitare<Calitate::Inalta>)){
        auto auxiliar=std::dynamic_pointer_cast<Chitare<Calitate::Inalta>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrCorzi()*(float(50)/100) * auxiliar->get_Pret_produc()*(float(45)/100));
    }

//Percutie

    if(typeid(*instr)==typeid(Tobe<Calitate::Joasa>)){
        auto auxiliar=std::dynamic_pointer_cast<Tobe<Calitate::Joasa>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrTobe()*(float(70)/100) * auxiliar->get_Pret_produc()*(float(10)/100));
    }

    if(typeid(*instr)==typeid(Tobe<Calitate::Medie>)){
        auto auxiliar=std::dynamic_pointer_cast<Tobe<Calitate::Medie>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrTobe()*(float(70)/100) * auxiliar->get_Pret_produc()*(float(30)/100));
    }

    if(typeid(*instr)==typeid(Tobe<Calitate::Inalta>)){
        auto auxiliar=std::dynamic_pointer_cast<Tobe<Calitate::Inalta>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrTobe()*(float(70)/100) * auxiliar->get_Pret_produc()*(float(50)/100));
    }

//Cu clape

    if(typeid(*instr)==typeid(Orgi<Calitate::Joasa>)){
        auto auxiliar=std::dynamic_pointer_cast<Orgi<Calitate::Joasa>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrClape()*(float(30)/100) * auxiliar->get_Pret_produc()*(float(15)/100));
    }

    if(typeid(*instr)==typeid(Orgi<Calitate::Medie>)){
        auto auxiliar=std::dynamic_pointer_cast<Orgi<Calitate::Medie>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrClape()*(float(30)/100) * auxiliar->get_Pret_produc()*(float(35)/100));
    }

    if(typeid(*instr)==typeid(Orgi<Calitate::Inalta>)){
        auto auxiliar=std::dynamic_pointer_cast<Orgi<Calitate::Inalta>>(instr);
        preturi[auxiliar->get_ID()]=auxiliar->get_Pret_produc()+(auxiliar->getNrClape()*(float(30)/100) * auxiliar->get_Pret_produc()*(float(55)/100));
    }
}

void Magazin_unic::afisare(std::ostream &out) const {
    for(auto &i:stocul_instrumentelor){
        i->afisare(out);
        std::cout<<"\n";
    }
    for(auto &i:preturi)
        std::cout<<"Id: "<<i.first<<"; Pret:"<<i.second<<" RON\n";
}



class Furnizor{
public:
    virtual ~Furnizor();

    virtual std::shared_ptr<Instrumente> FactoryMethod(std::string nume_Producator,std::string tip, float pret_Producator) const = 0;

    void SomeOperation(std::string nume_Producator,std::string tip, float pret_Producator) const;
};

Furnizor::~Furnizor()=default;

void Furnizor::SomeOperation(std::string nume_Producator, std::string tip, float pret_Producator) const {
    auto product = FactoryMethod(nume_Producator,tip,pret_Producator);
    std::cout<<"Produsul este gata!"<<*product<<"\n";
}


template<Calitate C>
class Furnizor_chitare : public Furnizor {
public:
    std::shared_ptr<Instrumente> FactoryMethod(std::string nume_Producator,std::string tip, float pret_Producator) const override;
};

template<Calitate C>
std::shared_ptr<Instrumente>
Furnizor_chitare<C>::FactoryMethod(std::string nume_Producator, std::string tip, float pret_Producator) const {
    return std::make_shared<Chitare<C>>(nume_Producator,tip,6,pret_Producator);
}

template<Calitate C>
class Furnizor_tobe : public Furnizor {
public:
    std::shared_ptr<Instrumente> FactoryMethod(std::string nume_Producator,std::string tip, float pret_Producator) const override;
};

template<Calitate C>
std::shared_ptr<Instrumente>
Furnizor_tobe<C>::FactoryMethod(std::string nume_Producator, std::string tip, float pret_Producator) const {
    return std::make_shared<Tobe<C>>(nume_Producator,tip,8,pret_Producator);
}

template<Calitate C>
class Furnizor_orgi : public Furnizor {
public:
    std::shared_ptr<Instrumente> FactoryMethod(std::string nume_Producator,std::string tip, float pret_Producator) const override;
};

template<Calitate C>
std::shared_ptr<Instrumente>
Furnizor_orgi<C>::FactoryMethod(std::string nume_Producator, std::string tip, float pret_Producator) const {
    return std::make_shared<Orgi<C>>(nume_Producator,tip,160,pret_Producator);
}


class Client{
public:
       Client();

       void comanda_speciala(const Furnizor& furnizor, std::string nume_Producator,std::string tip, float pret_Producator) const;

};

Client::Client()=default;

void Client::comanda_speciala(const Furnizor &furnizor, std::string nume_Producator, std::string tip, float pret_Producator) const{
    furnizor.SomeOperation(nume_Producator,tip,pret_Producator);
}


int main(){
    Chitare<Calitate::Joasa>ob1("Harley Benton","electric",6,1030);
    Tobe<Calitate::Medie>ob2("Yamaha","acustic",8,3500);
    Orgi<Calitate::Inalta>ob3("Schubert","electric",200,4000);

    try{
        ob1.setTipElectrAcus("electr");
        ob1.set_Pret(-10);
    }
    catch (const Tip_invalid &ob){
        std::cout<<ob.what();
    }
    catch (const Numar_invalid &ob){
        std::cout<<ob.what();
    }

    std::cout<<ob1<<"\n"<<ob2<<"\n"<<ob3<<"\n\n\n";



    auto p=Magazin_unic::getInstance();
    p->add_instrument(std::make_shared<Chitare<Calitate::Medie>>("Fender","acustic",7,2500));
    auto pt=std::make_shared<Chitare<Calitate::Medie>>("Ibanez","electric",6,3000);
    auto ptr=std::make_shared<Chitare<Calitate::Inalta>>("Ibanez","electric",6,6000);
    p->add_instrument(pt);
    p->add_instrument(ptr);
    std::cout<<*p<<"\n\n";


    auto copie1=pt->Clone();
    auto copie2=pt->Clone();
    std::cout<<"\n"<<*copie1<<"\n"<<*copie2;



    int operatie;
    Client client1;
    std::cout<<"Ce instrument doriti sa comandati?\n1.Chitara\n2.Tobe\n3.Clape\n";std::cin>>operatie;
    switch (operatie) {
        case 1:{
            Furnizor_chitare<Calitate::Inalta> f_chitare;
            client1.comanda_speciala(f_chitare,"Fender","electric",7500);
            break;}
        case 2:{
            Furnizor_tobe<Calitate::Medie> f_tobe;
            client1.comanda_speciala(f_tobe,"Yamaha","electric",4500);
            break;}
        case 3:{
            Furnizor_orgi<Calitate::Joasa> f_orgi;
            client1.comanda_speciala(f_orgi,"Yamaha","electric",3000);
            break;
        }
    }

}
