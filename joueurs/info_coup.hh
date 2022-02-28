#include "../brix.hh"
#include "../jeu.hh"
#include <memory>

struct s_position
{
    unsigned int ordonne;
    unsigned int abcisse;
};

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

    s_position _pos_j;
    s_position _pos_a; 

    std::shared_ptr<Jeu> _jeu;

    Brix _coup;


    char _piece;
    char _piece_a;

    bool horsPlateau(unsigned int ordonne, unsigned int absysse);

    void afficher_info_coup(std::string const &fichier) const;

public:
    info_coup(Brix const &coup, std::shared_ptr<Jeu> const &jeu, char piece, unsigned int nb_tour);
    double calcul(); //retourne le winrate du coup
    double print(std::string const &fichier); //retourne le winrate du coup
    void alignement(); 
};