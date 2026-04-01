#include <cassert>
#include "../include/Data_base.h"
// #include "../include/Employe.h

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

        //Generation d'un ID cout (hash du nom)
        size_t id = std::hash<std::string>{}(name);

        std::cout << current << "/" << total
                  << " [" <<ANSI_GREEN << ANSI_BOLD << "PASS" << ANSI_RESET << "] "
                  << std::hex << (id & 0xFFFFFFFF) << std::dec << "   "
                  << std::fixed << std::setprecision(2) << duration.count() << " ms - " 
                  << name << std::endl; 
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

int main(void)
{
    int total_tests = 1;
    int current_test = 1;

    std::cout << ANSI_BOLD << ANSI_BLUE << "Lancement des tests unitaires..." << ANSI_RESET  <<std::endl;

    //Appel des tests
    run_test(current_test++, total_tests, "testEnvoyerMSG", testEnvoyerMSG);

    //autres tests...
    
    // testEnvoyerMSG();
    std::cout << "\nTests TERMINES!" << std::endl;
}