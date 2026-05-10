#include "Employe.h"
#include <string>
#include <iostream>
#include <limits>
#include <sstream>
#include <regex>
#include <sodium.h>

using namespace std;

Employe::Employe(std::string nom, std::string prenom,
                std::string dateAdhesion_e, std::string situaMatrimonial, std::string poste, std::string typeContrat,
                std::string Mot_de_passe, std::string email, std::string categorie, int age, float salaire, std::string date_naissance)
                {
                    m_nom = nom;
                    m_prenom = prenom;
                    m_date_adhesion_entreprise = dateAdhesion_e;
                    m_situation_matrimonial = situaMatrimonial;
                    m_poste = poste;
                    m_type_contrat = typeContrat;
                    m_mot_de_passe = Mot_de_passe;
                    m_email = email;
                    m_categorie = categorie;
                    m_age = age;
                    m_salaire = salaire;
                    m_actif = true;
                    m_date_naissance = date_naissance;
                }

Employe::Employe()
                {

                }

        bool estUnNomValide(const string& nom)
        {
            if(nom.length() < 2 || nom.length() > 12)
            {
                return false;
            }

            regex pattern("^[a-zA-ZÀ-ÿ][ 'a-zA-ZÀ-ÿ-]*$");

            return regex_match(nom, pattern);
        }


        std::string Employe:: getNom() 
        { 
             cout << "Entrez le nom : ";
             cin >> m_nom;

             while(!estUnNomValide(m_nom))
             {
                cout << "Nom invalide (Caracteres non autorises ou trop court) !" << endl;
                cout << "Entrez le Nom : ";
                cin >> m_nom;
             }

            return m_nom;
        }

        std::string Employe:: getPrenom() 
        {
             cout << "Entrez le Prenom : ";
             cin >> m_prenom;

             while(!estUnNomValide(m_prenom))
             {
                cout << "Prenom invalide (Caracteres non autorises ou trop court) !" << endl;
                cout << "Entrez le Prenom : ";
                cin >> m_prenom;
             }

            return m_prenom;
        }


        std::string Employe:: getDate_adhesion() 
        {
            int jour, mois, annee;
            //jour
            std::cout << "---Date Adhesion " << "(Format : jj.mm.aaaa)" << std::endl;
             cout << "> Entrez le jour (1 - 30) : ";
             while(!(cin >> jour) || (jour < 1 || jour > 31))
             {
                cout << "> Entrez le jour (1 - 30) : ";
                cin.clear();
                cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
             }

             //mois
             cout << "> Entrez le mois (1 - 12) : ";
             while(!(cin >> mois) || (mois < 1 || mois > 12))
             {
                cout << "> Entrez le mois (1 - 12) : ";
                cin.clear();
                cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
             }

             //annee
             cout << "> Entrez l'annee (1940 - 2100) : ";
             while(!(cin >> annee) || (annee < 1940 || annee > 2100))
             {
                cout << "> Entrez l'annee (1940 - 2100) : ";
                cin.clear();
                cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
             }

            stringstream jr, ms, an;
            jr << jour;
            ms << mois;
            an << annee;
            
            m_date_adhesion_entreprise = an.str() + "-" + ms.str() + "-" +jr.str();

            return m_date_adhesion_entreprise;
        }

        std::string Employe::setDateNaissance()
        {
            int jour, mois, annee;
            //jour
            std::cout << "---Date de naissance " << "(Format : jj.mm.aaaa)" << std::endl;
             cout << "> Entrez le jour (1 - 30) : ";
             while(!(cin >> jour) || (jour < 1 || jour > 31))
             {
                cout << "> Entrez le jour (1 - 30) : ";
                cin.clear();
                cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
             }

             //mois
             cout << "> Entrez le mois (1 - 12) : ";
             while(!(cin >> mois) || (mois < 1 || mois > 12))
             {
                cout << "> Entrez le mois (1 - 12) : ";
                cin.clear();
                cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
             }

             //annee
             cout << "> Entrez l'annee (1940 - 2100) : ";
             while(!(cin >> annee) || (annee < 1940 || annee > 2100))
             {
                cout << "> Entrez l'annee (1940 - 2100) : ";
                cin.clear();
                cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
             }

            stringstream jr, ms, an;
            jr << jour;
            ms << mois;
            an << annee;
            
            m_date_naissance = an.str() + "-" + ms.str() + "-" +jr.str();

            return m_date_naissance;
        }

        std::string Employe:: getSituation_matrimonial() 
        {
            bool bon = false;
            do
            {
                int choix;
                cout << "> Choissisez votre situation : " << endl;
                cout << "1. Celibataire" << endl;
                cout << "2. Marie(e)" << endl;
                cout << "3. Divorce" << endl;
                cout << "> ";

                //gestion des erreurs de saisie pour le choix de la situation matrimonial
                while(!(std::cin >> choix) || (choix < 1 || choix > 3))
                            {
                                std::cout << "> choix (1-3): ";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                
                switch (choix)
                {
                    case  1: m_situation_matrimonial = "Celibataire"; bon = false; break;
                    case  2: m_situation_matrimonial = "Marie(e)"; bon = false; break;
                    case  3: m_situation_matrimonial = "Divorce(e)"; bon = false; break;
                    
                    
                    default: cout << "Valeur pas adequate" << endl; bon = true;
                    break;
                }
            }while (bon == true);

            return m_situation_matrimonial;
        }


        std::string Employe:: getPoste() 
        {
            bool bon = false;
            do
            {
                int choix;
                cout << "> Choissisez votre Poste : " << endl;
                cout << "1. Ingenieur" << endl;
                cout << "2. Medecin" << endl;
                cout << "3. Directeur" << endl;
                cout << "4. Financier" << endl;
                cout << "5. Informaticien" << endl;
                cout << "6. Commercial" << endl;
                cout << "7. Gestionnaire" << endl;
                cout << "8. Agent de sec" << endl;
                cout << "9. Garagiste" << endl;
                cout << "> ";
                
                 //gestion des erreurs de saisie pour le choix du poste
                            while(!(std::cin >> choix) || (choix < 1 || choix > 9))
                            {
                                std::cout << "> choix (1-9): ";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                 
                
                switch (choix)
                {
                    case  1: m_poste = "Ingenieur"; bon = false; break;
                    case  2: m_poste = "Medecin"; bon = false; break;
                    case  3: m_poste = "Directeur"; bon = false; break;
                    case  4: m_poste = "Financier"; bon = false; break;
                    case  5: m_poste = "Informaticien"; bon = false; break;
                    case  6: m_poste = "Commercial"; bon = false; break;
                    case  7: m_poste = "Gestionnaire"; bon = false; break;
                    case  8: m_poste = "Agent de sec"; bon = false; break;
                    case  9: m_poste = "Garagiste"; bon = false; break;
                    
                    
                    default: cout << "Valeur pas adequate" << endl; bon = true;
                    break;
                }
            }while (bon == true);

            return m_poste;
        }


        std::string Employe:: getType_contrat()  
        {
            bool bon = false;
            do
            {
                int choix;
                cout << "> Choissisez le type de contrat : " << endl;
                cout << "1. CDD (Contrat a Duree Determine)" << endl;
                cout << "2. CDI (Contrat a Duree Indetermine)" << endl;
                cout << "> " ;

                //gestion des erreurs de saisie pour le choix du type de contrat
                            while(!(std::cin >> choix) || (choix < 1 || choix > 2))
                            {
                                std::cout << "> choix (1-2): ";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                
                switch (choix)
                {
                    case  1: m_type_contrat = "CDD"; bon = false; break;
                    case  2: m_type_contrat = "CDI"; bon = false; break;
                    
                    
                    default: cout << "Valeur pas adequate" << endl; bon = true;
                }
            }while (bon == true);

            return m_type_contrat;
        }
        
        

        double Employe::getSalaire() 
        {
                
            cout << "> Entrez le salaire : ";
            while (!(cin >> m_salaire) || (m_salaire < 1000 || m_salaire > 20000))
            {
                cout << "> Entrez le salaire (1000-20000) : ";
                cin.clear();
                cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
            }

            if (m_salaire <= 5000) m_categorie = "A";
            if (5000 < m_salaire && m_salaire <= 15000) m_categorie = "B";
            if (15000 < m_salaire && m_salaire <= 20000) m_categorie = "C";
             return m_salaire;
        }


        bool Employe::getActif() const {return m_actif;}


       struct ResultatValidation
        {
            bool valide;
            std::vector<std::string> erreurs;
        };

        //On valide le mot de passe

        ResultatValidation verifierMotDePasse(string& mdp)
        {
            ResultatValidation resultat;
            resultat.valide = true;

            //contraine taille
            if(mdp.length() > 12)
            {
                resultat.erreurs.push_back("Le Mot de Passe doit contenir au mois 12 caracteres !");
                resultat.valide = true;
            }

            bool aMajuscule = false;
            bool aMinuscule = false;
            bool aChiffre = false;
            bool aSpecial = false;

            //Les caracteres speciaux autorises
            std::string specicaux = "!@#$%^&*()-_=+[]{}|;:,.<>?";

            for(char c : mdp)
            {
                if(isupper(static_cast<unsigned char>(c))) 
                    aMajuscule = true;
                else if(islower(static_cast<unsigned char>(c))) 
                    aMajuscule = true;
                else if (isdigit(static_cast<unsigned char>(c))) 
                    aChiffre = true;
                else if (specicaux.find(c) != string::npos)
                    aSpecial = true;
            }

            if(!aMajuscule)
            {
                resultat.erreurs.push_back("Manque d'une Majuscule.");
                resultat.valide = false;
            }
            if(!aMinuscule)
            {
                resultat.erreurs.push_back("Manque d'une minuscule.");
                resultat.valide = false;
            }
            if(!aChiffre)
            {
                resultat.erreurs.push_back("Manque d'un chiffre.");
                resultat.valide = false;
            }
            if(!aSpecial)
            {
                resultat.erreurs.push_back("Manque d'une Caractere Speciale.");
                resultat.valide = false;
            }

            return resultat;
        }

        //Fonction de chiffrement du mot de passe
        std::string crypterMotDePasse(const std::string& password)
        {
            char hashed_password[crypto_pwhash_STRBYTES];
            if (crypto_pwhash_str(hashed_password, password.c_str(), 
                                  password.length(),
                                  crypto_pwhash_OPSLIMIT_INTERACTIVE,
                                  crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0)
                                  {
                                    //a definir
                                    throw std::runtime_error("Erreur lors du chiffrement du mot de passe !");
                                  }
                                  else
                                  {
                                    // cout << "Le mot de passe chiffree : " << hashed_password << endl;
                                  }
            return std::string(hashed_password); 
        }

        //Recuperation du Mot de passe.

        std::string Employe::getMot_de_passe() 
         {
            cout << "> Entrez le Mot de passe : ";
            string password;
             cin >> password;
             ResultatValidation res = verifierMotDePasse(password);

             while(res.valide)
             {
                for(const auto& err : res.erreurs) cout << "- " << err << endl;
                cout << "> Entrez le Mot de passe : ";
                string password;
                cin >> password;
                res = verifierMotDePasse(password);
             }


             std::string password1 = crypterMotDePasse(password);
            // return m_mot_de_passe;
            return password1;  //Retourne le mot de passe chiffré, mais je verifie d'abord dans la BD
         }


        bool estUnEmailValide(const std::string& mail)
        {
            //Verification de la presence d'un prefixe, du @, d'un domaine et d'une extension de 2 lettres min
            const std::regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");

            return std::regex_match(mail, pattern);
        }

        std::string Employe::getEmail() 
        {
            cout << "> Entrez le mail : ";
            cin >> m_email;
            while((!estUnEmailValide(m_email)))
            {
                cout << "> Entrez un mail valide : ";
                 cin >> m_email;
            }

            return m_email;
        }

        std::string Employe::getCategorie() const 
        {

            return m_categorie;
        }

        int Employe::activer()
        {
            m_etat = 1;
            return m_etat;
        }

        int Employe::desactiver()
        {
            m_etat = 0;
            return m_etat;
        }