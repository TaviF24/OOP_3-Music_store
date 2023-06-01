#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include <exception>

class ValoareNegativa : public std::exception {
public:
    const char *what() const noexcept override {
        return "Nu se permite val negativa";
    }
};

enum class TipCombustibil {
    Benzina,
    Motorina
};

/* PrintStream */
class PrintStream {
public:
    virtual void Print(std::ostream&) const = 0;

    friend std::ostream& operator<<(std::ostream&, const PrintStream&);

    virtual ~PrintStream();
};

std::ostream& operator<<(std::ostream& os, const PrintStream& obj) {
    obj.Print(os);
    return os;
}

PrintStream::~PrintStream() = default;


class Model : public PrintStream {
protected:
    int anul, vitezaMax, greutate;
    std::string numele;
public:
    Model(int anul, int vitezaMax, int greutate, const std::string &numele);

    void Print(std::ostream &ostream) const override;

    int getAnul() const;

    void setAnul(int anul);

    int getVitezaMax() const;

    void setVitezaMax(int vitezaMax);

    int getGreutate() const;

    void setGreutate(int greutate);

    const std::string &getNumele() const;

    void setNumele(const std::string &numele);

    void CresteViteza(int);

    virtual int GetAutonomie() const = 0;

    virtual ~Model();
};

Model::~Model() = default;

Model::Model(int anul, int vitezaMax, int greutate, const std::string &numele) : anul(anul), vitezaMax(vitezaMax),
                                                                                 greutate(greutate), numele(numele) {
    if (anul < 0 || vitezaMax < 0) throw ValoareNegativa();
}

int Model::getAnul() const {
    return anul;
}

void Model::setAnul(int anul) {
    Model::anul = anul;
}

int Model::getVitezaMax() const {
    return vitezaMax;
}

void Model::setVitezaMax(int vitezaMax) {
    Model::vitezaMax = vitezaMax;
}

int Model::getGreutate() const {
    return greutate;
}

void Model::setGreutate(int greutate) {
    Model::greutate = greutate;
}

const std::string &Model::getNumele() const {
    return numele;
}

void Model::setNumele(const std::string &numele) {
    Model::numele = numele;
}

void Model::Print(std::ostream &os) const {
     os <<  numele << " " << anul << ' ';
}

void Model::CresteViteza(int p) {
    vitezaMax += (vitezaMax * p) /100;
}

template<TipCombustibil T>
class ModelFosil : public virtual Model {
protected:
    int capRez;
public:
    ModelFosil(int anul, int vitezaMax, int greutate, const std::string &nume,
               int capRez);

    void Print(std::ostream &ostream) const override;

    int GetAutonomie() const override;
};

template<TipCombustibil T>
ModelFosil<T>::ModelFosil(int anul, int vitezaMax, int greutate, const std::string &numele,
                       int capRez) : Model(anul, vitezaMax, greutate, numele),
                                                                        capRez(capRez) {}

template<>
void ModelFosil<TipCombustibil::Benzina>::Print(std::ostream &os) const {
    os << "ModelFosil ";
    Model::Print(os);
    os << capRez << "benzina ";
}

template<>
void ModelFosil<TipCombustibil::Motorina>::Print(std::ostream &os) const {
    os << "ModelFosil ";
    Model::Print(os);
    os << capRez << "motorina ";
}

template<TipCombustibil T>
int ModelFosil<T>::GetAutonomie() const {
    return capRez / pow(greutate,1.0/2);
}

class ModelElectric : public virtual Model {
protected:
    int capBaterie;
public:
    ModelElectric(int anul, int vitezaMax, int greutate, const std::string &numele, int capBaterie);

    void Print(std::ostream &ostream) const override;

    int GetAutonomie() const override;
};

ModelElectric::ModelElectric(int anul, int vitezaMax, int greutate, const std::string &numele, int capBaterie) : Model(
        anul, vitezaMax, greutate, numele), capBaterie(capBaterie) {
}

void ModelElectric::Print(std::ostream &ostream) const {
    ostream << "ModelElectric ";
    Model::Print(ostream);
    ostream << capBaterie << ' ';
}

int ModelElectric::GetAutonomie() const {
    return capBaterie / pow(greutate, 2);
}

class ModelHibrid: public ModelElectric, public ModelFosil<TipCombustibil::Benzina> {
public:
    ModelHibrid(int anul, int vitezaMax, int greutate, const std::string &numele, int capBaterie,
                int capRez);

    void Print(std::ostream &ostream) const override;

    int GetAutonomie() const override;
};

ModelHibrid::ModelHibrid(int anul, int vitezaMax, int greutate, const std::string &numele, int capBaterie,
            int capRez)
            : ModelElectric(anul, vitezaMax, greutate, numele, capBaterie),
                ModelFosil<TipCombustibil::Benzina>(anul, vitezaMax, greutate, numele, capRez),
                Model(anul, vitezaMax, greutate, numele) {}

void ModelHibrid::Print(std::ostream &ostream) const {
    ostream << "ModelHibrid ";
    ModelFosil::Print(ostream);
    //
}

int ModelHibrid::GetAutonomie() const {
    return ModelElectric::GetAutonomie();
}

class Tranzactie : public PrintStream {
protected:
    std::string nume, data;
    std::vector<std::shared_ptr<Model>> modele;
public:
    Tranzactie(const std::string &nume, const std::string &data, const std::vector<std::shared_ptr<Model>> &modele);

    void Print(std::ostream &ostream) const override;
};

Tranzactie::Tranzactie(const std::string &nume, const std::string &data,
                       const std::vector<std::shared_ptr<Model>> &modele) : nume(nume), data(data), modele(modele) {}

void Tranzactie::Print(std::ostream &ostream) const {
    ostream << nume << " ";
    for (auto m : modele)
        ostream << *m << ' ';
 }

 class Producator {
 protected:
     std::vector<std::shared_ptr<Model>> modele;
    std::vector<std::shared_ptr<Tranzactie>> tranzactii;

    static Producator* instance;
    Producator() = default;

 public:
    static Producator* GetInstance() {
        if (instance == nullptr)
            instance = new Producator();
        return instance;
    }

    ~Producator(){
        std::cout<<"destructor"<<"\n";
//        delete instance;
    }
 };

Producator* Producator::instance = nullptr;

int main() {
    try {
        auto m = ModelFosil<TipCombustibil::Benzina>(2019, 11, 2, "ss", 100);
        std::cout << m;
    } catch (const ValoareNegativa& e) {
        std::cout << e.what();
    }
    std::cout << "Hello, World!" << std::endl;

    Producator::GetInstance();

    return 0;
}

//#include <iostream>
//
//class Singleton
//{
//public:
//    ~Singleton()  { std::cout << "destruction!\n"; }
//
//    static Singleton& getInstance()
//    {
//        static Singleton instance;
//        return instance;
//    }
//
//    void foo() { std::cout << "foo!\n"; }
//
//private:
//    Singleton() { std::cout << "construction!\n"; }
//};
//
//int main()
//{
//    Singleton::getInstance().foo();
//}

// Output:
//   construction!
//   foo!
//   destruction!