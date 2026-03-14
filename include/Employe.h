#ifndef DEF_EMPLOYE
#define DEF_EMPLOYE

#include <string>

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

};

//Ici ce sont des fonction hors de la classe pour des fonctionnalites specifiques.

std::string crypterMotDePasse(const std::string& mot_de_passe);  //Crypter le mot de passe
bool estUnEmailValide(const std::string& mail);                  //Verifier le mail
bool estUnNomValide(const std::string& nom);                     // structure pour la validation des donnees
struct ResultatValidation;
ResultatValidation verifierMotDePasse(const std::string& mdp);   //Verification du mot de passe

#endif