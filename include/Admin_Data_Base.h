#ifndef DEF_ADMIN_DATA_BASE
#define DEF_ADMIN_DATA_BASE

#include <string>
#include <sqlite3.h>
#include "Data_base.h"
#include "Employe.h"

class Admin_db : public Employe
{
    public:
    Admin_db(const char* filename);
    ~Admin_db();
    void ajouterAdmin();
    bool connexionAdmin(std::string id_);
    void afficherAdmin();
    void modifierAdmin(std::string id_);
    void afficherUserAdmin(std::string id_);
    std::string selectName(std::string id_);
    std::string getETat();
    DataConnxion getAdminData(){return m_data_con;}
    bool verifieridAdminexist(std::string id_);
    private:
    sqlite3* m_bd;
    bool m_etat_compte;
    DataConnxion m_data_con;

    bool verifierMDPdansBD(std::string id_, std::string mot_de_passe);


};

void afficherLigneAdmin(sqlite3_stmt* stmt);
void dessinnerRow();
std::string generateurID(sqlite3* bd, std::string c, char m);

#endif