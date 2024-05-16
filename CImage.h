#pragma once
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

class CImage {
private:
    int** Tab_Image;
    int nbLigne;
    int nbCol;

public:
    CImage();
    ~CImage();
    CImage(int ligne, int colonne);
    void saveMatrixToFile(const string& nom);
    string seuil(int ligne, int col);
    int binaryToDecimal(const string& binaryCode);
    int* Histogramme();
    void saveHist(int* histogram, const string& nom);

    friend ostream& operator<<(ostream& os, const CImage& image);
    friend istream& operator>>(istream& is, CImage& image);
};
