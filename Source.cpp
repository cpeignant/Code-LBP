#include "CImage.h"
#include <iostream>

using namespace std;

int main() {
    // Utilisation

    CImage image(10, 10); // Cr�e une image de taille 10x10 avec des valeurs al�atoires
    image.saveMatrixToFile("image.txt"); // Sauvegarde l'image dans un fichier
    int* histogramme = image.Histogramme(); // Calcule l'histogramme
    image.saveHist(histogramme, "histogramme.csv"); // Sauvegarde l'histogramme dans un fichier Excel


    return 0;
}
