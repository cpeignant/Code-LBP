#include "CImage.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <locale>
#include <iomanip>
using namespace std;

// Constructeur par défaut
CImage::CImage() : nbLigne(0), nbCol(0), Tab_Image(nullptr) {}

// Destructeur
CImage::~CImage() {
    if (Tab_Image != nullptr) { // vérifie si le tableau est vide
        for (int i = 0; i < nbLigne; ++i) {
            delete[] Tab_Image[i]; // supprime ligne par ligne
        }
        delete[] Tab_Image;
    }
}

// Constructeur avec des valeurs aléatoires
CImage::CImage(int ligne, int colonne) : nbLigne(ligne), nbCol(colonne), Tab_Image(nullptr) {

    srand(time(nullptr)); // Initialisation de la graine pour la génération aléatoire

    Tab_Image = new int* [nbLigne];
    for (int i = 0; i < nbLigne; ++i) {
        Tab_Image[i] = new int[nbCol];
    }
    for (int i = 0; i < nbLigne; ++i) {
        for (int j = 0; j < nbCol; ++j) {
            Tab_Image[i][j] = rand() % 256; // nombre aléatoire entre 0 et 255 
        }
    }
}

// Méthode pour sauvegarder la pseudo image dans un fichier

void CImage::saveMatrixToFile(const string& nom) {
    setlocale(LC_CTYPE, "fra");
    ofstream outFile(nom); // création du fichier
    if (outFile.is_open()) {
        for (int i = 0; i < nbLigne; ++i) {
            for (int j = 0; j < nbCol; ++j) {
                outFile << setw(5) << Tab_Image[i][j]; //remplissage du fichier avec les valeurs générées précédemment
            }
            outFile << endl;
        }
        outFile.close(); // fin de la création du fichier
        cout << "Image sauvegardée dans le fichier : " << nom << endl;
    }
    else {
        cerr << "Impossible d'ouvrir le fichier pour sauvegarder l'image." << endl;
    }
}

// Méthode pour calculer le LBP

string CImage::seuil(int ligne, int col) {
    string code;
    int centre = Tab_Image[ligne][col]; // pixel qui va être "analysé"
    for (int i = -1; i <= 1; i++) { // parcourt les déplacements verticaux autour du pixel central
        for (int j = -1; j <= 1; j++) { // parcourt les déplacements horizontaux
            if (i == 0 && j == 0) continue; //vérif si sur le pixel central

            // Gestion des bords de l'image
            int ligne_index = ligne + i;
            int col_index = col + j;
            if (ligne_index < 0 || ligne_index >= nbLigne || col_index < 0 || col_index >= nbCol) { // vérifie si les indices calculés des pixels voisins sont en dehors des limites de l'image.
                code += (centre > Tab_Image[ligne][col]) ? 0 : 1; // Si le pixel est en dehors de l'image, utilisez la valeur du pixel central (écriera toujours 1)
            }
            else {
                int voisin = Tab_Image[ligne_index][col_index];
                code += (centre > voisin) ? 0 : 1; // écriture de 0 si la valeur du centre est supérieur au voisin
            }
        }
    }
    return code;
}




// Méthode pour convertir les codes binaires en nombres décimaux

int CImage::binaryToDecimal(const string& binaryCode) {
    int decimal = 0;
    for (int i = 0; i < binaryCode.length(); ++i) { // itére jusqu'à avoir parcourut toute la longueur du code, normalement 8 ici.
        decimal += (binaryCode[i]) * pow(2, binaryCode.length() - i - 1);
    }
    return decimal;
}

// Méthode pour calculer l'histogramme

int* CImage::Histogramme() {
    int* hist = new int[256] {}; // Alloue dynamiquement un tableau de taille 256 avec des valeurs initialisées à 0

    for (int i = 1; i < nbLigne - 1; ++i) {
        for (int j = 1; j < nbCol - 1; ++j) {
            string code = seuil(i, j);
            int decimal = binaryToDecimal(code);
            hist[decimal]++; // incrémente de 1 à la valeur décimal du code LBP
        }
    }

    return hist;
}

// Méthode pour sauvegarder l'histogramme dans un fichier Excel

void CImage::saveHist(int* histogram, const std::string& nom) {

    ofstream file(nom, ios::out); //ouverture du fichier en écriture
    if (file.is_open()) { //vérif ouverture fichier
        file << "Nombre décimal, nombre occurence" << endl;
        for (int i = 0; i < 256; ++i) {
            file << i << "," << histogram[i] << endl; // écriture dans le fichier

        }
        file.close();
        cout << "Histogramme sauvegardé dans le fichier : " << nom << endl; // affichage de la confirmation d'écriture dans la console
    }
    else {
        cout << "Impossible d'ouvrir le fichier pour sauvegarder l'histogramme." << endl;
    }
}




// Surcharge de l'opérateur de sortie <<
ostream& operator<<(ostream& os, const CImage& image) {
    for (int i = 0; i < image.nbLigne; ++i) {
        for (int j = 0; j < image.nbCol; ++j) {
            os << setw(5) << image.Tab_Image[i][j];
        }
        os << endl;
    }
    return os;
}

// Surcharge de l'opérateur d'entrée >>
istream& operator>>(istream& is, CImage& image) {
    for (int i = 0; i < image.nbLigne; ++i) {
        for (int j = 0; j < image.nbCol; ++j) {
            is >> image.Tab_Image[i][j];
        }
    }
    return is;
}
