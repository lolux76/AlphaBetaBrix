#include "info_coup.hh"
#include "joueur_alphabeta.hh"
#include <memory>
#include <fstream>
#include <ostream>

info_coup::info_coup(Brix const &coup, std::shared_ptr<Jeu> const &jeu, char piece, unsigned int nb_tour) : _nb_piece_aligne_joueur{0, 0, 0}, _nb_piece_aligne_adversaires{0, 0, 0}, _nb_tour(nb_tour), _taux_victoire(0.), _pos_j({0, 0}), _pos_a({0, 0}), _jeu(jeu), _coup(coup), _piece(piece)
{
    _piece_a = (_piece == 'o') ? 'x' : 'o';

    if (_piece == 'o')
    {
        _pos_j.abcisse = coup.getAo();
        _pos_j.ordonne = coup.getOo();
        _pos_a.abcisse = coup.getAx();
        _pos_a.ordonne = coup.getOx();
    }
    else
    {
        _pos_a.abcisse = coup.getAo();
        _pos_a.ordonne = coup.getOo();
        _pos_j.abcisse = coup.getAx();
        _pos_j.ordonne = coup.getOx();
    }
}

//  int _abscisseX;
//  int _ordonneeX;
//  int _abscisseO;
//  int _ordonneeO;

bool info_coup::horsPlateau(unsigned int ordonne, unsigned int absisse)
{
    return ((ordonne > MAX_HAUTEUR) || (ordonne < 0)) || ((absisse > MAX_LARGEUR) || (absisse < 0));
}

double info_coup::print(std::string const &fichier)
{
    alignement();
    std::shared_ptr<Jeu> jeu_suivant = _jeu;
    jeu_suivant->joue(_coup);

    // jeu terminé, recherche de vainqueur
    if (jeu_suivant->fini())
    {
        if ((jeu_suivant->partie_O() && _piece == 'o') || (jeu_suivant->partie_X() && _piece == 'x'))
        {
            _taux_victoire = 1.;
        }
        else
        {
            _taux_victoire = 0.;
        }
    }
    else
    { // le jeu n'est pas fini
        auto coups = std::move(Joueur_AlphaBeta::rechercheCoupValide(*jeu_suivant));
        double sum = 0;
        for (auto coup : *coups)
        {
            info_coup calc(coup, jeu_suivant, _piece_a, _nb_tour + 1);
            sum += calc.print(fichier);
        }

        _taux_victoire = (sum / coups->size());
    }

    std::cout << *jeu_suivant << std::endl;
    std::cout << "Taux de victoire : " << _taux_victoire << std::endl;

    afficher_info_coup(fichier);
    return _taux_victoire;
}

void info_coup::afficher_info_coup(std::string const &fichier) const
{
    std::ofstream fl;
    fl.open(fichier, std::ios_base::app);
    if (fl.is_open())
    {
        fl << _nb_tour << ";" << _nb_piece_aligne_joueur[0] << ";" << _nb_piece_aligne_joueur[1] << ";" << _nb_piece_aligne_joueur[2] << ";" << _taux_victoire << std::endl;
        fl.close();
    }
}

void info_coup::alignement()
{
// todo : check hors plateau adversaire + enlever else break : fait
// j'ai retiré tous les else if > remplacé par des if

    for (int i = 0; i < 4; i++) // vertical, on lit en dessous
    {
        if ((!horsPlateau(_pos_j.ordonne - i, _pos_j.abcisse)) || (!horsPlateau(_pos_a.ordonne - i, _pos_a.abcisse)))
        {

            if (_jeu->plateau()[_pos_j.ordonne - i][_pos_j.abcisse] == _piece)
            {
                _nb_piece_aligne_joueur[1]++;
            }
            if(_jeu->plateau()[_pos_a.ordonne - i][_pos_a.abcisse] == _piece_a){
                _nb_piece_aligne_adversaires[1]++;

            }
        }

        else
            break;
    }

    for (int i = 0; i < 4; i++) // horizontal, on lit a gauche
    {
        if ((!horsPlateau(_pos_j.ordonne, _pos_j.abcisse - i)) || (!horsPlateau(_pos_a.ordonne, _pos_a.abcisse -i)))
        {
            if (_jeu->plateau()[_pos_j.ordonne][_pos_j.abcisse - i] == _piece)
            {
                _nb_piece_aligne_joueur[0]++;
            }
            if(_jeu->plateau()[_pos_a.ordonne][_pos_a.abcisse -1] == _piece_a){
                _nb_piece_aligne_adversaires[0]++;

            }
        }
        else
            break;
    }

    for (int i = 0; i < 4; i++) // horizontal, on lit a droite
    {
        if ((!horsPlateau(_pos_j.ordonne, _pos_j.abcisse + i)) || (!horsPlateau(_pos_a.ordonne, _pos_a.abcisse +i)))
        {
            if (_jeu->plateau()[_pos_j.ordonne][_pos_j.abcisse + i] == _piece)
            {
                _nb_piece_aligne_joueur[0]++;
            }
            if(_jeu->plateau()[_pos_a.ordonne][_pos_a.abcisse + i] == _piece_a){
                _nb_piece_aligne_adversaires[0]++;

            }
        }
        else
            break;
    }

    for (int i = 0; i < 4; i++) // diagonal descendente et on lit a droite
    {
        if ((!horsPlateau(_pos_j.ordonne - i, _pos_j.abcisse + i)) || (!horsPlateau(_pos_a.ordonne - i, _pos_a.abcisse +i)))
        {
            if (_jeu->plateau()[_pos_j.ordonne - i][_pos_j.abcisse + i] == _piece)
            {
                _nb_piece_aligne_joueur[2]++;
            }
            if(_jeu->plateau()[_pos_a.ordonne - i][_pos_a.abcisse + i] == _piece_a){
                _nb_piece_aligne_adversaires[2]++;
            }
        }
        else
            break;
    }
    // !!!! remplacer le && par || provoque une erreur de ségmentation je comprends pas pourquoi 

    for (int i = 0; i < 4; i++) // diagonal descendente et on lit a gauche
    {
        if ((!horsPlateau(_pos_j.ordonne + i, _pos_j.abcisse - i)) && (!horsPlateau(_pos_a.ordonne +i, _pos_a.abcisse -i)))
        {
            if (_jeu->plateau()[_pos_j.ordonne + i][_pos_j.abcisse - i] == _piece)
            {
                _nb_piece_aligne_joueur[2]++;
            }
            if(_jeu->plateau()[_pos_a.ordonne + i][_pos_a.abcisse - i] == _piece_a){
                _nb_piece_aligne_adversaires[2]++;
            }
        }
        else
            break;
    }

    for (int i = 0; i < 4; i++) // diagonal montante et on lit a gauche
    {
        if ((!horsPlateau(_pos_j.ordonne - i, _pos_j.abcisse - i)) && (!horsPlateau(_pos_a.ordonne - i, _pos_a.abcisse -i)))
        {
            if (_jeu->plateau()[_pos_j.ordonne - i][_pos_j.abcisse - i] == _piece)
            {
                _nb_piece_aligne_joueur[2]++;
            }
            if(_jeu->plateau()[_pos_a.ordonne - i][_pos_a.abcisse - i] == _piece_a){
                _nb_piece_aligne_adversaires[2]++;
            }
        }
        else
            break;
    }

    for (int i = 0; i < 4; i++) // diagonal montante et on lit a droite
    {
        if ((!horsPlateau(_pos_j.ordonne + i, _pos_j.abcisse + i)) && (!horsPlateau(_pos_a.ordonne +i, _pos_a.abcisse +i)))
        {
            if (_jeu->plateau()[_pos_j.ordonne + i][_pos_j.abcisse + i] == _piece)
            {
                _nb_piece_aligne_joueur[2]++;
            }
            if(_jeu->plateau()[_pos_a.ordonne + i][_pos_a.abcisse + i] == _piece_a){
                _nb_piece_aligne_adversaires[2]++;
            }
        }
        else
            break;
    }
}