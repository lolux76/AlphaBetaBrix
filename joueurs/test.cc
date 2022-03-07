#include "../brix.hh"
#include "../jeu.hh"
#include "info_coup.hh"
#include "joueur_alphabeta.hh"
#include <ostream>

int main () {
    Jeu jeux;

//Brix(int aX, int oX, int aO, int oO)

// TEST VERTICAL : 
Brix b1 (3,0,2,0);
Brix b2 (3,1,2,1);
Brix b3 (3,2,2,2);
Brix b4 (3,3,2,3);

jeux.joue(b1);
jeux.joue(b2);
jeux.joue(b3);

std::cout << jeux << std::endl;

info_coup info(b4,std::make_shared<Jeu>(jeux),'x',4);

info.alignement();
std::cout << "alignement vertical : "<<std::endl;
std::cout << info.accNbPieceAlignJ()[1]<< std::endl;
std::cout << info.accNbPieceAlignA()[1] << std::endl;


//// TEST HORIZONTAL : 
// Brix b5 (0,0,0,1);
// Brix b6 (1,0,1,1);
// Brix b7 (3,0,3,1);
// Brix b8 (4,0,4,1);
// Brix b9 (2,0,2,1);

// jeux.joue(b5);
// jeux.joue(b6);
// jeux.joue(b7);
// jeux.joue(b8);

// std::cout << jeux << std::endl;

// info_coup info2(b9,std::make_shared<Jeu>(jeux),'x',4);

// info2.alignement();
// std::cout << "alignement horizontal : "<<std::endl;
// std::cout << info2.accNbPieceAlignJ()[0]<< std::endl;
// std::cout << info2.accNbPieceAlignA()[0] << std::endl;

// // TEST DIAGONALE :
// Brix b10(1,0,0,0);
// Brix b11(0,1,1,1);
// Brix b12(0,3,1,3);
// Brix b13(5,0,6,0);
// Brix b14(4,1,5,1);
// Brix b15(4,3,5,3);
// Brix b16(5,4,6,4);
// Brix b17(3,2,2,2);

// jeux.joue(b10);
// jeux.joue(b11);
// jeux.joue(b12);
// jeux.joue(b13);
// jeux.joue(b14);
// jeux.joue(b15);
// jeux.joue(b16);

// std::cout << jeux << std::endl;

// info_coup info3(b17,std::make_shared<Jeu>(jeux),'x',4);

// info3.alignement();
// std::cout << "alignement diagonale : "<<std::endl;
// std::cout << info3.accNbPieceAlignJ()[2]<< std::endl;
// std::cout << info3.accNbPieceAlignA()[2] << std::endl;







// // TEST HORS PLATEAU :
// Brix b18(7,1,6,1);
// Brix b19(7,0,6,0);
// Brix b20(7,2,8,2);

// jeux.joue(b18);
// jeux.joue(b19);
// // jeux.joue(b20);

//  std::cout << jeux << std::endl;

//  info_coup info4(b20,std::make_shared<Jeu>(jeux),'x',4);

// info4.alignement();
// std::cout << "alignement vertical : "<<std::endl;
// std::cout << info4.accNbPieceAlignJ()[1]<< std::endl;
// std::cout << info4.accNbPieceAlignA()[1] << std::endl;

    return 0;
}

//info_coup(Brix const &coup, std::shared_ptr<Jeu> const &jeu, char piece, unsigned int nb_tour);
