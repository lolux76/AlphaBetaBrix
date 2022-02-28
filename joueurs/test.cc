#include "../brix.hh"
#include "../jeu.hh"
#include "info_coup.hh"
#include "joueur_alphabeta.hh"
#include <ostream>

int main () {
    Jeu jeux;
Brix b1 (3,0,2,0);
Brix b2 (3,1,2,1);
Brix b3 (3,2,2,2);
Brix b4 (3,3,2,3);
Brix b5 (3,4,2,4);


jeux.joue(b1);
jeux.joue(b2);
jeux.joue(b3);
jeux.joue(b4);

std::cout << jeux << std::endl;

info_coup info(b5,std::make_shared<Jeu>(jeux),'x',4);

info.alignement();
std::cout << info.accNbPieceAlignJ()[1]<< std::endl;
std::cout << info.accNbPieceAlignA()[1] << std::endl;

    return 0;
}
//Brix(int aX, int oX, int aO, int oO)
//info_coup(Brix const &coup, std::shared_ptr<Jeu> const &jeu, char piece, unsigned int nb_tour);
