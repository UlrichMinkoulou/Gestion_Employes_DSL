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
    std::string mdpAdmin;
    int etatadmin;

};

struct Dt_Admin
{
    public: 
    Dt_Admin(std::string nom, std::string mdp, int etat):  m_nomAdmin(nom), m_mdpAdmin(mdp), m_etatadmin(etat){}
    
    //setteurs
    // std::string setId(std::string id_){m_idAdmin = id_; return m_idAdmin;}
    std::string setNom(std::string nom){m_nomAdmin = nom; return m_nomAdmin;}
    std::string setmdp(std::string mdp){m_mdpAdmin = mdp; return m_mdpAdmin;}
            int setEtat(int etat){m_etatadmin =  etat; return m_etatadmin;}

    //getteurs
    std::string getNom(){return m_nomAdmin;}
    std::string getMdp(){return m_mdpAdmin = crypterMotDePasse(m_mdpAdmin);}
    int getEtat(){return m_etatadmin;}


    private:
    // std::string m_idAdmin;
    std::string m_nomAdmin;
    std::string m_mdpAdmin;
    int m_etatadmin;
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
    bool verifieridAdminexist_caching(std::string id_); //ok
    void afficherAdmin_caching(); //ok

    //Test
    void ajouterAdminTest(Dt_Admin& ad);
    bool connexionAdminTest(std::string id_, std::string mdp);
    void modifierAdminTest(std::string id_, std::string nom, std::string mdp, int etat);
    Data_Admin testRechercherUnAdmin(std::string id_);
 

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