#ifndef DEF_ADMIN_DATA_BASE
#define DEF_ADMIN_DATA_BASE

#include <string>
#include <sqlite3.h>
#include "Data_base.h"
#include "Employe.h"

struct Data_Admin
{
    std::string idAdmin;
    std::string nomAdmin;
    std::string prenomAdmin;
    std::string mdpAdmin;
    int etatadmin;

};

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

    DataConnxion getAdminData()
    {
        return m_data_con;
    }

    bool verifieridAdminexist(std::string id_);

    
    bool verifierMDPdansBD(std::string id_, std::string mot_de_passe);
    
    //Caching
    bool verifierMDPdansBD_caching(std::string id, std::string mdp); //ok
    void chargerCacheAdmin(); //ok
    void afficherUserAdmin_caching(); //ok
    std::string selectName_caching(std::string id); //ok
    void afficherAdmin_caching(); //ok
 

    std::vector<Data_Admin> getCache()
    {
        return m_liste_Admin;
    }
    
    private:
    sqlite3* m_bd;
    bool m_etat_compte;
    DataConnxion m_data_con;


    //Vector pour le caching
    std::vector<Data_Admin> m_liste_Admin;
};

void afficherLigneAdmin(sqlite3_stmt* stmt);
void dessinnerRow();
std::string generateurID(sqlite3* bd, std::string c, char m);

#endif