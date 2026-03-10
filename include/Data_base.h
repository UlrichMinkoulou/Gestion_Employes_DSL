#ifndef DEF_DATA_BASE
#define DEF_DATA_BASE

#include "Employe.h"
#include <string>
#include <sqlite3.h>
#include <vector>



// #define CANVAS_ITY_IMPLEMENTATION
#include "canvas_ity.hpp"



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
        std::string objet;
        std::string contenu;
        std::string date_time;
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
        
        std::string getObjet(){
                return objet;
            }

        std::string getDatetime(){
                return date_time;
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
        
        std::string setObjet(std::string obt)
        {
            objet = obt;
            return objet;
        }

        std::string setDateTime(std::string dt)
        {
            date_time = dt;
            return date_time;
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
        void imprimer_fiche_paie(std::string id_emp);
        void rechercherUnEmploye_id(std::string id_);
        void mofifierInfoConnexionEmploye(std::string id_);
        
        bool verif_if(std::string identifiant);

        DataConnxion getData(){return m_data;}

        //variables pour l'implementation de l'envoi des messages
        void envoyer_MSG(std::string destinataire, std::string expediteur, std::string contenu, std::string objet);
        void lecture_MSG();
        void afficher_MSG()const;
        void afficher_MSG_recus(std::string id_user)const;
        void lire_MSG_recus(std::string id_user);
        void afficher_MSG_non_lus(std::string id_user)const;
        void afficher_MSG_lus(std::string id_user)const;
        void afficher_MSG_envoyes(std::string id_user)const;
        void message_RNL(std::string id_user);
        std::vector<Data_Message> recupererMessages(std::string id_user);
        std::string afficherDiscussion(std::string id_user, std::string id_destinataire)const;
        std::string selectionnerExpediteur(std::string id_user);

        void LireContenuMessage(std::string id_user);

        bool verifierMDPdansBD(std::string id_, std::string mot_de_passe);

    private:
        sqlite3 *m_db;
        DataConnxion m_data;

        Data_Message m_data_msg;
        std::vector<Data_Message> m_liste_messages; //pour stocker les messages dans un vecteur

};

void afficherLigneEmploye(sqlite3_stmt *stmt);
void dessinnerLignes();
void dessinnerLignesMSG();
std::string getPassword();
std::string affichageMessageRecusNonLus(std::string text);
void dessinerQRCode(canvas_ity::canvas& cv, std::string texte, float x, float y, float tailleCarre);
std::vector<unsigned char> chargerPolice(std::string chemin);
// void dessinerLogo(canvas_ity::canvas& cv, std::string chemin, float x, float y, float cibleL, float cibleH);

#endif