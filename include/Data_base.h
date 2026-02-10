#ifndef DEF_DATA_BASE
#define DEF_DATA_BASE

#include "Employe.h"
#include <string>
#include <sqlite3.h>

struct  DataConnxion
    {
        std::string id_;
        bool res;
        int id_user;
        std::string name;
    };

class DataBase
{
    public:
        DataBase(const char* nomFichier);
        ~DataBase();
        void ajouterEmploye();
        void rechercherUnEmploye()const;
        void activerdesactiverEmployer();
        void changerInfoEmploye();
        void afficherEmploye();
        void afficherUser(std::string idUser);
        bool connexionEmploye(std::string id_);
        void imprimer_fiche_paie(std::string id_);
        void rechercherUnEmploye_id(std::string id_);
        DataConnxion getData(){return m_data;}

    private:
        sqlite3 *m_db;
        DataConnxion m_data;

};

void afficherLigneEmploye(sqlite3_stmt *stmt);
void dessinnerLignes();
std::string getPassword();

#endif