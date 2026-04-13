#include <cassert>
#include "../include/Data_base.h"
#include "../include/Employe.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <functional>

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

int test_passed = 0;
int test_failed = 0;

void run_test(int current, int total, std::string name, std::function<void()> test_func)
{
    auto start = std::chrono::high_resolution_clock::now();

    try
    {
        test_func();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        std::stringstream time_ss;
        time_ss << std::fixed << std::setprecision(2) << duration.count() << " ms";
        size_t id = std::hash<std::string>{}(name);

        // On regroupe "9/19" dans une seule string pour l'aligner parfaitement
        std::string progress = std::to_string(current) + "/" + std::to_string(total);

        std::cout << ANSI_RESET << std::right << std::setw(6) << progress; // Fixe la largeur du compteur
        std::cout << ANSI_GREEN << ANSI_BOLD << " [PASS] " << ANSI_RESET
                  << std::hex << std::setw(8) << (id & 0xFFFFFFFF) << std::dec << "   "
                  << std::left << std::setw(12) << time_ss.str() << " - "
                  << ANSI_BOLD << name << ANSI_RESET << std::endl; 
    }
    catch (...)
    {
        std::string progress = std::to_string(current) + "/" + std::to_string(total);
        std::cout << std::left << std::setw(6) << progress
                  << ANSI_RED << ANSI_BOLD << " [FAIL] " << ANSI_RESET 
                  << name << std::endl;
    }
}



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

int main(void)
{

    std::remove("entreprise_.db"); // Supprimer la base de données existante pour des tests propres
    // int total_tests = 2;
    int total_tests = 17;
    int current_test = 1;

    std::cout << ANSI_BOLD << ANSI_BLUE << "Lancement des tests unitaires..." << ANSI_RESET  <<std::endl;

    //Appel des tests
    run_test(current_test++, total_tests, "testEnvoyerMSG", testEnvoyerMSG); //1
    run_test(current_test++, total_tests, "testAjouterEmploye", testAjouterEmploye); //2
    run_test(current_test++, total_tests, "testRechercherEmploye", testRechercherEmploye); //3
    run_test(current_test++, total_tests, "testImprimerFichePaie", testImprimerFichePaie); //4
    run_test(current_test++, total_tests, "testVerifieridentifiant", testVerifieridentifiant); //5
    run_test(current_test++, total_tests, "testActiverDesactiverEmploye", testActiverDesactiverEmploye); //6
    run_test(current_test++, total_tests, "testLireMessagesRecus", testLireMessagesRecus); //7
    // run_test(current_test++, total_tests, "testPerformanceCaching", testPerformanceCaching); //8
    // run_test(current_test++, total_tests, "testPerformanceCachingMSG", testPerformanceCachingMSG); //9
    run_test(current_test++, total_tests, "testVerifierMDP", tesVerifierMDP); //10
    run_test(current_test++, total_tests, "testRecupererMessages", testRecupererMessages); //11
    run_test(current_test++, total_tests, "testChangerInfosEmploye", tesChangerInfosEmploye); //12
    run_test(current_test++, total_tests, "testConnexionEmploye", testConnexionEmploye); //13
    run_test(current_test++, total_tests, "testChargerCacheMSG", testChargerCacheMSG); //14
    run_test(current_test++, total_tests, "testCoherenceChache", testCoherenceChache); //15
    run_test(current_test++, total_tests, "testVerifIdInexistant", testVerifIdInexistant); //16
    run_test(current_test++, total_tests, "testMauvaisMotDePasse", testMauvaisMotDePasse); //17
    run_test(current_test++, total_tests, "testRechercherId", testRechercherId); //18
    run_test(current_test++, total_tests, "testModifierInfosEmployes", testModifierInfosEmployes); //19


    //autres tests...
    
    // testEnvoyerMSG();
    std::cout << "\nTests TERMINES!" << std::endl;
}