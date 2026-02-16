#ifndef DEF_DATA_BASE
#define DEF_DATA_BASE

#include "Employe.h"
#include <string>
#include <sqlite3.h>
#include <vector>

struct  DataConnxion
    {
        std::string id_;
        bool res;
        int id_user;
        std::string name;
    };

struct Data_Message
    {
        private:
        std::string id_destinataire;
        std::string id_expediteur;
        std::string contenu;
        int lu; //variable de lecture des messages 0(non lu) et 1(lu)

        public:
        Data_Message(): lu(0) {}; //initialisation de lu a 0 (non lu);

        //getteur
        std::string getIDdestinataire(){
            
            return id_destinataire;
        }

        std::string getIDexpediteur() {
            return id_expediteur;
        }
        
        std::string getContenu(){
                return contenu;
            }

        //seteurs
        std::string setIDdestinataire(std::string id_dest)
        {
            id_destinataire = id_dest;
            return id_destinataire;
        }

        std::string setIDexpediteur(std::string id_exp)
        {
            id_expediteur = id_exp;
            return id_expediteur;
        }

        std::string setContenu(std::string cont)
        {
            contenu = cont;
            return contenu;
        }



    };

class DataBase
{
    public:
        DataBase(const char* nomFichier);
        ~DataBase();
        void ajouterEmploye();
        void rechercherUnEmploye()const;
        void activerdesactiverEmployer();
        void changerInfoEmploye();
        void afficherEmploye();
        void afficherUser(std::string idUser);
        bool connexionEmploye(std::string id_);
        void imprimer_fiche_paie(std::string id_);
        void rechercherUnEmploye_id(std::string id_);
        DataConnxion getData(){return m_data;}

        //variables pour l'implementation de l'envoi des messages
        void envoyer_MSG(std::string destinataire, std::string expediteur, std::string contenu);
        void lecture_MSG();
        void afficher_MSG()const;

    private:
        sqlite3 *m_db;
        DataConnxion m_data;

        Data_Message m_data_msg;
        std::vector<Data_Message> m_liste_messages; //pour stocker les messages dans un vecteur

};

void afficherLigneEmploye(sqlite3_stmt *stmt);
void dessinnerLignes();
std::string getPassword();

#endif