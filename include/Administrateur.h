#ifndef DEF_ADMINISTRATEUR
#define DEF_ADMINISTRATEUR
#include "Employe.h"
#include <sqlite3.h>
#include <string>

class Admin : public Employe
{
    public:
    Admin();
    int activation();
    void desactivation();

    private:
        bool m_etre_employe;
        bool activer_employe;

};

#endif