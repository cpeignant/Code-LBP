#include "CImage.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <locale>
#include <iomanip>
using namespace std;

// Constructeur par d�faut
CImage::CImage() : nbLigne(0), nbCol(0), Tab_Image(nullptr) {}

// Destructeur
CImage::~CImage() {
    if (Tab_Image != nullptr) { // v�rifie si le tableau est vide
        for (int i = 0; i < nbLigne; ++i) {
            delete[] Tab_Image[i]; // supprime ligne par ligne
        }
        delete[] Tab_Image;
    }
}

// Constructeur avec des valeurs al�atoires
CImage::CImage(int ligne, int colonne) : nbLigne(ligne), nbCol(colonne), Tab_Image(nullptr) {

    srand(time(nullptr)); // Initialisation de la graine pour la g�n�ration al�atoire

    Tab_Image = new int* [nbLigne];
    for (int i = 0; i < nbLigne; ++i) {
        Tab_Image[i] = new int[nbCol];
    }
    for (int i = 0; i < nbLigne; ++i) {
        for (int j = 0; j < nbCol; ++j) {
            Tab_Image[i][j] = rand() % 256; // nombre al�atoire entre 0 et 255 
        }
    }
}

// M�thode pour sauvegarder la pseudo image dans un fichier

void CImage::saveMatrixToFile(const string& nom) {
    setlocale(LC_CTYPE, "fra");
    ofstream outFile(nom); // cr�ation du fichier
    if (outFile.is_open()) {
        for (int i = 0; i < nbLigne; ++i) {
            for (int j = 0; j < nbCol; ++j) {
                outFile << setw(5) << Tab_Image[i][j]; //remplissage du fichier avec les valeurs g�n�r�es pr�c�demment
            }
            outFile << endl;
        }
        outFile.close(); // fin de la cr�ation du fichier
        cout << "Image sauvegard�e dans le fichier : " << nom << endl;
    }
    else {
        cerr << "Impossible d'ouvrir le fichier pour sauvegarder l'image." << endl;
    }
}

// M�thode pour calculer le LBP

string CImage::seuil(int ligne, int col) {
    string code;
    int centre = Tab_Image[ligne][col]; // pixel qui va �tre "analys�"
    for (int i = -1; i <= 1; i++) { // parcourt les d�placements verticaux autour du pixel central
        for (int j = -1; j <= 1; j++) { // parcourt les d�placements horizontaux
            if (i == 0 && j == 0) continue; //v�rif si sur le pixel central

            // Gestion des bords de l'image
            int ligne_index = ligne + i;
            int col_index = col + j;
            if (ligne_index < 0 || ligne_index >= nbLigne || col_index < 0 || col_index >= nbCol) { // v�rifie si les indices calcul�s des pixels voisins sont en dehors des limites de l'image.
                code += (centre > Tab_Image[ligne][col]) ? 0 : 1; // Si le pixel est en dehors de l'image, utilisez la valeur du pixel central (�criera toujours 1)
            }
            else {
                int voisin = Tab_Image[ligne_index][col_index];
                code += (centre > voisin) ? 0 : 1; // �criture de 0 si la valeur du centre est sup�rieur au voisin
            }
        }
    }
    return code;
}




// M�thode pour convertir les codes binaires en nombres d�cimaux

int CImage::binaryToDecimal(const string& binaryCode) {
    int decimal = 0;
    for (int i = 0; i < binaryCode.length(); ++i) { // it�re jusqu'� avoir parcourut toute la longueur du code, normalement 8 ici.
        decimal += (binaryCode[i]) * pow(2, binaryCode.length() - i - 1);
    }
    return decimal;
}

// M�thode pour calculer l'histogramme

int* CImage::Histogramme() {
    int* hist = new int[256] {}; // Alloue dynamiquement un tableau de taille 256 avec des valeurs initialis�es � 0

    for (int i = 1; i < nbLigne - 1; ++i) {
        for (int j = 1; j < nbCol - 1; ++j) {
            string code = seuil(i, j);
            int decimal = binaryToDecimal(code);
            hist[decimal]++; // incr�mente de 1 � la valeur d�cimal du code LBP
        }
    }

    return hist;
}

// M�thode pour sauvegarder l'histogramme dans un fichier Excel

void CImage::saveHist(int* histogram, const std::string& nom) {

    ofstream file(nom, ios::out); //ouverture du fichier en �criture
    if (file.is_open()) { //v�rif ouverture fichier
        file << "Nombre d�cimal, nombre occurence" << endl;
        for (int i = 0; i < 256; ++i) {
            file << i << "," << histogram[i] << endl; // �criture dans le fichier

        }
        file.close();
        cout << "Histogramme sauvegard� dans le fichier : " << nom << endl; // affichage de la confirmation d'�criture dans la console
    }
    else {
        cout << "Impossible d'ouvrir le fichier pour sauvegarder l'histogramme." << endl;
    }
}




// Surcharge de l'op�rateur de sortie <<
ostream& operator<<(ostream& os, const CImage& image) {
    for (int i = 0; i < image.nbLigne; ++i) {
        for (int j = 0; j < image.nbCol; ++j) {
            os << setw(5) << image.Tab_Image[i][j];
        }
        os << endl;
    }
    return os;
}

// Surcharge de l'op�rateur d'entr�e >>
istream& operator>>(istream& is, CImage& image) {
    for (int i = 0; i < image.nbLigne; ++i) {
        for (int j = 0; j < image.nbCol; ++j) {
            is >> image.Tab_Image[i][j];
        }
    }
    return is;
}
