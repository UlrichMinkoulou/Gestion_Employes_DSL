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
                        "AGE INTEGER,"
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
        string id_user = generateurID(m_db, "EDSL", 'e');
        
        Employe e;
            
        string sqlInsert = "INSERT INTO EMPLOYE (ID, NOM, PRENOM, AGE, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, EMAIL, MDP, ETAT) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
        sqlite3_stmt *stmt;

        if(sqlite3_prepare_v2(m_db, sqlInsert.c_str(), -1, &stmt, NULL) != SQLITE_OK)
        {
            cerr << "Erreur de preparation : " << sqlite3_errmsg(m_db) << endl; 
            return;
        }


        sqlite3_bind_text(stmt, 1, id_user.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, e.getNom().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, e.getPrenom().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, e.getAge());
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
            cout << "\nEmploye ajoute avec succes !! " <<endl;
        else
            cerr << "\nErreur  lors de l'insertion : " << sqlite3_errmsg(m_db) << endl;
            
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
         << "| " << setw(4) << "AGE"        
         << "| " << setw(10) << "DATE_ADHE"
         << "| " << setw(14) << "SITUATION_MAT"
         << "| " << setw(10) << "POSTE"        
         << "| " << setw(12) << "TYPECONTRAT"
         << "| " << setw(8) << "SALAIRE"        
         << "| " << setw(10) << "CATEGORIE"
         << "| " << setw(14) << "MDP"
         << "| " << setw(18) << "EMAIL"
         << "| " << setw(18) << "ETAT" << "|" << endl;

    dessinnerLignes();


    string sql = "SELECT ID, NOM, PRENOM, AGE, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, MDP, EMAIL, ETAT FROM EMPLOYE;";
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
         << "| " << setw(4) << "AGE"        
         << "| " << setw(10) << "DATE_ADHE"
         << "| " << setw(14) << "SITUATION_MAT"
         << "| " << setw(10) << "POSTE"        
         << "| " << setw(12) << "TYPECONTRAT"
         << "| " << setw(8) << "SALAIRE"        
         << "| " << setw(10) << "CATEGORIE"
         << "| " << setw(14) << "MDP"
         << "| " << setw(18) << "EMAIL"
         << "| " << setw(18) << "ETAT" << "|" << endl;

    dessinnerLignes();


    string sql = "SELECT ID, NOM, PRENOM, AGE, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, MDP, EMAIL, ETAT FROM EMPLOYE WHERE ID =?;";
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
        string sql = "SELECT ID, NOM, PRENOM, AGE, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, MDP, EMAIL, ETAT FROM EMPLOYE WHERE NOM LIKE ?;";
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

            if(!trouve) cout << "| Aucun Employe trouve pour ce nom. " << setw(15) << " " << "|" << endl;
            dessinnerLignes();
        }
        sqlite3_finalize(stmt);
    }

    void DataBase::rechercherUnEmploye_id (string identifiant)
    {


        // recherche = "%" + recherche + "%"; // format pour le LIkE SQL
        sqlite3_stmt *stmt;
        string sql = "SELECT ID, NOM, PRENOM, AGE, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, MDP, EMAIL, ETAT FROM EMPLOYE WHERE ID = ?;";
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

            if(!trouve) cout << "| Aucun Employe trouve pour cet identifiant. " << setw(15) << " " << "|" << endl;
            dessinnerLignes();
        }
        sqlite3_finalize(stmt);
    }


//Modification des infos Employe
    void DataBase::changerInfoEmploye()
    {

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
    std::cout << "4. Age " << std::endl;
    std::cout << "5. Date d'adhesion " << std::endl;
    std::cout << "6. Situation Matrimonial " << std::endl;
    std::cout << "7. Poste " << std::endl;
    std::cout << "8. Type de contrat " << std::endl;
    std::cout << "9. Salaire " << std::endl;
    std::cout << "10. Mot de passe " << std::endl;
    std::cout << "11. Email " << std::endl;
    std::cout << "> ";
     std::cin >> choix;


    switch (choix)
    {
        case 1: 
                {
                    string sql = "UPDATE EMPLOYE SET NOM=?, PRENOM=?, AGE=?, DATE_ADHE=?, SITUATION_MAT=?, POSTE=?, TYPECONTRAT=?, SALAIRE=?, CATEGORIE=?, MDP=?, EMAIL=?, ETAT=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 13, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 1, e.getNom().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, e.getPrenom().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_int(stmt, 3, e.getAge());
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
                                cout << "Mise a jour reussie !" << endl;
                            else    
                                cout << "Aucun Employe trouve avec cet Id !" << endl;
                        }
                    else
                        {
                            cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
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
                                cout << "Mise a jour reussie !" << endl;
                            else    
                                cout << "Aucun Employe trouve avec cet Id !" << endl;
                        }
                    else
                        {
                            cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
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
                                cout << "Mise a jour reussie !" << endl;
                            else    
                                cout << "Aucun Employe trouve avec cet Id !" << endl;
                        }
                    else
                        {
                            cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }

        case 4: 
                {
                    string sql = "UPDATE EMPLOYE SET AGE=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_int(stmt, 1, e.getAge());
                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << "Mise a jour reussie !" << endl;
                            else    
                                cout << "Aucun Employe trouve avec cet Id !" << endl;
                        }
                    else
                        {
                            cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
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
                                cout << "Mise a jour reussie !" << endl;
                            else    
                                cout << "Aucun Employe trouve avec cet Id !" << endl;
                        }
                    else
                        {
                            cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
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
                                cout << "Mise a jour reussie !" << endl;
                            else    
                                cout << "Aucun Employe trouve avec cet Id !" << endl;
                        }
                    else
                        {
                            cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
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
                                cout << "Mise a jour reussie !" << endl;
                            else    
                                cout << "Aucun Employe trouve avec cet Id !" << endl;
                        }
                    else
                        {
                            cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
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
                                cout << "Mise a jour reussie !" << endl;
                            else    
                                cout << "Aucun Employe trouve avec cet Id !" << endl;
                        }
                    else
                        {
                            cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }

        case 9: 
                {
                    string sql = "UPDATE EMPLOYE SET SALAIRE=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_double(stmt, 1, e.getSalaire());
                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << "Mise a jour reussie !" << endl;
                            else    
                                cout << "Aucun Employe trouve avec cet Id !" << endl;
                        }
                    else
                        {
                            cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }

        case 10: 
                {
                    string sql = "UPDATE EMPLOYE SET CATEGORIE=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 1, e.getCategorie().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << "Mise a jour reussie !" << endl;
                            else    
                                cout << "Aucun Employe trouve avec cet Id !" << endl;
                        }
                    else
                        {
                            cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }

        case 11:
                {
                    string sql = "UPDATE EMPLOYE SET MDP=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 1, e.getMot_de_passe().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << "Mise a jour reussie !" << endl;
                            else    
                                cout << "Aucun Employe trouve avec cet Id !" << endl;
                        }
                    else
                        {
                            cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
                        }

                    sqlite3_finalize(stmt);
                    break;
                }
                
        case 12: 
                {
                    string sql = "UPDATE EMPLOYE SET EMAIL=?  WHERE ID=?;";
                    sqlite3_stmt *stmt;
                    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL); //laissez ohne v2

                    sqlite3_bind_text(stmt, 1, e.getEmail().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

    
                    if(sqlite3_step(stmt) == SQLITE_DONE) 
                        {
                            if(sqlite3_changes(m_db) > 0)
                                cout << "Mise a jour reussie !" << endl;
                            else    
                                cout << "Aucun Employe trouve avec cet Id !" << endl;
                        }
                    else
                        {
                            cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_db) << endl;
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

    bool DataBase::connexionEmploye(std::string id_)
    {

    std::time_t start = std::time(nullptr);
    std::string mot_de_passe; char ch;
    
    m_data.id_ = id_;

    std::cout << "> Mot de passe : " << std::flush;
    mot_de_passe = getPassword();
    std::cout << "votre mot de passe : " << mot_de_passe << std::endl;


    std::string sql_recherche = "SELECT ID, NOM, PRENOM, AGE, DATE_ADHE, SITUATION_MAT, POSTE, TYPECONTRAT, SALAIRE, CATEGORIE, MDP, EMAIL, ETAT FROM EMPLOYE WHERE ID = ? AND MDP = ?;";
    sqlite3_stmt *stmt;

    if(sqlite3_prepare_v2(m_db, sql_recherche.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, mot_de_passe.c_str(), -1, SQLITE_TRANSIENT);
        m_data.res = true;

        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
             const unsigned char *user_name = sqlite3_column_text(stmt, 1);
             std::cout << "Bienvenu " << (user_name ? reinterpret_cast<const char*>(user_name) : "NULL") << std::endl;
            m_data.name =  reinterpret_cast<const char*>(user_name); //faire le caste

            std::cout << "Chargement de la page";
            for(int i = 0; i < 8; ++i)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "." << std::flush;
            } 

            system("clear");
        }
         else
         {
            m_data.res = false;
            std::cout << "Id ou mot de passe Incorrect !" << std::endl;
         }

    } 
      else 
        {
            std::cerr << "Erreur de preparation : " << sqlite3_errmsg(m_db) << std::endl;
        }
    
    sqlite3_finalize(stmt);

    return m_data.res;

    }

    void DataBase::imprimer_fiche_paie(std::string id_){ std::cout << "Fonction Non Disponible !!!" << std::endl;}

    void DataBase::activerdesactiverEmployer()
    {
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
                
                default: cout << "Valeur incorrect !" << endl; valide = false;
                break;
            }
        }while (valide == false);

        
        sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL);

        sqlite3_bind_int(stmt, 1, etat);
        sqlite3_bind_text(stmt, 2, m_data.id_.c_str(), -1, SQLITE_TRANSIENT);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
            {
                if(sqlite3_changes(m_db) > 0)
                    cout << "Mise a jour d'etat reussie !" << endl;
                else    
                    cout << "Aucun Employe trouve avec cet Id !" << endl;
            }
        else
            {
                cerr << "Erreur lors de la mise a jour de l'etat : " << sqlite3_errmsg(m_db) << endl;
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
                cerr << "Erreur de preparation : " << sqlite3_errmsg(m_db) << endl;
                return;
            }
        
        sqlite3_bind_text(stmt, 1, m_data_msg.setIDdestinataire(destinataire).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, m_data_msg.setIDexpediteur(expediteur).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, m_data_msg.setContenu(contenu).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, m_data_msg.setObjet(objet).c_str(), -1, SQLITE_TRANSIENT);
         sqlite3_bind_int(stmt, 5, 1);

        if(sqlite3_step(stmt) == SQLITE_DONE)
            cout << "\nMessage envoye avec succes !!" << endl;
        else 
            cerr << "\nErreur d'envoi : " << sqlite3_errmsg(m_db) << endl;
        
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
                cerr << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << endl;
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
                cerr << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << endl;
            }
    }

        void DataBase::lire_MSG_recus(std::string id_user)
        {
            string sql = "UPDATE MESSAGE SET LU = 0 WHERE ID_DESTINATAIRE = ?;";
            sqlite3_stmt* stmt;

            if(sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
                {
                    cerr << "Erreur de preparation : " << sqlite3_errmsg(m_db) << endl;
                    return;
                }
            
            sqlite3_bind_text(stmt, 1, id_user.c_str(), -1, SQLITE_TRANSIENT);

            if(sqlite3_step(stmt) == SQLITE_DONE)
                cout << "\nMessages marques comme lus avec succes !!" << endl;
            else 
                cerr << "\nErreur de mise a jour : " << sqlite3_errmsg(m_db) << endl;
            
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

                    DataBase message("entreprise_.db");
                    message.lire_MSG_recus(id_);
            }
            else
            {
                cerr << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << endl;
            }  
            sqlite3_finalize(stmt);

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
                cerr << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << endl;
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
                cerr << "Erreur de preparation de la requete : " << sqlite3_errmsg(m_db) << endl;
            }  
            sqlite3_finalize(stmt);
        }



    void afficherLigneEmploye(sqlite3_stmt *stmt_)
    {
        cout << "| " << left << setw(8) << (const char*)sqlite3_column_text(stmt_, 0)
             << "| " << setw(10) << (const char*)sqlite3_column_text(stmt_, 1)
             << "| " << setw(10) << (const char*)sqlite3_column_text(stmt_, 2)
             << "| " << setw(4) << sqlite3_column_int(stmt_, 3)
             << "| " << setw(10) << (const char*)sqlite3_column_text(stmt_, 4)
             << "| " << setw(14) << (const char*)sqlite3_column_text(stmt_, 5)             
             << "| " << setw(10) << (const char*)sqlite3_column_text(stmt_, 6)
             << "| " << setw(12) << (const char*)sqlite3_column_text(stmt_, 7)
             << "| " << setw(8) << sqlite3_column_double(stmt_, 8)
             << "| " << setw(10) << (const char*)sqlite3_column_text(stmt_, 9)
             << "| " << setw(14) << (const char*)sqlite3_column_text(stmt_, 10)
             << "| " << setw(18) << (const char*)sqlite3_column_text(stmt_, 11) 
             << "| " << setw(18) << (const char*)sqlite3_column_text(stmt_, 12) << "|" << endl; 
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
         << setw(6)         
         << "+"
         << setw(12)
         << "+"        
         << setw(16)
         << "+" 
         << setw(12)         
         << "+"
         << setw(14)         
         << "+"
         << setw(10)
         << "+"       
         << setw(12)
         << "+" 
         << setw(16)         
         << "+"
         << setw(20)         
         << "+"         
         << setw(20)         
         << "+"
         << setfill(' ')
         << endl;
    }