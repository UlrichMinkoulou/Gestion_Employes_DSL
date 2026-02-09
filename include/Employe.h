#ifndef DEF_EMPLOYE
#define DEF_EMPLOYE

#include <string>

class Employe
{
    protected:
        std::string m_nom;
        std::string m_prenom;
        std::string m_date_entre;
        std::string m_date_sortie;
        std::string m_date_adhesion_entreprise;
        std::string m_situation_matrimonial;
        std::string m_poste;
        std::string m_type_contrat;
        int m_age;
        bool m_actif;
        double m_salaire;
        char m_mdp;
        std::string m_mot_de_passe;
        std::string m_email;
        std::string m_categorie;
        std::string m_identifiant_Employe;

    public:
        Employe(std::string nom, std::string prenom,
                std::string dateAdhesion_e, std::string situaMatrimonial, std::string poste, std::string typeContrat,
                std::string Mot_de_passe, std::string email, std::string categorie, int age, float salaire);
        Employe();
        void afficherInformations()const;
        void imprimerFichePaie()const;
        std::string getNom() ;
        std::string getPrenom() ;
        std::string getDate_entree() const;
        std::string getDate_sortie() const;
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

};

bool estUnEmailValide(const std::string& mail);
bool estUnNomValide(const std::string& nom);
struct ResultatValidation;
ResultatValidation verifierMotDePasse(const std::string& mdp);

#endif