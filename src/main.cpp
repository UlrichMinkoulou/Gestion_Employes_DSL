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
  DataBase db("entreprise_.db");
  db.testerConnexionMySQL();

  // Admin_db ad("entreprise_.db");
  // ad.ajouterAdmin();
  // ad.chargerCacheAdmin();
  // ad.verifieridAdminexist_caching("ADSL0001");

  pageAcceuil();

}

