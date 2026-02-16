#ifndef DEF_CONNEXION
#define DEF_CONNEXION
#include <string>

void pageAcceuil();
bool viewUser(std::string IdUser);
bool viewAdmin(std::string IdAmin);
bool login();
void connexion();
void deconnexion();
void dessinnerL();
void dessinerLdeux();
void cacherModeDePasse();
void imprimer_fiches_paies();
void modifier_Infos();
void options_Employes(int id_admin);
void messages(std::string id);
void clearCLI();
void presentation();
void produits_services();
void boutique();
void contats();
#endif