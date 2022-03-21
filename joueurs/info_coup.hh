#pragma once

#include "../brix.hh"
#include "../jeu.hh"
#include <memory>
#include <vector>
#include "jeuBinaire.hh"



struct s_position
{
    unsigned short ordonne;
    unsigned short abcisse;
};

class info_coup
{
private:
    unsigned short _nb_piece_aligne_joueur[3];
    // 0 : ligne
    // 1 : colonne
    // 2 : diagonale
    unsigned short _nb_piece_aligne_adversaires[3];
    // 0 : ligne
    // 1 : colonne
    // 2 : diagonale
    unsigned short _nb_tour;
    double _taux_victoire;

    s_position _pos_j;
    s_position _pos_a;

    std::shared_ptr<JeuBinaire> _jeu;

    Brix _coup;

    char _piece;
    char _piece_a;

    bool horsPlateau(unsigned int ordonne, unsigned int absysse);

    void afficher_info_coup(std::string const &fichier) const;

public:
    std::vector<unsigned int> accNbPieceAlignJ()
    {
        std::vector<unsigned int> ret;
        ret.push_back(_nb_piece_aligne_joueur[0]);
        ret.push_back(_nb_piece_aligne_joueur[1]);
        ret.push_back(_nb_piece_aligne_joueur[2]);
        return ret;

    }
    std::vector<unsigned int> accNbPieceAlignA()
    {
        std::vector<unsigned int> ret;
        ret.push_back(_nb_piece_aligne_adversaires[0]);
        ret.push_back(_nb_piece_aligne_adversaires[1]);
        ret.push_back(_nb_piece_aligne_adversaires[2]);
        return ret;
    }

    info_coup(Brix const &coup, std::shared_ptr<JeuBinaire> const &jeu, char piece, unsigned int nb_tour);
    double print(std::string const &fichier); // retourne le winrate du coup
    void alignement();

    int eval();

    std::shared_ptr<info_coup> clone() const;
};