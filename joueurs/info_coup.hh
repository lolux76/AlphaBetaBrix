#include "brix.hh"
#include "jeu.hh"
#include <memory>

using ligne = std::array<char, MAX_LARGEUR>;
using board = std::array<ligne, MAX_HAUTEUR>;

class info_coup
{
private:
    unsigned int _nb_piece_aligne_joueur[3];
    // 0 : ligne
    // 1 : colonne
    // 2 : diagonale
    unsigned int _nb_piece_aligne_adversaires[3];
    // 0 : ligne
    // 1 : colonne
    // 2 : diagonale
    unsigned int _nb_tour;
    double _taux_victoire;

    std::shared_ptr<board> _plateau;
    Brix _coup;
    char _piece;



public:
    info_coup(Brix const &coup, std::shared_ptr<board> const &plateau, char piece, unsigned int nb_tour);

    void calcul();
    void print(std::string & const fichier);

};