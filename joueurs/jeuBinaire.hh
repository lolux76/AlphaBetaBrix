#pragma once
#include <bitset>
#include "jeu.hh"

#define JeuBinaireBitset std::bitset<8 * 44 * 2>

enum class GameState {ALIGNEMENT_X, ALIGNEMENT_Y, NULLE, EN_COURS};


class JeuBinaire
{
private:
    JeuBinaireBitset plateauBinaire;
    int nb_tours;
    bool alignementX;
    bool alignementY;
    GameState etat;
    

    JeuBinaireBitset initialiserBitset(Jeu &jeu);

public:
    JeuBinaire(Jeu jeu);
    bool coordValide(Brix const & coup) const;//La piece est dans le plateau
    int nbCoupJoue() const;// Retourne le nombre de coup deja joue
    bool coup_licite(Brix const & coup, int nb) const; // Vérifie si le nb-ième coup est licite
    bool voisin(Brix const & coup) const;//La partie basse de la piece bien formé a un voisin collé a elle
    void joue(Brix const & coup); // Joue le coup (sans vérifier qu'il est licite)
};