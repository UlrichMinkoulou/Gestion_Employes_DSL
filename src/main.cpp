#include <iostream>
#include "Data_base.h"
#include "Employe.h"
#include "Connexion.h"
#include "Admin_Data_Base.h"



int main(void) 
{

    DataBase base_de_donne("entreprise.db");
    Admin_db bd_admin("dataBase_admin.db");

    // bd_admin.ajouterAdmin();
    bd_admin.afficherAdmin();
    
    // base_de_donne.ajouterEmploye();
    base_de_donne.afficherEmploye();
    pageAcceuil();
    // base_de_donne.rechercherUnEmploye();
    // base_de_donne.changerInfoEmploye();
    // base_de_donne.connexionEmploye();

}

