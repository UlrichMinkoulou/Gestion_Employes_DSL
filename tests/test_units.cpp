#include <cassert>
#include "../include/Data_base.h"
#include "../include/Employe.h"
#include <gtest/gtest.h>
#include <vector>


//La Fixure permettant de partager la logique d'init
class DataBaseTest : public :: testing::Test {
    protected:
        DataBase* db;

        void SetUp() override
        {
            db = new DataBase("entreprise_.db");
        }

        void TearDown() override
        {
            delete db;
            std::remove("entreprise_.db"); // Nettoyer la base de données après chaque test
        }
};

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






void testEnvoyerMSG()
{
    DataBase db("entreprise_.db");
    db.envoyer_MSG("ADSL0001", "EDSL0001", "2nd message", "Test2");
    db.envoyer_MSG("ADSL0001", "EDSL0001", "Test de message4", "Test1");
    db.envoyer_MSG("ADSL0001", "EDSL0001", "Test de messag23", "Test2");
    db.envoyer_MSG("ADSL0001", "EDSL0001", "Test de message2", "Test3");
    db.envoyer_MSG("ADSL0001", "EDSL0001", "Test de message1", "Test4");
    db.envoyer_MSG("ADSL0001", "EDSL0001", "Test de message", "Test");
    // db.envoyer_MSG("ADSL0001", "EDSL0001", "Test de message", "Test");
    std::vector<Data_Message> messages = db.recupererMessages("ADSL0001");
    assert(!messages.empty());
    // assert(messages.back().getIDdestinataire() == "ADSL0001");
    assert(messages.back().getIDexpediteur() == "EDSL0001");
    assert(messages.back().getContenu() == "Test de message");
    // assert(messages.back().getObjet() == "Test");
    // assert(messages.back().getDatetime() == "")
}

void testAjouterEmploye()
    {
        DataBase emp("entreprise_.db");
        Employe e("UIj", "John","2023-12-05", "Celibataire", "Financier", "CDD", "Password123", "doe@dsl.cm", "B", 30, 4500.0, "1998-02-20");
        Employe f("Yop", "Mina","2023-12-05", "Marie", "Gestionnaire", "CDD", "Password123", "ym@dsl.cm", "B", 30, 4029.0, "2000-02-20");
        emp.ajouterEmployeTest(e);
        
        // emp.chargerCache();
        // std::vector<EmployeData> liste = emp.getCache();
        // assert(!liste.empty());
        // assert(liste.back().m_nom == "Doe");
        // assert(liste.back().m_prenom == "John");
        // assert(liste.back().m_identifiant_Employe == "EDSL0001");
        // assert(liste.back().m_etat == 1);
        // assert(liste.back().m_salaire == 4500.0);
        // assert(emp.verifierMDPdansBD("EDSL0001", "password123") == true);
        // // std::cout << "Email recupere: " << liste.back().m_email << std::endl;
        // assert(estUnEmailValide(liste.back().m_email)== true);
        // assert(liste.back().m_email == "doe@dsl.cm");
        // assert(liste.back().m_categorie == "A");
        // assert(liste.back().m_poste == "Financier");
        // assert(liste.back().m_type_contrat == "CDI");
        // assert(liste.back().m_date_adhesion_entreprise == "2023-01-15");
        // assert(liste.back().m_date_naissance == "1990-05-20");
        // assert(liste.back().m_situation_matrimonial == "Celibataire");
    }


void testRechercherEmploye()
{
    DataBase emp("entreprise_.db");
    Employe empt("Mkri", "sima","2023-12-05", "Celibataire", "Financier", "CDD", "Password123", "doe@dsl.cm", "B", 30, 4500.0, "1998-02-20");
    Employe empt1("Tois", "sima","2023-12-05", "Celibataire", "Financier", "CDD", "Password123", "doe@dsl.cm", "B", 30, 4500.0, "1998-02-20");
    Employe empt2("Tysu", "sima","2023-12-05", "Celibataire", "Financier", "CDD", "Password123", "doe@dsl.cm", "B", 30, 4500.0, "1998-02-20");
    Employe empt3("Pgtdso", "sima","2023-12-05", "Celibataire", "Financier", "CDD", "Password123", "doe@dsl.cm", "B", 30, 4500.0, "1998-02-20");
    emp.ajouterEmployeTest(empt);
    emp.ajouterEmployeTest(empt1);
    emp.ajouterEmployeTest(empt2);
    emp.ajouterEmployeTest(empt3);

    const EmployeData& e = emp.testRechercherUnEmploye("EDSL0002");

    if(e.m_nom != "Mkri")
    {
        std::cerr << ANSI_RED << ANSI_BOLD << "Erreur: Employe non trouve!" << ANSI_RESET << std::endl;
        std::cerr << "ID recherche: EDSL0002" << std::endl;
        std::cerr << "Nom trouve: " << e.m_nom << std::endl;
        assert(false); // Force l'echec du test
    }

    assert(!e.m_identifiant_Employe.empty());
    assert(e.m_nom == "Mkri");
    // std::cout << "Employe trouve: " << e.m_nom << " " << e.m_prenom << std::endl;

    emp.chargerCache();
    const std::vector<EmployeData>& listeemployes = emp.getCache();

    for(const auto& emp : listeemployes)
    {
        if(emp.m_nom == "Mkri")
        {
            assert(emp.m_prenom == "sima");
            assert(emp.m_identifiant_Employe == "EDSL0002");
            assert(emp.m_etat == 1);
            assert(emp.m_salaire == 4500.0);
            assert(emp.m_email == "doe@dsl.cm");
        }
    }

}

void testImprimerFichePaie()
{
    DataBase emp("entreprise_.db");
    emp.chargerCache();

    emp.testimprimer_fiche_paie_caching("EDSL0001");
}

void testVerifieridentifiant()
{
    DataBase emp("entreprise_.db");
    assert(emp.testverif_if("EDSL0001") == true);
    assert((emp.verifieridexist("NONEXISTANTID")) == false);
}

void testActiverDesactiverEmploye()
{
    DataBase emp("entreprise_.db");
    emp.testactiverdesactiverEmployer("EDSL0001", 2); // Desactiver
    const EmployeData& e = emp.testRechercherUnEmploye("EDSL0001");
    assert(e.m_etat == 0); // Verifier que l'employe est desactive

    emp.testactiverdesactiverEmployer("EDSL0001", 1); // Activer
    const EmployeData& e2 = emp.testRechercherUnEmploye("EDSL0001");
    assert(e2.m_etat == 1); // Verifier que l'employe est active
}

void testLireMessagesRecus()
{
    DataBase emp("entreprise_.db");
    emp.lire_MSG_recus("ADSL0001", 1);
    
    std::vector<Data_Message> messages = emp.recupererMessages("ADSL0001");

    emp.chargerCacheMSG();
    assert(!messages.empty());
    // assert(messages.back().getIDdestinataire() == "ADSL0001");
    assert(messages.back().getIDexpediteur() == "EDSL0001");
    assert(messages.back().getContenu() == "Test de message");
    // assert(messages.back().getObjet() == "Test");
}

void testPerformanceCaching()
{
    DataBase emp("entreprise_.db");

    auto start = std::chrono::high_resolution_clock::now();
    emp.afficherEmploye();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_no_cache = end - start;

    emp.chargerCache();

    start = std::chrono::high_resolution_clock::now();
    emp.afficherEmploye_caching(emp.getCache());
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_cache = end - start;

    std::cout << "\nTemps sans cache: " << duration_no_cache.count() << " ms" << std::endl;
    std::cout << "Temps avec cache: " << duration_cache.count() << " ms\n" << std::endl;

    assert(duration_cache.count() < duration_no_cache.count());
}

    //test performance pour le caching MSG
    void testPerformanceCachingMSG()
    {
        DataBase emp("entreprise_.db");

        
        auto start = std::chrono::high_resolution_clock::now();
        emp.chargerCacheMSG();
        emp.afficher_MSG_recus_caching("ADSL0001");
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "\nTemps d'affichage MSG avec caching : " << duration.count() << " microseconds" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        emp.afficher_MSG_non_lus("ADSL0001");
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "\nTemps d'affichage MSG sans caching : " << duration.count() << " microseconds\n" << std::endl;
        std::cout << std::endl;
    }

    void tesVerifierMDP()
    {
        DataBase mdp("entreprise_.db");
        assert(mdp.verifierMDPdansBD("EDSL0001", "Password123") == true);
        assert(mdp.verifierMDPdansBD("EDSL0001", "WrongPassword") == false);
    }

    void testRecupererMessages()
    {
        DataBase emp("entreprise_.db");
        std::vector<Data_Message> messages = emp.recupererMessages("ADSL0001");
        assert(!messages.empty());
        // assert(messages.back().getIDdestinataire() == "ADSL0001");
        assert(messages.back().getIDexpediteur() == "EDSL0001");
        assert(messages.back().getContenu() == "Test de message");
    }

    void tesChangerInfosEmploye()
    {
        DataBase emp("entreprise_.db");
        emp.testChangerInfosEmploye("EDSL0001", "Doki", "Med", "Medecin");

        const EmployeData& e = emp.testRechercherUnEmploye("EDSL0001");
        assert(e.m_nom == "Doki");
        assert(e.m_prenom == "Med");
        assert(e.m_poste == "Medecin");
    }

    void testConnexionEmploye()
    {
        DataBase emp("entreprise_.db");
        assert(emp.testConnexionEmploye("EDSL0001", "Password123") == true);
        assert(emp.testConnexionEmploye("EDSL01", "Password123") == false);
    }

    void testChargerCacheMSG()
    {

        DataBase msg("entreprise_.db");
        msg.envoyer_MSG("ADSL0001", "EDSL0001", "Test de message", "Test");
        msg.envoyer_MSG("ADSL0001", "EDSL0001", "Test de message", "Test");
        msg.envoyer_MSG("ADSL0001", "EDSL0001", "Test de message", "Test");

        msg.chargerCacheMSG();
        // std::vector<Data_Message> messages = msg.getCacheMSG();
        // assert(!messages.empty());
        // // assert(messages.back().getIDdestinataire() == "ADSL0001");
        // assert(messages.back().getIDexpediteur() == "EDSL0001");
        // assert(messages.back().getContenu() == "Test de message"); 
    }

    //test Securite et Cas d'erreurs
    void testVerifIdInexistant()
    {
        DataBase emp("entreprise_.db");
        assert(emp.testverif_if("EDSL0009") == false);
        assert(emp.verifieridexist("EDSL0009") == false);
    }


    void testMauvaisMotDePasse()
    {
        DataBase emp("entreprise_.db");
        assert(emp.verifierMDPdansBD("EDSL0001", "WrongPassword") == false);
    }

    void testRechercherId()
    {
        DataBase emp("entreprise_.db");
        const EmployeData& e = emp.testRechercherUnEmploye("EDSL0001");
        const EmployeData& e1 = emp.testRechercherUnEmploye("EDSL0009");
        assert(e.m_identifiant_Employe == "EDSL0001");
        assert(!(e1.m_identifiant_Employe == "EDSL0009") == true);
        assert((e1.m_nom == ""));
    }

    //Test Integrite des donnees (Changements)

    void testModifierInfosEmployes()
    {
        DataBase emp("entreprise_.db");
        emp.testChangerInfosEmploye("EDSL0001", "Doki", "Med", "Medecin");

        const EmployeData& e = emp.testRechercherUnEmploye("EDSL0001");
        assert(e.m_nom == "Doki");
        assert(e.m_prenom == "Med");
        assert(e.m_poste == "Medecin");
    }

    //Robustesse du Caching
    void testCoherenceChache()
    {
        DataBase emp("entreprise_.db");
        emp.chargerCache();  //On charge le cache

        emp.testChangerInfosEmploye("EDSL0002", "Doki_a", "Medi", "Ingenieur"); //On modifi un element
        const EmployeData& e = emp.testRechercherUnEmploye("EDSL0002"); //On recherche l'element modifie dans le cache
        assert(e.m_nom == "Doki_a"); 
    }


    // Utilisation de TEST_F (F pour Fixture)
TEST_F(DataBaseTest, AjouterEtRechercherEmploye) {
    Employe e("UIj", "John", "2023-12-05", "Celibataire", "Financier", "CDD", 
              "Password123", "doe@dsl.cm", "B", 30, 4500.0, "1998-02-20");
    
    db->ajouterEmployeTest(e);
    
    // On récupère les données
    EmployeData result = db->testRechercherUnEmploye("EDSL0001");

    // Assertions Google Test : plus besoin de std::cout ou d'assert manuel
    EXPECT_EQ(result.m_nom, "UIj");
    EXPECT_EQ(result.m_prenom, "John");
    EXPECT_DOUBLE_EQ(result.m_salaire, 4500.0);
}

TEST_F(DataBaseTest, VerifierMotDePasseValide) {
    // La DB est automatiquement recréée ici grâce au SetUp
    Employe e("UIj", "John", "2023-12-05", "Celibataire", "Financier", "CDD", 
              "Password123", "doe@dsl.cm", "B", 30, 4500.0, "1998-02-20");
    db->ajouterEmployeTest(e);

    EXPECT_TRUE(db->verifierMDPdansBD("EDSL0001", "Password123"));
    EXPECT_FALSE(db->verifierMDPdansBD("EDSL0001", "MauvaisMDP"));
}

TEST_F(DataBaseTest, ImprimerFichePaie){
    Employe e("UIj", "John", "2023-12-05", "Celibataire", "Financier", "CDD", 
              "Password123", "doe@dsl.cm", "B", 30, 4500.0, "1998-02-20");
    
    db->ajouterEmployeTest(e);

    db -> chargerCache();
    std::vector<EmployeData> liste = db->getCache();
    ASSERT_TRUE(!liste.empty());
    db -> testimprimer_fiche_paie_caching("EDSL0001");
    // db -> imprimer_fiche_paie("EDSL0001");

}

TEST_F(DataBaseTest, EnvoiEtReceptionMessages)
{
    //1. envoi d'un message
    db-> envoyer_MSG("ADSL0001", "EDSL0001", "Test de message", "Test");

    //2. recuperer les message du destinataire
    std::vector<Data_Message> msg = db -> recupererMessages("ADSL0001");

    //3. Test
    ASSERT_FALSE(msg.empty());

    ASSERT_EQ(msg.back().getIDexpediteur(), "EDSL0001");
    ASSERT_EQ(msg.back().getContenu(), "Test de message");
}

// TEST_F(DataBaseTest, PerformanceCaching)
// {
//     // Ajouter plusieurs employés pour tester la performance du caching
//     for(int i = 0; i < 100; ++i)
//     {
//         Employe e("Nom" + std::to_string(i), "Prenom" + std::to_string(i), "2023-12-05", "Celibataire", "Financier", "CDD", 
//                   "Password123", "doe" + std::to_string(i) + "@dsl.cm", "B", 30, 4500.0, "1998-02-20");
//         db->ajouterEmployeTest(e);
//     }

//     auto start = std::chrono::high_resolution_clock::now();
//     db->afficherEmploye(); // Sans cache
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double, std::milli> duration_no_cache = end - start;

//     db->chargerCache(); // Charger le cache

//     start = std::chrono::high_resolution_clock::now();
//     db->afficherEmploye_caching(db->getCache()); // Avec cache
//     end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double, std::milli> duration_cache = end - start;

//     std::cout << "\nTemps sans cache: " << duration_no_cache.count() << " ms" << std::endl;
//     std::cout << "Temps avec cache: " << duration_cache.count() << " ms\n" << std::endl;

//     EXPECT_LT(duration_cache.count(), duration_no_cache.count());
// }

TEST_F(DataBaseTest, VerifierIdentifiants)
{
    Employe e("UIj", "John", "2023-12-05", "Celibataire", "Financier", "CDD", 
              "Password123", "doe@dsl.cm", "B", 30, 4500.0, "1998-02-20");
    db->ajouterEmployeTest(e);

    ASSERT_TRUE(db->testverif_if("EDSL0001"));
    ASSERT_FALSE(db->verifieridexist("IDINEXISTANT"));
}

TEST_F(DataBaseTest, ActiverDesactiverEmploye)
{
    for(int i = 0; i < 1; ++i)
    {
        Employe e("Nom" + std::to_string(i), "Prenom" + std::to_string(i), "2023-12-05", "Celibataire", "Financier", "CDD", 
                  "Password123", "doe" + std::to_string(i) + "@dsl.cm", "B", 30, 4500.0, "1998-02-20");
        db->ajouterEmployeTest(e);
    }

    db->testactiverdesactiverEmployer("EDSL0001", 2);
    const EmployeData& emp = db->testRechercherUnEmploye("EDSL0001");
    ASSERT_EQ(emp.m_etat, 0);

    
}

// Tests lire Messages
TEST_F(DataBaseTest, LireMessagesRecus)
{
    db->envoyer_MSG("ADSL0001", "EDSL0001", "Test de la BD", "TestBD");

    std::vector<Data_Message> msg = db->recupererMessages("ADSL0001");

    db->chargerCacheMSG();
    ASSERT_FALSE(msg.empty());
    ASSERT_EQ(msg.back().getContenu(), "Test de la BD");
    ASSERT_EQ(msg.back().getIDexpediteur(), "EDSL0001");
    ASSERT_EQ(msg.back().getObjet(), "TestBD");
}

//test de securite et Cas d'erreurs
TEST_F(DataBaseTest, TestVerifIdInexistant)
{
    ASSERT_EQ(db->testverif_if("IDINEXISTANT"), false);
    ASSERT_EQ(db->verifieridexist("IDINEXISTANT"), false);
}

TEST_F(DataBaseTest, TestMauvaisMotDePasse)
{
        Employe e("UIj", "John", "2023-12-05", "Celibataire", "Financier", "CDD", 
              "Password123", "doe@dsl.cm", "B", 30, 4500.0, "1998-02-20");
        db->ajouterEmployeTest(e);

        ASSERT_FALSE(db->verifierMDPdansBD("EDSL0001", "MauvaisMDP"));
}

TEST_F(DataBaseTest, TestRechercherId)
{
    for(int i = 0; i < 3; ++i)
    {
        Employe e("Nom" + std::to_string(i), "Prenom" + std::to_string(i), "2023-12-05", "Celibataire", "Financier", "CDD", 
                  "Password123", "doe" + std::to_string(i) + "@dsl.cm", "B", 30, 4500.0, "1998-02-20");
        db->ajouterEmployeTest(e);
    }

    const EmployeData& e = db->testRechercherUnEmploye("EDSL0002");
    const EmployeData& e1 = db->testRechercherUnEmploye("EDSL0006");

    ASSERT_TRUE(e.m_identifiant_Employe == "EDSL0002");
    ASSERT_FALSE(e1.m_identifiant_Employe == "EDSL0006");
    ASSERT_TRUE(e1.m_nom.empty());
}


//Test Integrite des donnees (Changements)
