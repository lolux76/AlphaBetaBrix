#include "info_coup.hh"
#include "brix.hh"
#include "jeu.hh"
#include <memory>

info_coup::info_coup(Brix const &coup, std::shared_ptr<Jeu> const &jeu, char piece, unsigned int nb_tour) : _nb_piece_aligne_joueur{0, 0, 0}, _nb_piece_aligne_adversaires{0, 0, 0}, _coup(coup), _jeu(jeu), _piece(piece), _nb_tour(nb_tour) {}

//  int _abscisseX;
//  int _ordonneeX;
//  int _abscisseO;
//  int _ordonneeO;

bool info_coup::horsPlateau(unsigned int ordonne, unsigned int absisse)
{
    if ((ordonne > MAX_HAUTEUR) || (ordonne < 0))
        return false;
    if ((absisse > MAX_LARGEUR) || (absisse < 0))
        return false;
}

void info_coup::alignement()
{

    unsigned int ord_j = 0;
    unsigned int abs_j = 0;
    unsigned int ord_a = 0;
    unsigned int abs_a = 0;
    char piece_a = ' ';
    char piece_j = ' ';

    if (_piece == 'O')
    {
        abs_j = _coup.getAo();
        abs_a = _coup.getAx();
        ord_j = _coup.getOo();
        ord_a = _coup.getOx();
        piece_a = 'X';
        piece_j = 'O';
    }
    else
    {
        abs_a = _coup.getAo();
        abs_j = _coup.getAx();
        ord_a = _coup.getOo();
        ord_j = _coup.getOx();
        piece_a = 'O';
        piece_j = 'X';

    }



    for (int i = 0; i < 4; i++) // vertical, on lit en dessous
    {
        if ((horsPlateau(ord_j - i, abs_j)) && _jeu->plateau()[ord_j - i][abs_j]!=piece_a){
            if(_jeu->plateau()[ord_j - i][abs_j]==piece_j){
                _nb_piece_aligne_joueur[1]++;
            }
        }
        else break;
    }

    for (int i = 0; i < 4; i++) // horizontal, on lit a gauche
    {
        if ((horsPlateau(ord_j, abs_j -i )) && _jeu->plateau()[ord_j][abs_j-i]!=piece_a){
            if(_jeu->plateau()[ord_j][abs_j-i]==piece_j){
                _nb_piece_aligne_joueur[0]++;
            }
        }
        else break;
    }

    for (int i = 0; i < 4; i++) // horizontal, on lit a droite
    {
        if ((horsPlateau(ord_j, abs_j +i )) && _jeu->plateau()[ord_j][abs_j+i]!=piece_a){
            if(_jeu->plateau()[ord_j][abs_j+i]==piece_j){
                _nb_piece_aligne_joueur[0]++;
            }
        }
        else break;
    }

    for (int i = 0; i < 4; i++) // diagonal descendente et on lit a gauche
    {
        if ((horsPlateau(ord_j -i, abs_j +i )) && _jeu->plateau()[ord_j -i][abs_j+i]!=piece_a){
            if(_jeu->plateau()[ord_j-i][abs_j+i]==piece_j){
                _nb_piece_aligne_joueur[2]++;
            }
        }
        else break;   
    }

    for (int i = 0; i < 4; i++) // diagonal descendente et on lit a droite
    {
        if ((horsPlateau(ord_j +i, abs_j -i )) && _jeu->plateau()[ord_j +i][abs_j-i]!=piece_a){
            if(_jeu->plateau()[ord_j+i][abs_j-i]==piece_j){
                _nb_piece_aligne_joueur[2]++;
            }
        }
        else break;   
    }

    
    
}