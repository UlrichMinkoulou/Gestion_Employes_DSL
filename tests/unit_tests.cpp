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

void run_test(int current, int total, std::string name, std::function<void()> test_func)
{
    auto start = std::chrono::high_resolution_clock::now();

    try
    {
        test_func();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        // 1. On prépare la partie "Temps" dans un flux
    std::stringstream time_ss;
    time_ss << std::fixed << std::setprecision(2) << duration.count() << " ms";

 
        //Generation d'un ID cout (hash du nom)
        size_t id = std::hash<std::string>{}(name);

        std::cout << ANSI_RESET << current << "/" << total;
        std::cout << ANSI_GREEN << ANSI_BOLD << " [" << "PASS" << "] " << ANSI_RESET
                  << std::hex << (id & 0xFFFFFFFF) << std::dec << "   "
                  <<std::left << std::setw(12) << time_ss.str() << " - "
                  << ANSI_BOLD << name << ANSI_RESET << std::endl; 
    }
    catch (...)
    {
        std::cout << ANSI_RED << ANSI_BOLD << "/" << "[FAIL]  " << ANSI_RESET << name << std::endl;
    }
}


void testEnvoyerMSG()
{
    DataBase db("entreprise_.db");
    db.envoyer_MSG("ADSL0001", "EDSL0001", "Test de message", "Test");
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
        Employe e("Troscow", "Soh","2023-12-05", "Celibataire", "Ingenieur", "CDD", "Password123", "troscowh@dsl.cm", "B", 30, 10400.0, "1998-02-20");
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
    const EmployeData& e = emp.testRechercherUnEmploye("EDSL0001");
    assert(!e.m_identifiant_Employe.empty());
    assert(e.m_nom == "Doe");
    // std::cout << "Employe trouve: " << e.m_nom << " " << e.m_prenom << std::endl;

    emp.chargerCache();
    const std::vector<EmployeData>& listeemployes = emp.getCache();

    for(const auto& emp : listeemployes)
    {
        if(emp.m_nom == "Doe")
        {
            assert(emp.m_prenom == "John");
            assert(emp.m_identifiant_Employe == "EDSL0001");
            assert(emp.m_etat == 1);
            assert(emp.m_salaire == 4500.0);
            assert(emp.m_email == "doe@dsl.cm");
        }
    }

}


int main(void)
{
    int total_tests = 3;
    int current_test = 1;

    std::cout << ANSI_BOLD << ANSI_BLUE << "Lancement des tests unitaires..." << ANSI_RESET  <<std::endl;

    //Appel des tests
    run_test(current_test++, total_tests, "testEnvoyerMSG", testEnvoyerMSG);
    run_test(current_test++, total_tests, "testAjouterEmploye", testAjouterEmploye);
    run_test(current_test++, total_tests, "testRechercherEmploye", testRechercherEmploye);

    //autres tests...
    
    // testEnvoyerMSG();
    std::cout << "\nTests TERMINES!" << std::endl;
}