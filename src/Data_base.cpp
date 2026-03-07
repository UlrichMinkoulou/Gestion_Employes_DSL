#include "Data_base.h"
#include "Admin_Data_Base.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <sodium.h>


// --- LIBRAIRIES DESSIN ET ECRITURE ---
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define CANVAS_ITY_IMPLEMENTATION
#include "canvas_ity.hpp"

extern "C" {
    #include "qrcode.h"
}

const std::string ANSI_RESET = "\033[0m";
const std::string ANSI_RED = "\033[31m";
const std::string ANSI_BOLD = "\033[1m";
const std::string ANSI_GREEN = "\033[32m";
const std::string ANSI_BLUE = "\033[34m";
const std::string ANSI_YELLOW = "\033[33m";
const std::string ANSI_MAGENTA = "\033[35m";
const std::string ANSI_CYAN = "\033[36m";
const std::string ANSI_WHITE = "\033[37m";
const std::string ANSI_BLACK = "\033[30m";

using namespace std;


DataBase::DataBase(char const* nomFichier)
{
    if(sqlite3_open(nomFichier, &m_db) != SQLITE_OK)
        std::cerr << "Erreur d'ouverture de la BD : " << sqlite3_errmsg(m_db) << endl;
    else 
        std::cout << "  " << endl; //Base de donnees ouverte avec succes !
    
    
    char * msg_err;
    string sqlCreate =  "CREATE TABLE IF NOT EXISTS EMPLOYE ("
                        "ID TEXT NOT NULL PRIMARY KEY,"
                        "NOM TEXT NOT NULL,"
                        "PRENOM TEXT NOT NULL,"
                        "DATE_NAIS TEXT NOT NULL,"
                        "DATE_ADHE TEXT NOT NULL,"
                        "SITUATION_MAT TEXT NOT NULL,"
                        "POSTE TEXT NOT NULL,"
                        "TYPECONTRAT TEXT NOT NULL,"
                        "SALAIRE DOUBLE,"
                        "CATEGORIE TEXT NOT NULL,"
                        "EMAIL TEXT NOT NULL,"
                        "MDP TEXT NOT NULL,"
                        "ETAT INTEGER);";

    sqlite3_exec(m_db, sqlCreate.c_str(), NULL, 0, &msg_err);

    //creation de la table pour les message dans la base de donnees
    string sqlCreate_message = "CREATE TABLE IF NOT EXISTS MESSAGE ("
                               "ID_MSG INTEGER PRIMARY KEY AUTOINCREMENT,"
                               "ID_DESTINATAIRE TEXT NOT NULL,"
                               "ID_EXPEDITEUR TEXT NOT NULL,"
                               "CONTENU_MESSAGE TEXT NOT NULL,"
                               "OBJET TEXT NOT NULL,"
                               "DATE_TIME DATETIME DEFAULT CURRENT_TIMESTAMP,"
                               "LU INTEGER DEFAULT 0);";

    sqlite3_exec(m_db, sqlCreate_message.c_str(), NULL, 0, &msg_err);

}

DataBase::~DataBase()
{
    sqlite3_close(m_db);
}


void DataBase::ajouterEmploye()
    {
        std::cout << std::endl << std::endl << "-------------------- Ajout employe" << std::endl << std::endl;
        string id_user = generateurID(m_db, "EDSL", 'e');
        
        Employe e;
            
        string sqlInsert = "INSERT INTO EMPLOYE (ID, NOM, PRENOM, DATE_NAIS, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, EMAIL, MDP, ETAT) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
        sqlite3_stmt *stmt;

        if(sqlite3_prepare_v2(m_db, sqlInsert.c_str(), -1, &stmt, NULL) != SQLITE_OK)
        {
            cerr << ANSI_BOLD << ANSI_RED << "\n[ERREUR]" << ANSI_RESET << "Erreur de preparation : " << sqlite3_errmsg(m_db) << endl; 
            return;
        }


        sqlite3_bind_text(stmt, 1, id_user.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, e.getNom().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, e.getPrenom().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, e.setDateNaissance().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, e.getDate_adhesion().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, e.getSituation_matrimonial().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, e.getPoste().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 8, e.getType_contrat().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 9, e.getSalaire());
        sqlite3_bind_text(stmt, 10, e.getCategorie().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 11, e.getEmail().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 12, e.getMot_de_passe().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 13, e.activer());
            
        if(sqlite3_step(stmt) == SQLITE_DONE)
            cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Employe ajoute avec succes !! " <<endl;
        else
            cerr << ANSI_BOLD << ANSI_RED << "\n[ERREUR]" << ANSI_RESET  << "Erreur  lors de l'insertion : " << sqlite3_errmsg(m_db) << endl;
            
        sqlite3_finalize(stmt);
    
            
    }


//Afficher la liste des Employes
    void DataBase:: afficherEmploye()
    {
        cout << "\n --- LISTES DES EMPLOYES --- " << endl;
        dessinnerLignes();

        //En-tete
    cout << "| " << left << setw(8) << "Id"
         << "| " << setw(10) << "NOM"
         << "| " << setw(10) << "PRENOM"
         << "| " << setw(10) << "DATE_NAIS"        
         << "| " << setw(10) << "DATE_ADHE"
         << "| " << setw(14) << "SITUATION_MAT"
         << "| " << setw(18) << "POSTE"        
         << "| " << setw(3) << "TC"
         << "| " << setw(12) << "SALAIRE"        
         << "| " << setw(1) << "C"
        //  << "| " << setw(14) << "MDP"
         << "| " << setw(25) << "EMAIL"
         << "| " << setw(1) << "E" << "|" << endl;

    dessinnerLignes();


    string sql = "SELECT ID, NOM, PRENOM, DATE_NAIS, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, MDP, EMAIL, ETAT FROM EMPLOYE;";
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL);
    while(sqlite3_step(stmt) == SQLITE_ROW) afficherLigneEmploye(stmt);
    dessinnerLignes();
    
    sqlite3_finalize(stmt);

    }

    bool desactiverEmployer(sqlite3* m_db, sqlite3_stmt *stmt)
    {

        return true;
    }

    void DataBase::afficherUser(std::string identifiant)
    {

    dessinnerLignes();
   cout << "| " << left << setw(8) << "Id"
         << "| " << setw(10) << "NOM"
         << "| " << setw(10) << "PRENOM"
         << "| " << setw(10) << "DATE_NAIS"        
         << "| " << setw(10) << "DATE_ADHE"
         << "| " << setw(14) << "SITUATION_MAT"
         << "| " << setw(18) << "POSTE"        
         << "| " << setw(3) << "TC"
         << "| " << setw(12) << "SALAIRE"        
         << "| " << setw(1) << "C"
        //  << "| " << setw(14) << "MDP"
         << "| " << setw(25) << "EMAIL"
         << "| " << setw(1) << "E" << "|" << endl;

    dessinnerLignes();


    string sql = "SELECT ID, NOM, PRENOM, DATE_NAIS, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, MDP, EMAIL, ETAT FROM EMPLOYE WHERE ID =?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, identifiant.c_str(), -1, SQLITE_TRANSIENT);
        while(sqlite3_step(stmt) == SQLITE_ROW) afficherLigneEmploye(stmt);
        dessinnerLignes();
        sqlite3_finalize(stmt);
    }
    

    }


//Recherche d'un employe
    void DataBase::rechercherUnEmploye ()const
    {
        string recherche;
        cout << "Entrez le nom (ou une partie du nom) a rechercher : "; cin >> recherche;
        recherche = "%" + recherche + "%"; // format pour le LIkE SQL
        sqlite3_stmt *stmt;
        string sql = "SELECT ID, NOM, PRENOM, DATE_NAIS, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, MDP, EMAIL, ETAT FROM EMPLOYE WHERE NOM LIKE ?;";
        if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, recherche.c_str(), -1, SQLITE_TRANSIENT);

            cout << "\n --- RESULTAT DE LA RECHERCHE ---" <<endl;
            dessinnerLignes();
            bool trouve = false;

            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                afficherLigneEmploye(stmt);
                trouve = true;
            }

            if(!trouve) cout << ANSI_BOLD << ANSI_RED <<"\n[ERREUR]" "| Aucun Employe trouve pour ce nom. " << ANSI_RESET << setw(15) << " " << "|" << endl;
            dessinnerLignes();
        }
        sqlite3_finalize(stmt);
    }

    void DataBase::rechercherUnEmploye_id (string identifiant)
    {


        // recherche = "%" + recherche + "%"; // format pour le LIkE SQL
        sqlite3_stmt *stmt;
        string sql = "SELECT ID, NOM, PRENOM, DATE_NAIS, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, MDP, EMAIL, ETAT FROM EMPLOYE WHERE ID = ?;";
        if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, identifiant.c_str(), -1, SQLITE_TRANSIENT);

            cout << "\n --- EMPLOYE ---" <<endl;
            dessinnerLignes();
            bool trouve = false;

            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                afficherLigneEmploye(stmt);
                trouve = true;
            }

            if(!trouve) cout << ANSI_BOLD << ANSI_RED <<"\n[ERREUR]" << ANSI_RESET << "| Aucun Employe trouve pour cet identifiant. " << setw(15) << " " << "|" << endl;
            dessinnerLignes();
        }
        sqlite3_finalize(stmt);
    }
    
    bool DataBase::verif_if(string identifiant)
    {

        bool trouve = false;

        // recherche = "%" + recherche + "%"; // format pour le LIkE SQL
        sqlite3_stmt *stmt;
        string sql = "SELECT ID, NOM, PRENOM, DATE_NAIS, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, MDP, EMAIL, ETAT FROM EMPLOYE WHERE ID = ?;";
        if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, identifiant.c_str(), -1, SQLITE_TRANSIENT);

            cout << "\n --- EMPLOYE ---" <<endl;
            dessinnerLignes();
            //trouve = false;

            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                afficherLigneEmploye(stmt);
                trouve = true;
            }

            if(!trouve) cout << ANSI_BOLD << ANSI_RED <<"[ERREUR]" << ANSI_RESET << "| Aucun Employe trouve pour cet identifiant. " << setw(15) << " " << "|" << endl;
            dessinnerLignes();
        }
        sqlite3_finalize(stmt);
        return trouve;
    }

//Modification des infos Employe cote user

    void DataBase::mofifierInfoConnexionEmploye(std::string id_)
    {
        std::cout << "----Mofification des infos de connexion----" << std::endl << std::endl;
        std::string new_password, confirmPassword;

        bool verifier = true;
        do
        {
            std::cout << "Entrez le nouveau mot de passe : ";
            std::cin >> new_password;
            std::cout << "Confirmez le nouveau mot de passe : ";
             std::cin >> confirmPassword;

            if (new_password != confirmPassword)
            {
                std::cout << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET  << "Les mots de passe ne correspondent pas. Veuillez reessayer."  << std::endl;
                verifier = false;
            }
            else
                {
                    verifier = true;
                }

        }while (verifier == false);



        string sql = "UPDATE EMPLOYE SET MDP=? WHERE ID=?;";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL);
        sqlite3_bind_text(stmt, 1, new_password.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, id_.c_str(), -1, SQLITE_TRANSIENT);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
            {
                if(sqlite3_changes(m_db) > 0)
                    cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mise a jour du mot de passe reussie !" << endl;
                else    
                    cout << ANSI_BOLD << ANSI_RED << "\n[ERREUR]" << ANSI_RESET << "Aucun Employe trouve avec cet Id !" << endl;
            }
        else
            {
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur lors de la mise a jour du mot de passe : " << sqlite3_errmsg(m_db) << endl;
            }

        sqlite3_finalize(stmt);
    }


//Modification des infos Employe cote admin
    void DataBase::changerInfoEmploye()
    {
    std::cout << std::endl << std::endl << "-------------------- Mise a jour employe" << std::endl;

    Employe e;
    cout << "----Entrez l'identifiant : ";
     cin >> m_data.id_;

    DataBase BD("entreprise_.db");
    BD.rechercherUnEmploye_id(m_data.id_);

    int choix;

    std::cout << "---- Quelles Informations souhaitez-vous changer?" << std::endl;
    std::cout << "1. Tout" << std::endl;
    std::cout << "2. Nom " << std::endl;
    std::cout << "3. Prenom " << std::endl;
    std::cout << "4. Date de naissance " << std::endl;
    std::cout << "5. Date d'adhesion " << std::endl;
    std::cout << "6. Situation Matrimonial " << std::endl;
    std::cout << "7. Poste " << std::endl;
    std::cout << "8. Type de contrat " << std::endl;
    std::cout << "9. Salaire " << std::endl;
    std::cout << "10. Mot de passe " << std::endl;
    std::cout << "11. Email " << std::endl;
    std::cout << "> ";

            //gestion des erreurs de saisie pour le choix de l'information a modifier
                while(!(std::cin >> choix) || (choix < 1 || choix > 11))
                            {
                                std::cout << "> choix (1-11): ";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }


    switch (choix)
    {
        case 1: 
                {
                    string sql = "UPDATE EMPLOYE SET NOM=?, PRENOM=?, DATE_NAIS=?, DATE_ADHE=?, SITUATION_MAT=?, POSTE=?, TYPECONTRAT=?, SALAIRE=?, CATEGORIE=?, MDP=?, EMAIL=?, ETAT=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 13, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 1, e.getNom().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, e.getPrenom().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 3, e.setDateNaissance().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 4, e.getDate_adhesion().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 5, e.getSituation_matrimonial().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 6, e.getPoste().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 7, e.getType_contrat().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_double(stmt, 8, e.getSalaire());
                    sqlite3_bind_text(stmt, 9, e.getCategorie().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 10, e.getMot_de_passe().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 11, e.getEmail().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_int(stmt, 12, e.activer());

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mise a jour reussie !" << endl;
                            else    
                                cout << ANSI_BOLD << ANSI_RED << "\n[ERREUR]" << ANSI_RESET  << "Aucun Employe trouve avec cet Id !" << ANSI_RESET << endl;
                        }
                    else
                        {
                            cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << ANSI_RESET  << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;}

        case 2: {
                    string sql = "UPDATE EMPLOYE SET NOM=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 1, e.getNom().c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mise a jour reussie !" << endl;
                            else    
                                cout << ANSI_BOLD << ANSI_RED << "\n[ERREUR]" << ANSI_RESET  << "Aucun Employe trouve avec cet Id !" << ANSI_RESET << endl;
                        }
                    else
                        {
                            cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << ANSI_RESET  << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }

        case 3: 
                {
                    string sql = "UPDATE EMPLOYE SET PRENOM=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 1, e.getPrenom().c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mise a jour reussie !" << endl;
                            else    
                                cout << ANSI_BOLD << ANSI_RED<< "\n[ERREUR]" << ANSI_RESET  << "Aucun Employe trouve avec cet Id !" << ANSI_RESET << endl;
                        }
                    else
                        {
                            cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }

        case 4: 
                {
                    string sql = "UPDATE EMPLOYE SET DATE_NAIS=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 1, e.setDateNaissance().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mise a jour reussie !" << endl;
                            else    
                                cout << ANSI_BOLD << ANSI_RED<< "\n[ERREUR]" << ANSI_RESET  << "Aucun Employe trouve avec cet Id !" << ANSI_RESET << endl;
                        }
                    else
                        {
                            cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }

        case 5: 
                {
                    string sql = "UPDATE EMPLOYE SET DATE_ADHE=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 1, e.getDate_adhesion().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mise a jour reussie !" << endl;
                            else    
                                cout << ANSI_BOLD << ANSI_RED<< "\n[ERREUR]" << ANSI_RESET  << "Aucun Employe trouve avec cet Id !" << ANSI_RESET << endl;
                        }
                    else
                        {
                            cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }

        case 6:
                {
                    string sql = "UPDATE EMPLOYE SET SITUATION_MAT=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 1, e.getSituation_matrimonial().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mise a jour reussie !" << endl;
                            else    
                                cout << ANSI_BOLD << ANSI_RED<< "\n[ERREUR]" << ANSI_RESET  << "Aucun Employe trouve avec cet Id !" << ANSI_RESET << endl;
                        }
                    else
                        {
                            cerr << ANSI_BOLD << ANSI_RED << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << ANSI_RESET  << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }

        case 7:
                {
                    string sql = "UPDATE EMPLOYE SET POSTE=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 1, e.getPoste().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mise a jour reussie !" << endl;
                            else    
                                cout << ANSI_BOLD << ANSI_RED<< "\n[ERREUR]" << ANSI_RESET  << "Aucun Employe trouve avec cet Id !" << ANSI_RESET << endl;
                        }
                    else
                        {
                            cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }

        case 8: 
                {
                    string sql = "UPDATE EMPLOYE SET TYPECONTRAT=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 1, e.getType_contrat().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mise a jour reussie !" << endl;
                            else    
                                cout << ANSI_BOLD << ANSI_RED<< "\n[ERREUR]" << ANSI_RESET  << "Aucun Employe trouve avec cet Id !" << ANSI_RESET << endl;
                        }
                    else
                        {
                            cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }

        case 9: 
                {
                    string sql = "UPDATE EMPLOYE SET SALAIRE=?, CATEGORIE=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_double(stmt, 1, e.getSalaire());
                    sqlite3_bind_text(stmt, 2, e.getCategorie().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 3, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mise a jour reussie !" << endl;
                            else    
                                cout << ANSI_BOLD << ANSI_RED<< "\n[ERREUR]" << ANSI_RESET  << "Aucun Employe trouve avec cet Id !" << ANSI_RESET << endl;
                        }
                    else
                        {
                            cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }

        case 10:
                {
                    string sql = "UPDATE EMPLOYE SET MDP=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 1, e.getMot_de_passe().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mise a jour reussie !" << endl;
                            else    
                                cout << ANSI_BOLD << ANSI_RED<< "\n[ERREUR]" << ANSI_RESET  << "Aucun Employe trouve avec cet Id !" << ANSI_RESET << endl;
                        }
                    else
                        {
                            cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }
                
        case 11: 
                {
                    string sql = "UPDATE EMPLOYE SET EMAIL=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 1, e.getEmail().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mise a jour reussie !" << endl;
                            else    
                                cout << ANSI_BOLD << ANSI_RED<< "\n[ERREUR]" << ANSI_RESET  << "Aucun Employe trouve avec cet Id !" << ANSI_RESET << endl;
                        }
                    else
                        {
                            cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }
    
    default:
        break;
    }



    }

//Fonction pour lire un caractere sans echo dans le terminal
    std::string getPassword()
    {
        std::string password;

        struct termios oldt, newt;
        

        tcgetattr(STDIN_FILENO, &oldt); //Sauvegarde la configuration actuelle
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); //Desactive le mode canonique et l'echo
        tcsetattr(STDIN_FILENO,TCSANOW, &newt);
        
        char ch;
        while(true)
        {
            ch = getchar();
            if(ch == '\n' || ch == '\r')
            {
                break;
            }else if (ch == 127 || ch == 8){
                if(!password.empty()){
                    password.pop_back();

                    std::cout <<"\b \b" << std::flush;
                }
            }else{
                password += ch;
                std::cout << '*' << std::flush;
            }

        }

        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); //Restaure la configuration

        return password;
    }

    void DataBase::verifierMDPdansBD(std::string id_, std::string mot_de_passe)
    {
        string mdp, mdp_crypte, mdp_bd;
        // std::cout << "> Mot de passe : " << std::endl;
        //  std::cin >> mdp;
        mdp_crypte = crypterMotDePasse(mot_de_passe);

        string sql = "SELECT MDP FROM EMPLOYE WHERE ID = ?;";
        sqlite3_stmt *stmt;

        if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
        {
            std::cerr << ANSI_BOLD << ANSI_GREEN << "\n[INFO]" << ANSI_RESET << "Erreur SQL :" << sqlite3_errmsg(m_db) << std::endl;
            return;
        }
            sqlite3_bind_text(stmt, 1, id_.c_str(), -1, SQLITE_TRANSIENT);
        
            if(sqlite3_step(stmt) == SQLITE_ROW)
            {
                mdp_bd = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)); //Récupère le mot de passe crypté de la BD
                // std::cout << "Mot de passe crypté saisi : " << mdp_crypte << std::endl;
                std::cout << "Mot de passe crypté dans la BD : " << mdp_bd << std::endl;

                if(crypto_pwhash_str_verify(mdp_bd.c_str(), mot_de_passe.c_str(), mot_de_passe.size()) == 0)
                {
                    std::cout << ANSI_BOLD << ANSI_GREEN << "\n[SUCCES]" << ANSI_RESET << "Mot de passe correct !" << std::endl;
                }
                else
                {
                    std::cout << ANSI_BOLD << ANSI_RED << "\n[ERREUR]" << ANSI_RESET << "Mot de passe incorrect !" << std::endl;
                }
            }
            else
            {
                std::cout << ANSI_BOLD << ANSI_RED << "\n[ERREUR]" << ANSI_RESET << "Aucun utilisateur trouvé avec cet ID !" << std::endl;
            }

            sqlite3_finalize(stmt);

    }

    bool DataBase::connexionEmploye(std::string id_)
    {

            std::time_t start = std::time(nullptr);
            std::string mot_de_passe; char ch;
            
            m_data.id_ = id_;

            std::cout << "> Mot de passe : " << std::flush;
            mot_de_passe = getPassword();
            string mdp_crypte = crypterMotDePasse(mot_de_passe);
            std::cout << "votre mot de passe : " << mdp_crypte << std::endl;


            std::string sql_recherche = "SELECT ID, NOM, PRENOM, DATE_NAIS, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, MDP, EMAIL, ETAT FROM EMPLOYE WHERE ID = ? AND MDP = ?;";
            sqlite3_stmt *stmt;

            if(sqlite3_prepare_v2(m_db, sql_recherche.c_str(), -1, &stmt, NULL) != SQLITE_OK)
            {
                std::cerr << ANSI_BOLD << ANSI_GREEN << "\n[INFO]" << ANSI_RESET << "Erreur SQL :" << sqlite3_errmsg(m_db) << std::endl;
                return false;
            }
                sqlite3_bind_text(stmt, 1, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 2, mdp_crypte.c_str(), -1, SQLITE_TRANSIENT);
                m_data.res = true;

 
                
                if(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    int etat = (sqlite3_column_int(stmt, 12)); //Récupère la valeur de la colonne ETAT

                        std::cout << "Valeur de etat : " << etat << std::endl;

                        if(etat ==0) {
                                m_data.res = false;
                                std::cout << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << "Compte inactif, veillez contacter l'administrateur !\n\n"  << std::endl;
                                sqlite3_finalize(stmt);
                                
                                return m_data.res;               
                            }
                                const unsigned char *user_name = sqlite3_column_text(stmt, 1);
                                std::cout << ANSI_BOLD << ANSI_GREEN << "Bienvenu " << (user_name ? reinterpret_cast<const char*>(user_name) : "NULL") << ANSI_RESET << std::endl;
                                m_data.name =  reinterpret_cast<const char*>(user_name); //faire le caste
                    
                                std::cout << "Chargement de la page";
                                for(int i = 0; i < 8; ++i)
                                {
                                    std::this_thread::sleep_for(std::chrono::seconds(1));
                                    std::cout << "." << std::flush;
                                } 
                                
                                system("clear");               
                        } else
                            {
                                m_data.res = false;
                                std::cout << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << "Id ou mot de passe Incorrect !"  << std::endl;
                            }

                    
                    sqlite3_finalize(stmt);

                    return m_data.res;
    }


// #include "qrcode.h" // Assure-toi d'avoir inclus la lib
void dessinerQRCode(canvas_ity::canvas& cv, std::string texte, float x, float y, float tailleCarre) {
    // 1. Créer le QR Code (Version 3 = environ 29x29 modules, assez pour un ID ou URL)
    QRCode qrcode;


   const uint8_t version = 10; // Version 5 pour être large avec les noms longs
    
    // Le buffer doit avoir une taille précise calculée par la bibliothèque
    uint8_t modules[qrcode_getBufferSize(version)]; 

    // Appel de la fonction avec ta signature :
    // qrcode (pointeur), modules (le buffer), version, ecc, data (const char*)
    int8_t result = qrcode_initText(&qrcode, modules, version, ECC_MEDIUM, texte.c_str());

        if (result != 0) {
            std::cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << " : Texte trop long pour la version du QR Code !" << std::endl;
            return;
        }

        // Calcul de la taille d'un "module" (un petit carré noir)
        float moduleSize = tailleCarre / qrcode.size;

        for (uint8_t y_qr = 0; y_qr < qrcode.size; y_qr++) {
            for (uint8_t x_qr = 0; x_qr < qrcode.size; x_qr++) {
                if (qrcode_getModule(&qrcode, x_qr, y_qr)) {
                    // On utilise moduleSize pile, sans ajout, pour garder les séparations nettes
                    cv.fill_rectangle(x + (x_qr * moduleSize), 
                                    y + (y_qr * moduleSize), 
                                    moduleSize, 
                                    moduleSize);
                    }
                }
            }
        }


std::vector<unsigned char> chargerPolice(std::string chemin) {
    std::ifstream file(chemin, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << ": Impossible d'ouvrir le fichier .ttf !" << std::endl;
        return {}; // Renvoie un vector vide
    }
    std::streamsize size = file.tellg();
    if (size <= 0) return {};

    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> buffer(size);
    file.read((char*)buffer.data(), size);
    return buffer;
}

void dessinerLogo(canvas_ity::canvas& cv, std::string chemin, float x, float y, float cibleL, float cibleH) {
    int imgL, imgH, canaux;
    
    // 1. Charger l'image en forçant 4 canaux (R, G, B, Alpha)
    unsigned char* données = stbi_load(chemin.c_str(), &imgL, &imgH, &canaux, 4);

    if (données) {
        // 2. Calcul du STRIDE : Largeur de l'image source * 4 octets (RGBA)
        int stride = imgL * 4;

        // 3. Appel de la fonction de canvas_ity avec ta signature :
        // image, width, height, stride, x, y, to_width, to_height
        cv.draw_image(données, imgL, imgH, stride, x, y, cibleL, cibleH);

        // 4. Libérer la mémoire STB
        stbi_image_free(données);
    } else {
        std::cerr << ANSI_BOLD << ANSI_RED<< "[ERREUR]" << ANSI_RESET << ": Impossible de charger le logo " << chemin << std::endl;
    }

}  

    void DataBase::imprimer_fiche_paie(std::string id_){ 
    DataBase user("entreprise_.db");


           sqlite3_stmt* stmt;
           string sql = "SELECT ID, NOM, PRENOM, DATE_NAIS, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, MDP, EMAIL, ETAT FROM EMPLOYE WHERE ID = ?;";

            if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
            {
                sqlite3_bind_text(stmt, 1, id_.c_str(), -1, SQLITE_TRANSIENT);
                while(sqlite3_step(stmt) == SQLITE_ROW)
                {

                    
                    std::string nom = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                    std::string prenom = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                    std::string id_emp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                    std::string poste = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
                    double salaire_brut = sqlite3_column_double(stmt, 8);
                    std::string type_contrat = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
                    std::string date_embauche = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                    std::string sit_mat = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                    int etat = sqlite3_column_int(stmt, 12);
                    std::string dateNaissance = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                    std::string dateTime = __DATE__; //Date de generation de la fiche de paie


                        std::string ville = "Paris";
                        int largeur = 800;
                        int hauteur = 1132;
                        std::string annee_ = "2026";
                        std::string mois_ = "04";
                        std::string position_gestion = "ACTIVITE";
                        std::string position_solde = "ACTIVITE NORMALEMENT";
                        std::string banc = "SG FRANCE";
                        int nocpt = 2004320986;  //On doit modifier la valeur de facon externe pour chaque employe
                        int nmbEnfant = 0;
                        canvas_ity::canvas cv(largeur, hauteur);

                        if(etat ==0){
                            position_gestion = "INACTIF";
                            position_solde = "INACTIF";
                            salaire_brut = 0.0;
                        }


                    std::vector<unsigned char> font_data = chargerPolice("/home/ulrich/Downloads/Roboto-Regular.ttf");
                        cv.set_font(font_data.data(), (int)font_data.size(), 30.0f); // Charger une police pour le titre

                        if (largeur <= 0 || hauteur <= 0 || largeur > 5000 || hauteur > 5000) {
                        std::cerr << "Dimensions invalides !" << std::endl;
                        return;
                    }
                        // --- FOND & CADRE ---
                        // std::string nom; double brut = 6789.45; // Valeurs d'exemple, à remplacer par des données réelles
                        cv.set_color(canvas_ity::fill_style, 255, 255, 255, 255); // Blanc
                        cv.fill_rectangle(0, 0, largeur, hauteur);
                        
                        cv.set_color(canvas_ity::stroke_style, 44, 62, 80, 255); // Bleu foncé
                        cv.set_line_width(5);
                        cv.stroke_rectangle(10, 10, 780, 580);

                        // --- ENTÊTE ---

                        // On charge l'image
                        dessinerLogo(cv, "dsl.jpeg", 50, 20, 250, 250); // Chemin du logo et position
                        // cv.set_font(font_data.data(), (int)font_data.size(), 18.0f); // Note: il faut charger une police .ttf pour un rendu pro
                        // cv.set_color(canvas_ity::fill_style, 44, 62, 80, 255);
                        // cv.fill_text("BULLETIN DE PAIE", 250, 60);  

                        cv.set_line_width(2);
                        cv.move_to(50, 80);
                        cv.line_to(750, 80);
                        cv.stroke();

                        // --- INFOS EMPLOYÉ ---
                        std::string employe = "Matricule : " + id_;
                        std::string annee = "Annee : " + annee_;
                        std::string mois = "Mois : " + mois_;
                        cv.set_font(font_data.data(), (int)font_data.size(), 18.0f); // Gras pour les infos
                        cv.set_color(canvas_ity::fill_style, 0, 0, 0, 255);
                        cv.fill_text(employe.c_str(), 50, 220);
                        cv.fill_text(annee.c_str(), 50, 240);
                        cv.fill_text(mois.c_str(), 50, 260);
                        cv.fill_text(("date edition: " + dateTime).c_str(), 500, 220);
                        cv.fill_text(("Nom : " + nom).c_str(), 50, 310);
                        cv.fill_text(("Prénom : " + prenom).c_str(), 50, 330);
                        cv.fill_text(("Poste : " + poste).c_str(), 50, 350);
                        cv.fill_text(("Contrat : " + type_contrat).c_str(), 50, 370);
                        cv.fill_text(("Ville : " + ville).c_str(), 500, 310);

                        cv.fill_text(("Date d'embauche : " + date_embauche).c_str(), 500, 330);
                        cv.fill_text(("situation mat: " + sit_mat).c_str(), 500, 350);
                        cv.fill_text(("nombres enfants: " + std::to_string(nmbEnfant)).c_str(), 500, 370);

                        cv.fill_text(("Position solde: " + position_solde).c_str(), 50, 420);
                        cv.fill_text(("Position gestion: " + position_gestion).c_str(), 50, 450);
                        cv.fill_text(("No Identif: 5404" + dateNaissance).c_str(), 500, 450);
                        cv.fill_text(("Banque: " + banc).c_str(), 50, 480);
                        cv.fill_text(("no cpt: 0001" + std::to_string(nocpt)).c_str(), 500, 480);

                        // --- TABLEAU DES MONTANTS ---
                        // Dessiner l'entête du tableau
                        // cv.set_color(canvas_ity::fill_style, 236, 240, 241, 255); // Gris clair
                        // cv.fill_rectangle(50, 220, 700, 40);
                    
                    
                        cv.set_font(font_data.data(), (int)font_data.size(), 22.0f); // Gras pour le total
                        cv.set_color(canvas_ity::fill_style, 0, 0, 0, 255);
                        cv.fill_text("Code-elt", 50, 530);
                        cv.fill_text("Désignation", 200, 530);
                        cv.fill_text("Montant", 600, 530);

                        // Calculs
                        double cotisations = salaire_brut * 0.22;
                        double net = salaire_brut - cotisations;

                        // Lignes du tableau
                        cv.set_font(font_data.data(), (int)font_data.size(), 18.0f); // Gras pour les infos
                        cv.fill_text("560", 50, 560);
                        cv.fill_text("Salaire Brut", 200, 560);
                        cv.fill_text((std::to_string((double)salaire_brut) + " €").c_str(), 600, 560);

                    // cv.set_color(canvas_ity::fill_style, 192, 57, 43, 255); // Rouge pour les retenues//
                        cv.fill_text("561", 50, 580);
                        cv.fill_text("Cotisations Sociales (22%)", 200, 580);
                        cv.fill_text(("-" + std::to_string((double)cotisations) + " €").c_str(), 600, 580);
                        // cv.fill_text("562", 50, 600 );

                        cv.fill_text("563", 50, 620);
                        cv.fill_text("Prime mois ", 200, 620);
                        cv.fill_text("564", 50, 640);
                        cv.fill_text("Prime mission ", 200, 640);
                        cv.fill_text("565", 50, 660);
                        cv.fill_text("impos revenu Pers. Physique ", 200, 660);

                        // --- TOTAL NET ---
                        cv.set_color(canvas_ity::fill_style, 0, 0, 0, 255); // Vert pour le Net
                        // cv.fill_rectangle(50, 400, 700, 50);
                        
                        //cv.set_color(canvas_ity::fill_style, 255, 255, 255, 255);
                        cv.set_font(font_data.data(), (int)font_data.size(), 22.0f); // Gras pour le total
                        cv.fill_text("NET À PAYER", 200, 700);
                        cv.fill_text((std::to_string((double)net) + " €").c_str(), 600, 700);

                        //codeqr
                        // cv.set_color(canvas_ity::fill_style, 255, 255, 255, 255);
                        // cv.fill_rectangle(50 - 10, 750 - 10, 200 + 20, 200 + 20);
                        std::string infoQr = id_emp +   nom +  std::to_string((double)net) + " €";
                        dessinerQRCode(cv, infoQr, 50.0f, 750.0f, 4*50.0f); // Position et taille du QR code

                        // --- SAUVEGARDE ---
                        // La bibliothèque génère un tableau de pixels (RGBA)
                        std::vector<unsigned char> image(largeur * hauteur * 4);
                        cv.get_image_data(image.data(), largeur, hauteur, largeur * 4, 0, 0);
                        // -------------------------------------------------------------

                        // 1. Créer un buffer pour stocker les pixels (RGBA : 4 octets par pixel)
                        std::cout << "Tentative de création d'un buffer de : " << (largeur * hauteur * 4) << " octets" << std::endl;
                        std::vector<unsigned char> pixels(largeur * hauteur * 4);

                        // 2. Transférer le dessin du canvas vers notre buffer de pixels
                        cv.get_image_data(pixels.data(), largeur, hauteur, largeur * 4, 0, 0);

                        // 3. Ecrire le fichier PNG sur le disque
                        std::string nomFichier = "Fiche_Paie_" + id_emp + "_" + dateTime + ".png";
                        
                        // Paramètres : Nom, Largeur, Hauteur, Canaux (4 pour RGBA), Pixels, Pas (Largeur * 4)
                        int succes = stbi_write_png(nomFichier.c_str(), largeur, hauteur, 4, pixels.data(), largeur * 4);

                        if (succes) {
                            std::cout << "\033[32m[SUCCÈS]\033[0m Image générée : " << nomFichier << std::endl;
                            
                            // --- BONUS : Ouvrir l'image automatiquement (Windows) ---
                            std::string cmd = "start " + nomFichier;
                            system(cmd.c_str());
                        } else {
                            std::cerr << "\033[31m[ERREUR]\033[0m Impossible d'écrire le fichier PNG." << std::endl;
                        }
                }
             sqlite3_finalize(stmt);
            }
            else
            {
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << endl;
            }  


   
    }

    void DataBase::activerdesactiverEmployer()
    {
        std::cout << std::endl << std::endl << "--------------------Activer/Desactiver employe" << std::endl << std::endl;
        cout << "----Entrez l'identifiant : ";
         cin >> m_data.id_;

        DataBase BD("entreprise_.db");
        BD.rechercherUnEmploye_id(m_data.id_);
        int etat;

        string sql = "UPDATE EMPLOYE SET ETAT = ? WHERE ID = ?;";
        sqlite3_stmt* stmt;
        Employe e;

        int choix; bool valide;
        do
        {
            cout << "\n1. Activer le compte." << endl;
            cout << "2. Desactiver le compte." << endl;
            cout << "> ";
             cin >> choix;
            
            switch (choix)
            {
                case 1: etat = 1; valide = true; break;
                case 2: etat = 0; valide = true; break;
                
                default: cout << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Valeur incorrect !" << endl; valide = false;
                break;
            }
        }while (valide == false);

        
        sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL);

        sqlite3_bind_int(stmt, 1, etat);
        sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
            {
                if(sqlite3_changes(m_db) > 0)
                    cout << ANSI_BOLD << ANSI_GREEN << "[SUCCES]" << ANSI_RESET << "Mise a jour d'etat reussie !" << endl;
                else    
                    cout << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Aucun Employe trouve avec cet Id !" << ANSI_RESET << endl;
            }
        else
            {
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur lors de la mise a jour de l'etat : " << sqlite3_errmsg(m_db) << endl;
            }

        sqlite3_finalize(stmt);

    }


    //Fonctions pour Messagerie

    void DataBase::envoyer_MSG(string destinataire, string expediteur, string contenu, string objet)
    {
        string sql = "INSERT INTO MESSAGE (ID_DESTINATAIRE, ID_EXPEDITEUR, CONTENU_MESSAGE, OBJET, DATE_TIME, LU) VALUES(?, ?, ?, ?, datetime('now', 'localtime'), ?);";
        sqlite3_stmt* stmt;

        if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
            {
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de preparation : " << sqlite3_errmsg(m_db) << endl;
                return;
            }
        
        sqlite3_bind_text(stmt, 1, m_data_msg.setIDdestinataire(destinataire).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, m_data_msg.setIDexpediteur(expediteur).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, m_data_msg.setContenu(contenu).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, m_data_msg.setObjet(objet).c_str(), -1, SQLITE_TRANSIENT);
         sqlite3_bind_int(stmt, 5, 1);

        if(sqlite3_step(stmt) == SQLITE_DONE)
            cout << ANSI_BOLD << ANSI_GREEN << "[SUCCES]" << ANSI_RESET << "Message envoye avec succes !!" << endl;
        else 
            cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur d'envoi : " << sqlite3_errmsg(m_db) << ANSI_RESET << endl;
        
        sqlite3_finalize(stmt);

    }

    void DataBase::afficher_MSG()const
    {
           cout << "\n --- MESSAGES ENVOYES --- " << endl;
           dessinnerLignes();
           cout << "| " << left << setw(8) << "Id_Msg"
                << "| " << setw(10) << "Destinataire"
                << "| " << setw(10) << "Expediteur"
                << "| " << setw(10) << "Objet"
                << "| " << setw(10) << "Contenu"
                << "| " << setw(10) << "Date/Heure"
                << "| " << setw(4) << "Lu" << "|" << endl; 
            dessinnerLignes();

            sqlite3_stmt* stmt;
            string sql = "SELECT ID_MSG, ID_DESTINATAIRE, ID_EXPEDITEUR, OBJET, CONTENU_MESSAGE, DATE_TIME, LU FROM MESSAGE";

            if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
            {
                sqlite3_bind_text(stmt, 1, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);   
                while(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    cout << "| " << left << setw(8) << sqlite3_column_int(stmt, 0)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 1)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 2)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 3)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 4)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 5)
                         << "| " << setw(4) << sqlite3_column_int(stmt, 6) 
                         << "|" << endl;
                }
            }
            else
            {
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << endl;
            }
    }

    void DataBase::afficher_MSG_recus(string id_)const
    {
        cout << "\n --- MESSAGES RECUS --- " << endl;
           dessinnerLignes();
           cout << "| " << left << setw(8) << "Id_Msg"
                << "| " << setw(10) << "Expediteur"
                << "| " << setw(10) << "Objet"
                << "| " << setw(10) << "Contenu"
                << "| " << setw(10) << "Date/Heure"
                << "| " << setw(4) << "Lu" << "|" << endl; 
            dessinnerLignes();

            sqlite3_stmt* stmt;
            string sql = "SELECT ID_MSG, ID_EXPEDITEUR, OBJET, CONTENU_MESSAGE, DATE_TIME, LU FROM MESSAGE WHERE ID_DESTINATAIRE = ?;";

            if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
            {
                sqlite3_bind_text(stmt, 1, id_.c_str(), -1, SQLITE_TRANSIENT);
                while(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    cout << "| " << left << setw(8) << sqlite3_column_int(stmt, 0)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 1)
                         << "| " << setw(10) << (const char*)   sqlite3_column_text(stmt, 2)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 3)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 4)
                         << "| " << setw(4) << sqlite3_column_int(stmt, 5)
                         << "|" << endl;
                }
            }
            else
            {
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << endl;
            }
    }

        void DataBase::lire_MSG_recus(std::string id_user)
        {
            string sql = "UPDATE MESSAGE SET LU = 0 WHERE ID_DESTINATAIRE = ?;";
            sqlite3_stmt* stmt;

            if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
                {
                    cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de preparation : " << sqlite3_errmsg(m_db) << ANSI_RESET << endl;
                    return;
                }
            
            sqlite3_bind_text(stmt, 1, id_user.c_str(), -1, SQLITE_TRANSIENT);

            if(sqlite3_step(stmt) == SQLITE_DONE)
                cout << ANSI_BOLD << ANSI_GREEN << "[SUCCES]" << ANSI_RESET << "Messages marques comme lus avec succes !!" << ANSI_RESET << endl;
            else 
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de mise a jour : " << sqlite3_errmsg(m_db) << ANSI_RESET << endl;
            
            sqlite3_finalize(stmt);
        }


        void DataBase::afficher_MSG_non_lus(string id_)const
    {
        cout << "\n --- MESSAGES NON LUS --- " << endl; 
           dessinnerLignes();
           cout << "| " << left << setw(8) << "Id_Msg"
                << "| " << setw(10) << "Expediteur"
                << "| " << setw(10) << "Objet"
                << "| " << setw(10) << "Contenu"
                << "| " << setw(10) << "Date/Heure" 
                << "|" << endl; 
            dessinnerLignes();

            sqlite3_stmt* stmt;
            string sql = "SELECT ID_MSG, ID_EXPEDITEUR, OBJET, CONTENU_MESSAGE, DATE_TIME FROM MESSAGE WHERE ID_DESTINATAIRE = ? AND LU = 1;";

            if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
            {
                sqlite3_bind_text(stmt, 1, id_.c_str(), -1, SQLITE_TRANSIENT);
                while(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    cout << "| " << left << setw(8) << sqlite3_column_int(stmt, 0)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 1)
                         << "| " << setw(10) << (const char*)   sqlite3_column_text(stmt, 2)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 3)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 4)
                         //<< "| " << setw(4) << sqlite3_column_int(stmt, 5)
                         << "|" << endl;
                }

                    sqlite3_finalize(stmt);

                    DataBase message("entreprise_.db");
                    message.lire_MSG_recus(id_);
            }else
            {
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << ANSI_RESET << endl;
            }  


    }


        void DataBase:: afficher_MSG_lus(std::string id_user)const
        {
            cout << "\n --- MESSAGES LUS --- " << endl; 
           dessinnerLignes();
           cout << "| " << left << setw(8) << "Id_Msg"
                << "| " << setw(10) << "Expediteur"
                << "| " << setw(10) << "Objet"
                << "| " << setw(10) << "Contenu"
                << "| " << setw(10) << "Date/Heure" 
                << "|" << endl; 
            dessinnerLignes();

            sqlite3_stmt* stmt;
            string sql = "SELECT ID_MSG, ID_EXPEDITEUR, OBJET, CONTENU_MESSAGE, DATE_TIME FROM MESSAGE WHERE ID_DESTINATAIRE = ? AND LU = 0;";

            if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
            {
                sqlite3_bind_text(stmt, 1, id_user.c_str(), -1, SQLITE_TRANSIENT);
                while(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    cout << "| " << left << setw(8) << sqlite3_column_int(stmt, 0)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 1)
                         << "| " << setw(10) << (const char*)   sqlite3_column_text(stmt, 2)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 3)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 4)
                         //<< "| " << setw(4) << sqlite3_column_int(stmt, 5)
                         << "|" << endl;
                }
            }
            else
            {
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << ANSI_RESET << endl;
            }  
            sqlite3_finalize(stmt);
        }


        void DataBase::afficher_MSG_envoyes(std::string id_user)const
        {
            cout << "\n --- MESSAGES ENVOYES --- " << endl; 
           dessinnerLignes();
           cout << "| " << left << setw(8) << "Id_Msg"
                << "| " << setw(10) << "Destinataire"
                << "| " << setw(10) << "Objet"
                << "| " << setw(10) << "Contenu"
                << "| " << setw(10) << "Date/Heure" 
                << "|" << endl; 
            dessinnerLignes();

            sqlite3_stmt* stmt;
            string sql = "SELECT ID_MSG, ID_DESTINATAIRE, OBJET, CONTENU_MESSAGE, DATE_TIME FROM MESSAGE WHERE ID_EXPEDITEUR = ?;";

            if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
            {
                sqlite3_bind_text(stmt, 1, id_user.c_str(), -1, SQLITE_TRANSIENT);
                while(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    cout << "| " << left << setw(8) << sqlite3_column_int(stmt, 0)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 1)
                         << "| " << setw(10) << (const char*)   sqlite3_column_text(stmt, 2)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 3)
                         << "| " << setw(10) << (const char*)sqlite3_column_text(stmt, 4)
                         //<< "| " << setw(4) << sqlite3_column_int(stmt, 5)
                         << "|" << endl;
                }
            }
            else
            {
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << ANSI_RESET << endl;
            }  
            sqlite3_finalize(stmt);
        }

        string affichageMessageRecusNonLus(std::string texte)
        {
            stringstream ss(texte);
            string mot, resultat;
            int compteur = 0;

            while(ss >> mot && compteur < 3)
            {
                if(compteur > 0) 
                {
                    resultat += " ";
                }
                
                resultat += mot;
                compteur++;
            }

            if(ss >> mot)
            {
                resultat += "...";
            }

            return resultat;
        }

        void DataBase:: message_RNL(std::string id_user)
        {
            sqlite3_stmt* stmt;
            string sql = "SELECT ID_EXPEDITEUR, OBJET, CONTENU_MESSAGE, DATE_TIME FROM MESSAGE WHERE ID_DESTINATAIRE = ? AND LU = 1;";

            if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
            {
                sqlite3_bind_text(stmt, 1, id_user.c_str(), -1, SQLITE_TRANSIENT);
                while(sqlite3_step(stmt) == SQLITE_ROW)
                {                    string contenu = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                    string objet = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                    string expediteur   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                    string date_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                    string message = ANSI_BLUE + ANSI_BOLD +  expediteur +  ANSI_RESET + ANSI_CYAN + ANSI_BOLD  + " " + objet + " - " + ANSI_RESET + ANSI_YELLOW + affichageMessageRecusNonLus(contenu) + ANSI_RESET+ ANSI_BOLD + ANSI_BLUE + " [" + date_time + "]" + ANSI_RESET;
                    std::cout << message << std::endl;
                }
                
            }
            else
            {
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << ANSI_RESET << endl;
            }  
            sqlite3_finalize(stmt);
        }

        std::vector<Data_Message> DataBase::recupererMessages(std::string id_user)
        {
            std::vector<Data_Message> messages;
            sqlite3_stmt* stmt;
            string sql = "SELECT ID_EXPEDITEUR, CONTENU_MESSAGE, DATE_TIME FROM MESSAGE WHERE ID_DESTINATAIRE = ?;";

            if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
            {
                sqlite3_bind_text(stmt, 1, id_user.c_str(), -1, SQLITE_TRANSIENT);
                while(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    Data_Message msg;
                    msg.setIDexpediteur(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                    msg.setContenu(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
                    msg.setDateTime(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
                    
                    messages.push_back(msg);
                }
            }
            else
            {
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << ANSI_RESET << endl;
            }  

            sqlite3_finalize(stmt);

            return messages;
        }

//Discussion entre deux utilisateurs
        std::string DataBase::afficherDiscussion(string id_user, string id_correspondant)const
        {
            sqlite3_stmt* stmt;
            string sql = "SELECT ID_EXPEDITEUR, OBJET, CONTENU_MESSAGE, DATE_TIME FROM MESSAGE WHERE (ID_DESTINATAIRE = ? AND ID_EXPEDITEUR = ?) OR (ID_DESTINATAIRE = ? AND ID_EXPEDITEUR = ?) ORDER BY DATE_TIME;";

            if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
            {
                sqlite3_bind_text(stmt, 1, id_user.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 2, id_correspondant.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 3, id_correspondant.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 4, id_user.c_str(), -1, SQLITE_TRANSIENT);

                while(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    string expediteur = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                    string objet = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                    string contenu = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                    string date_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                    //string message = ANSI_BLUE + ANSI_BOLD +  expediteur +  ANSI_RESET + ANSI_CYAN + ANSI_BOLD  + " " + contenu + " - " + ANSI_RESET + ANSI_YELLOW + date_time + ANSI_RESET;
                    //std::cout << message << std::endl;

                        if(expediteur == id_user)
                        {
                            cout << ANSI_MAGENTA<< ANSI_BOLD << "                    [Moi]" << "[" << objet << "]" << std::endl << ANSI_RESET << "                    "  << contenu << ANSI_YELLOW << " " << date_time << ANSI_RESET << endl;
                        }
                        else
                        {
                            cout << ANSI_GREEN << ANSI_BOLD << "[" << expediteur << "]" << "[" << objet << "]" << ANSI_RESET << std::endl << contenu << " " << ANSI_YELLOW << date_time << ANSI_RESET << endl;
                        }
                }

                sqlite3_finalize(stmt);
                return id_correspondant;
            }
            else
            {
                cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << ANSI_RESET << endl;
                return "";
            }  
            
        }

//Selectionner les expediteurs d'un destinataire
    std::string DataBase::selectionnerExpediteur(string id_user)
    {
        string sql = "SELECT DISTINCT ID_EXPEDITEUR FROM MESSAGE WHERE ID_DESTINATAIRE = ?;"; //on precise avec distinct pour eviter les doublons
        sqlite3_stmt* stmt;

        if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, id_user.c_str(), -1, SQLITE_TRANSIENT);

            std::cout << "\nDiscussions ==========" << std::endl << std::endl;
            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                string expediteur = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                cout << "            - " << expediteur << endl;
            }

            sqlite3_finalize(stmt);
            
            string id_correspondant;
            cout << std::endl << "Selectionnez la discussion (Identifiant): ";
             cin >> id_correspondant;

            return id_correspondant;
        }
        else
        {
            cerr << ANSI_BOLD << ANSI_RED << "[ERREUR]" << ANSI_RESET << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << ANSI_RESET << endl;
            return "";
        }



    }

    void afficherLigneEmploye(sqlite3_stmt *stmt_)
    {
        cout << "| " << left << setw(8) << (const char*)sqlite3_column_text(stmt_, 0)
             << "| " << setw(10) << (const char*)sqlite3_column_text(stmt_, 1)
             << "| " << setw(10) << (const char*)sqlite3_column_text(stmt_, 2)
             << "| " << setw(10) << (const char*)sqlite3_column_text(stmt_, 3)
             << "| " << setw(10) << (const char*)sqlite3_column_text(stmt_, 4)
             << "| " << setw(14) << (const char*)sqlite3_column_text(stmt_, 5)             
             << "| " << setw(18) << (const char*)sqlite3_column_text(stmt_, 6)
             << "| " << setw(3) << (const char*)sqlite3_column_text(stmt_, 7)
             << "| " << setw(12) << sqlite3_column_double(stmt_, 8)
             << "| " << setw(1) << (const char*)sqlite3_column_text(stmt_, 9)
            //  << "| " << setw(14) << (const char*)sqlite3_column_text(stmt_, 10)
             << "| " << setw(25) << (const char*)sqlite3_column_text(stmt_, 11) 
             << "| " << setw(1) << (const char*)sqlite3_column_text(stmt_, 12) << "|" << endl; 
    }

    void dessinnerLignes()
    {
    cout << "+"
         << setfill('-')
         << setw(10)
         << "+" 
         << setw(12)
         << "+" 
         << setw(12)         
         << "+"
         << setw(12)         
         << "+"
         << setw(12)
         << "+"        
         << setw(16)
         << "+" 
         << setw(20)         
         << "+"
         << setw(5)         
         << "+"
         << setw(14)
         << "+"       
         << setw(3)
         << "+" 
        //  << setw(16)         
        //  << "+"
         << setw(27)         
         << "+"         
         << setw(3)         
         << "+"
         << setfill(' ')
         << endl;
    }