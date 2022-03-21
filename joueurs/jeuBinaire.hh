#pragma once

#include <iostream>
#include <array>
#include "../brix.hh"
#include "../jeu.hh"
#include "bitset"

/*Class JeuBinaire:
 *           Contient les méthodes de la classe Jeu réadaptées à une utilisation en binaire pour économiser du temps de calcul
 *       Représentation des cases:
 *           - Vide : 0b00
 *           - X : 0b01
 *           - O : 0b10
 */

class JeuBinaire
{

private:
    using ligne = std::array<std::bitset<2>, MAX_LARGEUR>;
    using board = std::array<ligne, MAX_HAUTEUR>;
    board _plateau;
    bool _alignementX;
    bool _alignementO;
    Etat _etat;
    int _nb_tours;

    void initialiserPlateauBinaire(Jeu jeu); // Initialise le plateau en binaire

public:
    JeuBinaire(Jeu jeu);
    JeuBinaire(JeuBinaire const &jeuBinaire) = default;
    bool coordValide(Brix const &b) const;                // La piece est dans le plateau
    bool voisin(Brix const &b) const;                     // La partie basse de la piece bien formé a un voisin collé a elle
    int nbCoupJoue() const;                               // Retourne le nombre de coup deja joue
    bool coup_licite(Brix const &coup, int nb) const;     // Vérifie si le nb-ième coup est licite
    void joue(Brix const &coup);                          // Joue le coup (sans vérifier qu'il est licite)
    bool fini() const;                                    // Vérifie si un état final a été atteint (dans ce cas le jeu prend fin)
    bool partie_nulle() const;                            // Vérifie si la partie est nulle
    bool getAlignement_O() const { return _alignementO; } // accesseur à alignementO
    bool getAlignement_X() const { return _alignementX; } // accesseur à alignementX
    bool partie_X() const;                                // Vérifie si la partie est gagnée par x
    bool partie_O() const;                                // Vérifie si la partie est gagnée par o
    board const &plateau() const
    {
        return _plateau;
    }; // retourne le plateau en cours
};