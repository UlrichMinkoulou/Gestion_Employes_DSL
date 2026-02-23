#include <iostream>
#include "Data_base.h"
#include "Employe.h"
#include "Connexion.h"
#include "Admin_Data_Base.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

// --- LIBRAIRIES DESSIN ET ECRITURE ---
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define CANVAS_ITY_IMPLEMENTATION
#include "canvas_ity.hpp"


std::vector<unsigned char> chargerPolice(std::string chemin) {
    std::ifstream file(chemin, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier .ttf !" << std::endl;
        return {}; // Renvoie un vector vide
    }
    std::streamsize size = file.tellg();
    if (size <= 0) return {};

    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> buffer(size);
    file.read((char*)buffer.data(), size);
    return buffer;
}


void generer_fiche_paie_image(std::string id_emp) {
    int largeur = 800;
    int hauteur = 1000;
    canvas_ity::canvas cv(largeur, hauteur);


   std::vector<unsigned char> font_data = chargerPolice("/home/ulrich/Downloads/Roboto-Regular.ttf");
    cv.set_font(font_data.data(), (int)font_data.size(), 30.0f); // Charger une police pour le titre

    if (largeur <= 0 || hauteur <= 0 || largeur > 5000 || hauteur > 5000) {
    std::cerr << "Dimensions invalides !" << std::endl;
    return;
}
    // --- FOND & CADRE ---
    std::string nom, poste; double brut;
    cv.set_color(canvas_ity::fill_style, 255, 255, 255, 255); // Blanc
    cv.fill_rectangle(0, 0, 800, 600);
    
    cv.set_color(canvas_ity::stroke_style, 44, 62, 80, 255); // Bleu foncé
    cv.set_line_width(5);
    cv.stroke_rectangle(10, 10, 780, 580);

    // --- ENTÊTE ---
   
    cv.set_font(font_data.data(), (int)font_data.size(), 30.0f); // Note: il faut charger une police .ttf pour un rendu pro
    cv.set_color(canvas_ity::fill_style, 44, 62, 80, 255);
    cv.fill_text("BULLETIN DE PAIE", 250, 60);  

    cv.set_line_width(2);
    cv.move_to(50, 80);
    cv.line_to(750, 80);
    cv.stroke();

    // --- INFOS EMPLOYÉ ---
    std::string employe = "EMPLOYE : " + nom;
    cv.set_font(font_data.data(), (int)font_data.size(), 18.0f); // Gras pour les infos
    cv.set_color(canvas_ity::fill_style, 0, 0, 0, 255);
    cv.fill_text(employe.c_str(), 50, 130);
    cv.fill_text(("Poste : " + poste).c_str(), 50, 160);

    // --- TABLEAU DES MONTANTS ---
    // Dessiner l'entête du tableau
    cv.set_color(canvas_ity::fill_style, 236, 240, 241, 255); // Gris clair
    cv.fill_rectangle(50, 220, 700, 40);
    
    cv.set_color(canvas_ity::fill_style, 0, 0, 0, 255);
    cv.fill_text("Désignation", 60, 245);
    cv.fill_text("Montant", 600, 245);

    // Calculs
    double cotisations = brut * 0.22;
    double net = brut - cotisations;

    // Lignes du tableau
    cv.fill_text("Salaire Brut", 60, 300);
    cv.fill_text((std::to_string((int)brut) + " €").c_str(), 600, 300);

    cv.set_color(canvas_ity::fill_style, 192, 57, 43, 255); // Rouge pour les retenues
    cv.fill_text("Cotisations Sociales (22%)", 60, 340);
    cv.fill_text(("-" + std::to_string((int)cotisations) + " €").c_str(), 600, 340);

    // --- TOTAL NET ---
    cv.set_color(canvas_ity::fill_style, 46, 204, 113, 255); // Vert pour le Net
    cv.fill_rectangle(50, 400, 700, 50);
    
    cv.set_color(canvas_ity::fill_style, 255, 255, 255, 255);
   cv.set_font(font_data.data(), (int)font_data.size(), 22.0f); // Gras pour le total
    cv.fill_text("NET À PAYER", 60, 435);
    cv.fill_text((std::to_string((int)net) + " €").c_str(), 600, 435);

    // --- SAUVEGARDE ---
    // La bibliothèque génère un tableau de pixels (RGBA)
    std::vector<unsigned char> image(800 * 600 * 4);
    cv.get_image_data(image.data(), 800, 600, 800 * 4, 0, 0);
    // -------------------------------------------------------------

    // 1. Créer un buffer pour stocker les pixels (RGBA : 4 octets par pixel)
    std::cout << "Tentative de création d'un buffer de : " << (largeur * hauteur * 4) << " octets" << std::endl;
    std::vector<unsigned char> pixels(largeur * hauteur * 4);

    // 2. Transférer le dessin du canvas vers notre buffer de pixels
    cv.get_image_data(pixels.data(), largeur, hauteur, largeur * 4, 0, 0);

    // 3. Ecrire le fichier PNG sur le disque
    std::string nomFichier = "Fiche_Paie_" + id_emp + ".png";
    
    // Paramètres : Nom, Largeur, Hauteur, Canaux (4 pour RGBA), Pixels, Pas (Largeur * 4)
    int succes = stbi_write_png(nomFichier.c_str(), largeur, hauteur, 4, pixels.data(), largeur * 4);

    if (succes) {
        std::cout << "\033[32m[SUCCÈS]\033[0m Image générée : " << nomFichier << std::endl;
        
        // --- BONUS : Ouvrir l'image automatiquement (Windows) ---
        std::string cmd = "start " + nomFichier;
        system(cmd.c_str());
    } else {
        std::cerr << "\033[31m[ERREUR]\033[0m Impossible d'écrire le fichier PNG." << std::endl;
    }
}


int main(void) 
{

    // DataBase base_de_donne("entreprise_.db");
    // Admin_db bd_admin("dataBase_admin.db");

    // base_de_donne.ajouterEmploye();
    // base_de_donne.afficherEmploye();
    // bd_admin.afficherAdmin();
    // base_de_donne.activerdesactiverEmployer();

    // base_de_donne.envoyer_MSG("ADSL0001", "EDSL0001", "Mise a jour de mes avancements", "Avancement");
    // base_de_donne.afficher_MSG();


    // pageAcceuil();
        generer_fiche_paie_image("EDSL0001");




}

