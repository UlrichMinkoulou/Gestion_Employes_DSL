#include <iostream>
#include "Data_base.h"
#include "Employe.h"
#include "Connexion.h"
#include "Admin_Data_Base.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>


int main(void) 
{
  clearCLI();
    DataBase base_de_donne("entreprise_.db");
    // Admin_db bd_admin("dataBase_admin.db");

    // base_de_donne.ajouterEmploye();
    base_de_donne.afficherEmploye();
    base_de_donne.testRechercherUnEmploye("Doe");
    base_de_donne.rechercherUnEmploye();
    // base_de_donne.verifierMDPdansBD("EDSL0001", "#PO23o");

    // bd_admin.ajouterAdmin();
    // bd_admin.afficherAdmin();
    // // bd_admin.modifierAdmin("ADSL0001");

    // // bd_admin.afficherAdmin();
    // // base_de_donne.activerdesactiverEmployer();

    // // base_de_donne.envoyer_MSG("ADSL0001", "EDSL0001", "Mise a jour de mes avancements", "Avancement");
    // // base_de_donne.afficher_MSG();


    // pageAcceuil();
        // generer_fiche_paie_image("MINKOULOU NKOE", "Ulrich Martin", "EDSL0001", "Développeur C++", 6500.87, "CDI", "Paris", "2024-01-15");


  // if(estUnEmailValide("doe@dsl.cm")) std::cout << "doe@dsl.cm est un email valide" << std::endl;

}

