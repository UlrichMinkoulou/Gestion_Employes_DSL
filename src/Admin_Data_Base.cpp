#include "Admin_Data_Base.h"
#include "Data_base.h"
#include "Administrateur.h"
#include <string>
#include <iomanip>
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <sstream>

void clearCLI()
{
     system("clear");
}

Admin_db::Admin_db(const char* fileName)
{
    if(sqlite3_open(fileName, &m_bd) != SQLITE_OK)
        std::cerr << "Erreur d'ouverture de la BD : " << sqlite3_errmsg(m_bd) << std::endl;
    else
    std::cout << "Base de donnees ouverte avec succes !" << std::endl;
    
    char * msg_err;
    std::string sqlCreate = "CREATE TABLE IF NOT EXISTS ADMIN ("
                            "ID TEXT NOT NULL,"
                            "NOM TEXT NOT NULL,"
                            "ETAT TEXT NOT NULL,"
                            "MDP TEXT NOT NULL);";

    sqlite3_exec(m_bd, sqlCreate.c_str(), NULL, 0, &msg_err);
}

Admin_db::~Admin_db()
{
    sqlite3_close(m_bd);
}



void Admin_db::ajouterAdmin()
{
    Admin admin;
    std::string id = generateurID(m_bd, "ADSL", 'a');
    
    
    std::string sqlInsert = "INSERT INTO ADMIN (ID, NOM, ETAT, MDP) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if(sqlite3_prepare_v2(m_bd, sqlInsert.c_str(), -1, &stmt, NULL) != SQLITE_OK)
        std::cerr << "Erreur de preparation : " << sqlite3_errmsg(m_bd) << std::endl;
    
    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);   
    sqlite3_bind_text(stmt, 2, admin.getNom().c_str(), -1, SQLITE_TRANSIENT);   
    sqlite3_bind_text(stmt, 3, admin.activation().c_str(), -1, SQLITE_TRANSIENT);  
    sqlite3_bind_text(stmt, 4, admin.getMot_de_passe().c_str(), -1, SQLITE_TRANSIENT);  
    
    if(sqlite3_step(stmt) == SQLITE_DONE)
    std::cout << "\nAdmin ajoute avec le succes !" << std::endl;
    else    
    std::cerr << "\nErreur lors de l'insertion : " << sqlite3_errmsg(m_bd) <<  std::endl;

    sqlite3_finalize(stmt);
}

void Admin_db::afficherAdmin()
{
    /* a Coder urgement */
    dessinnerRow();
    std::cout << "| " << std::left << std::setw(8) << "Id"
              << "| " << std::setw(10) << "NOM"
              << "| " << std::setw(5)  << "ETAT"
              << "| " << std::setw(14) << "MDP" << "| " << std::endl;
              dessinnerRow();

              std::string sql = "SELECT ID, NOM, ETAT, MDP FROM ADMIN;";
              sqlite3_stmt* stmt;

              sqlite3_prepare_v2(m_bd, sql.c_str(), -1, &stmt, NULL);
              while(sqlite3_step(stmt) == SQLITE_ROW) afficherLigneAdmin(stmt);
              dessinnerRow();
              
    sqlite3_finalize(stmt);
    
}


void Admin_db::afficherUserAdmin(std::string identifiant)
{
    /* a Coder urgement */
    dessinnerRow();
    std::cout << "| " << std::left << std::setw(8) << "Id"
              << "| " << std::setw(10) << "NOM"
              << "| " << std::setw(5) << "ETAT"
              << "| " << std::setw(14) << "MDP" << "| " << std::endl;
              dessinnerRow();
              
    std::string sql = "SELECT ID, NOM, ETAT, MDP FROM ADMIN WHERE ID = ?;";
    sqlite3_stmt* stmt;
    
    if(sqlite3_prepare_v2(m_bd, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, identifiant.c_str(), -1, SQLITE_TRANSIENT);
            while(sqlite3_step(stmt) == SQLITE_ROW) afficherLigneAdmin(stmt);
            dessinnerRow();
            sqlite3_finalize(stmt);
        }
}

bool Admin_db::connexionAdmin(std::string id_)
{
    std::time_t start = std::time(nullptr);
    std::string mdp;
    char ch;

    m_data_con.id_ = id_;
    
    std::cout << "> Mot de passe : " << std::flush;
    mdp = getPassword();
    std::cout << "votre mot de passe : " << mdp << std::endl;
    
    std::string sql_search = "SELECT ID, NOM, ETAT, MDP FROM ADMIN WHERE ID = ? AND MDP = ?;";
    sqlite3_stmt* stmt;
    
    if(sqlite3_prepare_v2(m_bd, sql_search.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
            sqlite3_bind_text(stmt, 1, m_data_con.id_.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, mdp.c_str(), -1, SQLITE_TRANSIENT);
            m_data_con.res = true;
            if(sqlite3_step(stmt) == SQLITE_ROW)
            {
                    const unsigned char* user_name = sqlite3_column_text(stmt,1);
                    std::cout << "Bienvenu " << (user_name ? reinterpret_cast<const char*>(user_name) : "NULL") << std::endl;
                    m_data_con.name = reinterpret_cast<const char*>(user_name);
                    
                    std::cout << "Chargement de la page";
                    for(int i = 0; i < 8; ++i)
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        std::cout << "." << std::flush;
                    } 

                    clearCLI();
                } else
                    {
                        m_data_con.res = false;
                        std::cout << "Id ou mot de passe Incorrect !" << std::endl;
                    }

                }else
                {
                    std::cerr << "Erreur de preparation : " << sqlite3_errmsg(m_bd) << std::endl;
            }
            sqlite3_finalize(stmt);
            return m_data_con.res;
        }

void Admin_db::modifierAdmin(std::string id_employe)
        {
            Admin_db admin_User("dataBase_admin.db");

            std::cout << "----Modification des Infos de Connexion: " << std::endl << std::endl;
            std::string sql = "UPDATE ADMIN SET MDP=? WHERE ID = ?;";
            sqlite3_stmt* stmt;
            sqlite3_prepare_v2(m_bd, sql.c_str(), -1, &stmt, NULL);
            sqlite3_bind_text(stmt, 1, admin_User.getMot_de_passe().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, id_employe.c_str(), -1, SQLITE_TRANSIENT);

            if(sqlite3_step(stmt) == SQLITE_DONE)
            {
                if(sqlite3_changes(m_bd) > 0)
                    std::cout << "Mise a jour reussie !" << std::endl;
                else    
                    std::cout << "Acun Administrateur ne possede cet identifiant " << std::endl;
            } else
                {
                    std::cerr << "Erreur lors de la mise a jour : " << sqlite3_errmsg(m_bd) << std::endl;
                }
            sqlite3_finalize(stmt);
        }

void afficherLigneAdmin(sqlite3_stmt* stmt)
    {
        std::cout << "| " << std::left << std::setw(8) << (const char*)sqlite3_column_text(stmt, 0)
                  << "| " << std::setw(10) << (const char*)sqlite3_column_text(stmt, 1)
                  << "| " << std::setw(5) << (const char*)sqlite3_column_text(stmt, 2)
                  << "| " << std::setw(14) << (const char*)sqlite3_column_text(stmt, 3) << "|" << std::endl; 
  
    }

void dessinnerRow()
{
    std::cout << "+"
    << std::setfill('-')
    << std::setw(10)
    << "+" 
    << std::setw(12)
    << "+"                
    << std::setw(7)
    << "+"          
    << std::setw(16)         
    << "+"
              << std::setfill(' ')
              << std::endl; 
            }


std::string generateurID(sqlite3* bd, std::string c, char m)
    {
        sqlite3_stmt* stmt;
        int dernierNumero = 0;
        std::string sql ;
        if(m == 'a')
            sql = "SELECT MAX(CAST(SUBSTR(ID, 5) AS INTEGER)) FROM ADMIN;";
        else
            sql = "SELECT MAX(CAST(SUBSTR(ID, 5) AS INTEGER)) FROM EMPLOYE;";

        if(sqlite3_prepare_v2(bd, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
        {
            if(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    dernierNumero = sqlite3_column_int(stmt, 0);
                }
        }

        sqlite3_finalize(stmt);

        //incrementation
        int nouveauNumero = dernierNumero + 1;

        //Formatage ADSL + numero sur 4 chiffres (ex: 0001)
        std::stringstream ss;
        ss << c << std::setfill('0') << std::setw(4) << nouveauNumero;

        return ss.str();
    }