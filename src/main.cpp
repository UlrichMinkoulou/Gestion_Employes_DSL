#include <iostream>
#include "Data_base.h"
#include "Employe.h"
#include "Connexion.h"
#include "Admin_Data_Base.h"
#include "MySQL_DataBase.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>


int main(void) 
{
  clearCLI();
  DataBase db("entreprise_.db");
  // db.testerConnexionMySQL();
  MySQL_DatBase mysql_db("localhost", "root", "", "Gestion_emp_bd");
  Employe emp;
  // mysql_db.ajouterEmploye(emp);
  mysql_db.chargerCacheEmployes();
  db.afficherEmploye_caching(mysql_db.getCacheEmployes());
  Employe resultTrouve = mysql_db.rechercherEmployeParID_caching("EDSL0001");
  mysql_db.afficherunEmployeSQL(resultTrouve);
  
  mysql_db.rechercherEmployeparNom("o");
  mysql_db.activerdesactiverEmployerSQL("EDSL0002");
  db.afficherEmploye_caching(mysql_db.getCacheEmployes());

  mysql_db.modifierEmployeSQL("EDSL0001");
  

  // Admin_db ad("entreprise_.db");
  // ad.ajouterAdmin();
  // ad.chargerCacheAdmin();
  // ad.verifieridAdminexist_caching("ADSL0001");

  // pageAcceuil();

}

