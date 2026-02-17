#include <iostream>
#include "Data_base.h"
#include "Employe.h"
#include "Connexion.h"
#include "Admin_Data_Base.h"



int main(void) 
{

    DataBase base_de_donne("entreprise_.db");
    Admin_db bd_admin("dataBase_admin.db");

    // base_de_donne.ajouterEmploye();
    base_de_donne.afficherEmploye();
    // bd_admin.afficherAdmin();
    // base_de_donne.activerdesactiverEmployer();

    // base_de_donne.envoyer_MSG("ADSL0001", "EDSL0001", "Mise a jour de mes avancements", "Avancement");
    // base_de_donne.afficher_MSG();


    pageAcceuil();


}

