#ifndef DEF_EMPLOYE
#define DEF_EMPLOYE

#include <string>

std::string crypterMotDePasse(const std::string& mot_de_passe);  //Crypter le mot de passe

//opti #2 optimisation par caching (BD -> conteneur -> lecture directe)
struct EmployeData{
        std::string m_nom;
        std::string m_prenom;
        std::string m_date_adhesion_entreprise;
        std::string m_date_naissance;
        std::string m_situation_matrimonial;
        std::string m_poste;
        std::string m_type_contrat;
        // int m_age;
        // bool m_actif;
        int m_etat;
        double m_salaire;
        // char m_mdp;
        std::string m_mot_de_passe;
        std::string m_email;
        std::string m_categorie;
        std::string m_identifiant_Employe;
};

 //Ce code nous permet de creer une classe qui est une instance d'un employe.
class Employe
{
    protected:
        std::string m_nom;
        std::string m_prenom;
        std::string m_date_adhesion_entreprise;
        std::string m_date_naissance;
        std::string m_situation_matrimonial;
        std::string m_poste;
        std::string m_type_contrat;
        int m_age;
        bool m_actif;
        int m_etat;
        double m_salaire;
        char m_mdp;
        std::string m_mot_de_passe;
        std::string m_email;
        std::string m_categorie;
        std::string m_identifiant_Employe;

    public:
        Employe(std::string nom, std::string prenom,
                std::string dateAdhesion_e, std::string situaMatrimonial, std::string poste, std::string typeContrat,
                std::string Mot_de_passe, std::string email, std::string categorie, int age, float salaire, std::string date_naissance);
        Employe();
        void afficherInformations()const;
        void imprimerFichePaie()const;

        //Normalement ce sont des seteur... je devais ecrire setNom..., mais peut etre plus tard.
        std::string getNom() ;
        std::string getPrenom() ;
        std::string getDate_adhesion();
        std::string getSituation_matrimonial();
        std::string getPoste();
        std::string getType_contrat();
        int getAge();
        double getSalaire();
        bool getActif() const;
        std::string getMot_de_passe();
        std::string getEmail() ;
        std::string getCategorie() const;
        std::string getId();
        int activer();
        int desactiver();

        std::string setDateNaissance();

        //Ici je declare les bons setteurs
        std::string setNom(std::string nom){m_nom = nom; return m_nom;}
        std::string setPrenom(std::string prenom){m_prenom = prenom; return m_prenom;}
        std::string setDateAdhesion(std::string date_adhesion){m_date_adhesion_entreprise = date_adhesion; return m_date_adhesion_entreprise;}
        std::string setSituationMatrimonial(std::string situation_matrimonial){m_situation_matrimonial = situation_matrimonial; return m_situation_matrimonial;}
        std::string setPoste(std::string poste){m_poste = poste; return m_poste;}
        std::string setTypeContrat(std::string type_contrat){m_type_contrat = type_contrat; return m_type_contrat;}
        int setAge(int age){m_age = age; return m_age;}
        double setSalaire(double salaire){m_salaire = salaire; return m_salaire;}
        std::string setMotDePasse(std::string mot_de_passe){m_mot_de_passe = mot_de_passe; return m_mot_de_passe;}
        std::string setEmail(std::string email){m_email = email; return m_email;}
        std::string setCategorie(std::string categorie){m_categorie = categorie; return m_categorie;}
        std::string setId(std::string id){m_identifiant_Employe = id; return m_identifiant_Employe;}

        //getteurs speciaux
        std::string getNom_(){ return m_nom;}
        std::string getPrenom_(){ return m_prenom;}
        std::string getDateAdhesion_(){ return m_date_adhesion_entreprise;}
        std::string getSituationMatrimonial_(){ return m_situation_matrimonial;}
        std::string getPoste_(){ return m_poste;}
        std::string getTypeContrat_(){ return m_type_contrat;}
        int getAge_(){ return m_age;}
        double getSalaire_(){ return m_salaire;}
        std::string getMotDePasse_(){ return m_mot_de_passe = crypterMotDePasse(m_mot_de_passe);}
        std::string getEmail_(){ return m_email;}
        std::string getCategorie_(){ return m_categorie;}
        std::string getdateNaissance_(){ return m_date_naissance;}
        std::string setDateNaissance_(std::string date_naissance){m_date_naissance = date_naissance; return m_date_naissance;}

        int setEtat_(int etat){ m_etat = etat; return m_etat;}


};

//Ici ce sont des fonction hors de la classe pour des fonctionnalites specifiques.

bool estUnEmailValide(const std::string& mail);                  //Verifier le mail
bool estUnNomValide(const std::string& nom);                     // structure pour la validation des donnees
struct ResultatValidation;
ResultatValidation verifierMotDePasse(const std::string& mdp);   //Verification du mot de passe

#endif