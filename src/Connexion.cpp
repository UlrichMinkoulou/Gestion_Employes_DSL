#include "Connexion.h"
#include "Employe.h"
#include "Data_base.h"
#include "Administrateur.h"
#include "Admin_Data_Base.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <limits>


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

void pageAcceuil()
{
    int option;
    bool resultat = false;
    do
    {
            /* code */
            
            
            dessinnerL();
            std::cout << "| " << ANSI_RED << ANSI_BOLD << std::left << std::setw(28) << "DRONE.Solutions ltd." << ANSI_RESET << ANSI_BOLD
            << "| " << std::setw(18) << "1. PRESENTATION"
            << "| " << std::setw(23) << "2. PRODUITS & SERVICES"
            << "| " << std::setw(18) << "3. PROJETS"        
            << "| " << std::setw(18) << "4. BOUTIQUE"
            << "| " << std::setw(18) << "5. CONTACTS"
            << "| " << std::setw(18) << "6. CONNEXION"<< "|" << ANSI_RESET << std::endl;
        dessinnerL();
        std::cout << std::endl << std::endl;

        std::cout << "> BIENVENU !! " << std::endl;
        std::cout << "> Merci de choisir une Option : ";
        std::cin >> option;

        switch (option)
        {
            case 1: clearCLI(); presentation();  break;
            case 2: clearCLI(); produits_services(); break;
            case 3: clearCLI(); projets(); break;
            case 4: clearCLI(); boutique();  break;
            case 5: clearCLI(); contats();break; 
            case 6: clearCLI(); resultat = login();  break; //clearCLI(); a remettre plus tard pour eviter les bugs d'affichage
            
            
            default: resultat = true;
            break;
        }
    } while (resultat == false);

}

bool login()
{
   bool resultat = false;
   std::string identifiant_;
   std::cout << std::endl << std::endl;
    dessinerLdeux();
    std::cout << "| " << ANSI_RED << ANSI_BOLD <<  std::left << std::setw(28) << "DRONE.Solutions ltd." << ANSI_RESET << ANSI_BOLD
              << "| " << std::setw(23) << "CONNEXION"<< ANSI_RESET << "|" << std::endl;
    dessinerLdeux();

    std::cout << "\n> Identifiant : ";
     std::cin >> identifiant_;
     std::cin.ignore(1000, '\n');

    if(identifiant_[0] == 'A') 
        resultat =  viewAdmin(identifiant_);
    else if (identifiant_[0] == 'E')
        resultat = viewUser(identifiant_);
    else
        std::cout << ANSI_BOLD << ANSI_RED << "\n[FAIL]" << ANSI_RESET << "FAIL d'Identifiant." << std::endl;

    return false;

}

bool viewUser(std::string IdUser)
{
    DataBase User("entreprise_.db");
    User.chargerCacheMSG();
    bool resultat = true;

    if ( User.connexionEmploye(IdUser) == true)
    {
            do
            {
                std::cout << std::endl << std::endl;
                dessinerLdeux();
                std::cout << "| " << ANSI_RED << ANSI_BOLD << std::left << std::setw(28) << "DRONE.Solutions ltd." << ANSI_RESET << ANSI_BOLD
                << "| " << std::setw(23) << User.getData().name<< ANSI_RESET << "|" << std::endl;
                dessinerLdeux();
                User.afficherUser(User.getData().id_);
            
            
                std::cout << "\n\n1. Modifier mes Informations de connexion" << std::endl;
                std::cout << "2. Messages" << std::endl;
                std::cout << "3. Imprimer Fiche de paie" << std::endl;
                std::cout << "4. Se Deconnecter" << std::endl << std::endl;
                int choix;
                std::cout << "> ";
                //gestion des FAILs de saisie pour le choix du menu user
                while(!(std::cin >> choix) || (choix < 1 || choix > 4))
                {
                    std::cout << "> choix (1-4): ";
                    std::cin.clear();   
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                switch (choix)
                {
                    case 1: User.mofifierInfoConnexionEmploye(User.getData().id_); break;
                    case 2: messages(User.getData().id_); clearCLI(); break;
                    case 3: 
                    {
                        User.imprimer_fiche_paie(User.getData().id_); break;
                    }
                    case 4: resultat = false; break;
                    
                    default:
                        break;
                }

        }while (resultat == true);
    
    }




    return resultat;
}


bool viewAdmin(std::string IdAdmin)
{
    Admin_db admin_user("dataBase_admin.db");
    bool rester_Menu_Admin = false;

    if ( admin_user.connexionAdmin(IdAdmin) == true)
    {
        std::cout << std::endl << std::endl;
        dessinerLdeux();
        std::cout << "| "<< ANSI_RED << ANSI_BOLD << std::left << std::setw(28) << "DRONE.SOLUTIIONS ltd."<< ANSI_RESET << ANSI_BOLD
        << "| " << std::setw(23) << admin_user.getAdminData().name<< ANSI_RESET << "|" << std::endl;
        dessinerLdeux();
        admin_user.afficherUserAdmin(admin_user.getAdminData().id_);
        

        bool rester_Menu_employe = true;

        do
        {
            int choix;
            std::cout << "\n\n1. Options Employes" << std::endl;
            std::cout << "2. Message" << std::endl;
            std::cout << "3. Modifier mes INFOs de CONNEXION" << std::endl;
            std::cout << "4. Se Deconnecter" << std::endl << std::endl;
            std::cout << "> ";

            //gestion des FAILs de saisie pour le choix du menu admin
                            while(!(std::cin >> choix) || (choix < 1 || choix > 4))
                            {
                                std::cout << "> choix (1-4): ";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }

            DataBase bd_user("entreprise_.db");

            switch (choix)
            {
                case 1: 
                {

                    int choice; std::string id_;
                    bd_user.chargerCache();

                    do
                        {
        std::cout << "\n=======================================" << ANSI_BLUE << ANSI_BOLD << "OPTIONS EMPLOYES" << ANSI_RESET<< std::endl;
                            // std::cout << std::endl << "---------OPTIONS EMPLOYES--------- " << std::endl;
                            std::cout << "\n\n1. Afficher les Employes" << std::endl;
                            std::cout << "2. Ajouter un Employes" << std::endl;
                            std::cout << "3. Changer Infos Employes" << std::endl;
                            std::cout << "4. Activer/Desactiver Employes" << std::endl;
                            std::cout << "5. Imprimer fiche de paie" << std::endl;
                            std::cout << "6. Recherche d'un Employe" << std::endl;
                            std::cout << "7. Retour Menu Admin" << std::endl << std::endl;
                            std::cout << "> ";  

                            //gestion des FAILs de saisie pour le choix du menu employe
                            while(!(std::cin >> choice) || (choice < 1 || choice > 7))
                            {
                                std::cout << "> choix (1-7): ";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            
                            switch (choice)
                            {
                            case 1: 
                            {
                                // std::cout << "Depuis la BD" << std::endl;
                                // bd_user.afficherEmploye(); 
                                // std::cout << "Depuis la la RAM" << std::endl;
                                std::vector<EmployeData> vect = bd_user.getCache();
                                // const Empl data
                                bd_user.afficherEmploye_caching(vect);
                                break;
                            }
                            case 2: bd_user.ajouterEmploye(); bd_user.chargerCache(); break;
                            case 3: bd_user.changerInfoEmploye(); bd_user.chargerCache();break;
                            case 4: bd_user.activerdesactiverEmployer(); bd_user.chargerCache();break;
                            case 5: 
                            {
                                std::cout << std::endl << std::endl << "--- Imprimer fiche de paie ---" << std::endl;
                                std::cout << "Entrez l'identifiant de l'employe : ";
                                 std::cin >> id_;
                                if(bd_user.verif_if(id_))
                                {
                                    // bd_user.imprimer_fiche_paie(id_);  
                                    bd_user.imprimer_fiche_paie_caching(id_);
                                }
                                else
                                {
                                    std::cout << ANSI_BOLD << ANSI_RED <<"[FAIL]" << ANSI_RESET << "Identifiant incorrect ou employe inactif." << std::endl;
                                }
                                break;
                            }
                            case 6: std::cout << std::endl << std::endl << "--- Recherche employe ---" << std::endl;bd_user.rechercherUnEmploye(); break;
                            case 7: rester_Menu_employe = false; break;

                            default: rester_Menu_employe = true;
                                break;
                            }
                            
                        }while(rester_Menu_employe == true);
                    
                    rester_Menu_Admin = false; 
                    break;
                }
                    
                case 2: messages(admin_user.getAdminData().id_); rester_Menu_Admin = false; break;
                case 3: admin_user.modifierAdmin(admin_user.getAdminData().id_); rester_Menu_Admin = false; break;
                case 4: rester_Menu_Admin = true; break;
            
                default: rester_Menu_Admin = true; break;
            }

        } while (rester_Menu_Admin == false);
        
    }

    return rester_Menu_Admin;
}

void options_EMployes(int idAdmin)
{

}

void presentation()
{
   std::cout << std::endl << std::endl;
    dessinerLdeux();
    std::cout << "| " << ANSI_RED << ANSI_BOLD  << std::left << std::setw(28) << "DRONE.SOLUTIONS ltd." << ANSI_RESET << ANSI_BOLD
              << "| " << std::setw(23) << "PRESENTATION"<< ANSI_RESET << "|" << std::endl;
    dessinerLdeux();

    std::cout << std::endl << "DRONE.Solutions est une startup Camerounaise qui conçoit des drones et déploie des services via les technologies de drones \n";
    std::cout << "l’informatique, l’électronique, la robotique, les conceptions FPGA, l’automatique et l’intelligence artificielle.\n\n";
    std::cout << "Ville : Douala-Cameroun, Rue: Ange-Raphael\n\n";

    int choix;

    std::cout << "";
}

void contats()
{
   std::cout << std::endl << std::endl;
    dessinerLdeux();
    std::cout << "| " << ANSI_RED << ANSI_BOLD  << std::left << std::setw(28) << "DRONE.SOLUTIONS ltd." << ANSI_RESET << ANSI_BOLD
              << "| " << std::setw(23) << "CONTACTS"<< ANSI_RESET << "|" << std::endl;
    dessinerLdeux();

    std::cout << std::endl << "Contactez nous : dronesolutionscmr@gmail.com\n";
    std::cout << std::endl << "               : +237 657 670 004\n";
    std::cout << std::endl << "               : +33 644 667 950\n\n";
                 std::cout << "         Ville : Douala Cameroun, Ange Raphael\n\n";

    int choix;

    std::cout << "";
}

void produits_services()
{
   std::cout << std::endl << std::endl;
    dessinerLdeux();
    std::cout << "| " << ANSI_RED << ANSI_BOLD  << std::left << std::setw(28) << "DRONE.SOLUTIONS ltd." << ANSI_RESET << ANSI_BOLD
              << "| " << std::setw(23) << "PRODUITS ET SERVICES"<< ANSI_RESET << "|" << std::endl;
    dessinerLdeux();

    std::cout << "\n-> Drones " << std::endl;
    std::cout << "-> Logiciel d'InterfacAGE Systemes :  " << std::endl;
    std::cout << "-> Systemes Embarques " << std::endl;
    std::cout << "-> Robotique " << std::endl;
    std::cout << "-> Conceptions FPGA " << std::endl;
    std::cout << "-> Cryptographie pour Embarque \n" << std::endl;
}

void boutique()
{
   std::cout << std::endl << std::endl;
    dessinerLdeux();
    std::cout << "| " << ANSI_RED << ANSI_BOLD  << std::left << std::setw(28) << "DRONE.SOLUTIONS ltd." << ANSI_RESET << ANSI_BOLD
              << "| " << std::setw(23) << "BOUTIQUE"<< ANSI_RESET << "|" << std::endl;
    dessinerLdeux();
    std::cout<< "\nInformations non Dispanible !\n\n" << std::endl;
}

void projets()
{
   std::cout << std::endl << std::endl;
    dessinerLdeux();
    std::cout << "| " << ANSI_RED << ANSI_BOLD  << std::left << std::setw(28) << "DRONE.SOLUTIONS ltd." << ANSI_RESET << ANSI_BOLD
              << "| " << std::setw(23) << "PROJETS"<< ANSI_RESET << "|" << std::endl;
    dessinerLdeux(); 
    std::cout<< "\nInformations non Dispanible !\n\n" << std::endl;
}


void messages(std::string id)
{
  bool condition = false;
  
  do
    {
        /* code */
        
        std::cout << "\n=======================================" << ANSI_BLUE <<  ANSI_BOLD << "MESSAGERIE" << ANSI_RESET<< std::endl;
            DataBase user("entreprise_.db");
            user.message_RNL(id);
            std::cout << std::endl;
            
        std::cout << "1. Discussion" << std::endl;
        // std::cout << "2. Messages Recus(non lus)" << std::endl;
        std::cout << "2. Messages Recus(lus)" << std::endl;
        std::cout << "3. Nouveau Message" << std::endl;
        std::cout << "4. Messages Envoyes" << std::endl;
        std::cout << "5. Retour Menu" << std::endl << std::endl;
        int choix;
        std::cout << "> ";
            //gestion des FAILs de saisie pour le choix du menu de messagerie
                            while(!(std::cin >> choix) || (choix < 1 || choix > 6))
                            {
                                std::cout << "> choix (1-6): ";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }

                            user.chargerCacheMSG();
        switch (choix)
        {
            // case 2: { user.afficher_MSG_non_lus(id); break;} 

            case 2: { 
                //std::cout << "BD" << std::endl;
                // user.afficher_MSG_lus(id); 
                // std::cout << "RAM : j'affiche tous les messages" << std::endl;
                // user.afficherTousMessages();  
                // std::cout << "RAM : j'affiche tous les messages du destinataire " << id << std::endl;
                user.afficher_MSG_lus_caching(id);

                break;}

            case 3: 
            {
                std::string destinataire, contenu, objet;


                std::cout << "-----Envoyer un Message-----" << std::endl << std::endl;
                std::cout << "Destinataire: ";
                std::cin >> destinataire;
                
                std::cout << "Objet : ";
                std::cin.ignore(1000, '\n');
                std::getline(std::cin, objet);

                std::cout << "Message : ";
                std::getline(std::cin, contenu);

                user.envoyer_MSG(destinataire, id, contenu, objet); break;
            }

            case 4: {
                //  user.afficher_MSG_envoyes(id); 
                //  std::cout << "RAM" << std::endl;
                 user.afficher_MSG_envoyes_caching(id);
                 break;
                    }

            case 1: 
            {
                    std::string id_expediteur = user.selectionnerExpediteur(id);
                    id_expediteur = user.afficherDiscussion(id, id_expediteur); 
                    bool condition_discussion = false;
                    // user.LireContenuMessage(id_destinataire);

                    do
                    {
                            std::cout << "\n1. Ajouter une reponse" << std::endl;
                            std::cout << "2. Marquer comme lu" << std::endl;
                            std::cout << "3. Retour au menu" << std::endl;
                            int choix_discussion; 
                            std::cout << std::endl << "> ";

                            //gestion des FAILs de saisie pour le choix de la discussion
                                while(!(std::cin >> choix_discussion) || (choix_discussion < 1 || choix_discussion > 3))
                                {
                                    std::cout << "> choix (1-3): ";
                                    std::cin.clear();
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                }
                            
                        switch (choix_discussion)
                        {
                            case 1:
                                {
                                    std::string contenu, objet;
                                    std::cout << "Message : ";
                                    std::cin.ignore(1000, '\n');
                                    std::getline(std::cin, contenu);

                                    std::cout << "Objet : ";
                                    std::getline(std::cin, objet);
                                    
                                    user.envoyer_MSG(id_expediteur, id, contenu, objet); 
                                    DataBase message("entreprise_.db");
                                    message.afficherDiscussion(id, id_expediteur); 
                                    break;
                                }
                                
                            case 2: user.lire_MSG_recus(id_expediteur); break;
                            case 3: condition_discussion = true; clearCLI(); break;
                        }
                    }while (condition_discussion == false);
              break;  
            } 
            
            case 5: condition = true; break;
            
            default: condition = false; break;

        }
    } while (condition == false);

}
    void discussion(std::string id_user)
        {
            DataBase db("entreprise_.db");
            std::vector<Data_Message> liste = db.recupererMessages(id_user);
            for(Data_Message& msg : liste)
            {
                std::cout << "[" << msg.getIDexpediteur() << "] : " << msg.getContenu() << std::endl;
                std::cout << "Date/Heure: " << msg.getDatetime() << std::endl;
                std::cout << "-----------------------------" << std::endl;
            }
        }
// void messages(std::string id)
// {
    
// }


void dessinnerL()
{

    std::cout << "+"
         << std::setfill('-')
         << std::setw(30)
         << "+" 
         << std::setw(20)
         << "+" 
         << std::setw(25)         
         << "+"
         << std::setw(20)         
         << "+"
         << std::setw(20)
         << "+"        
         << std::setw(20)
         << "+" 
         << std::setw(20)         
         << "+"
         << std::setfill(' ')
         << std::endl;
    
}

void dessinerLdeux()
{
   std::cout << "+"
         << std::setfill('-')
         << std::setw(30)  
         << "+" 
         << std::setw(25)         
         << "+"
         << std::setfill(' ')
         << std::endl;
}
