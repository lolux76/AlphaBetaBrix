#include "brix.hh"
#include "jeu.hh"
#include <memory>



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

    std::shared_ptr<Jeu> _jeu;
    Brix _coup;
    char _piece;

    bool horsPlateau(unsigned int ordonne, unsigned int absysse) ; 



public:
    info_coup(Brix const &coup, std::shared_ptr<Jeu> const &jeu, char piece, unsigned int nb_tour);

    void calcul();
    void print(std::string & const fichier);
    void alignement();

};