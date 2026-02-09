#include <sqlite3.h>
#include <string>
#include "Employe.h"
#include "Administrateur.h"
#include <iomanip>
#include <iostream>

using namespace std;

Admin::Admin()
{
   m_actif = true; 
}

std::string Admin::activation()
{
    if(m_actif == true)
        return "OUI";
    else    
        return "NON";
}



    