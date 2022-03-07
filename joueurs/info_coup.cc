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

    bool align_j[7] = {true, true, true, true, true, true, true}; // vertical, horizontal G,D, Diag HG, HD, BG, BD
    bool align_a[7] = {true, true, true, true, true, true, true}; // vertical, horizontal G,D, Diag HG, HD, BG, BD


    for (int i = 1; i < 5; i++)
    {
        // vertical
        if (align_j[0] && !horsPlateau(_pos_j.ordonne - i, _pos_j.abcisse) && _jeu->plateau()[_pos_j.ordonne - i][_pos_j.abcisse] == _piece)
        {
            _nb_piece_aligne_joueur[1]++;
        }
        else
            align_j[0] = false;

        if (align_a[0] && !horsPlateau(_pos_a.ordonne - i, _pos_a.abcisse) && _jeu->plateau()[_pos_a.ordonne - i][_pos_a.abcisse] == _piece_a)
        {
            _nb_piece_aligne_adversaires[1]++;
        }
        else
            align_a[0] = false;

        // horizontal G
        if (align_j[1] && !horsPlateau(_pos_j.ordonne, _pos_j.abcisse - i) && _jeu->plateau()[_pos_j.ordonne][_pos_j.abcisse - i] == _piece)
        {
            _nb_piece_aligne_joueur[0]++;
        }
        else
            align_j[1] = false;

        if (align_a[1] && !horsPlateau(_pos_a.ordonne, _pos_a.abcisse - i) && _jeu->plateau()[_pos_a.ordonne][_pos_a.abcisse - i] == _piece_a)
        {
            _nb_piece_aligne_adversaires[0]++;
        }
        else
            align_a[1] = false;

        // horizontal D
        if (align_j[2] && !horsPlateau(_pos_j.ordonne, _pos_j.abcisse + i) && _jeu->plateau()[_pos_j.ordonne][_pos_j.abcisse + i] == _piece)
        {
            _nb_piece_aligne_joueur[0]++;
        }
        else
            align_j[2] = false;

        if (align_a[2] && !horsPlateau(_pos_a.ordonne, _pos_a.abcisse + i) && _jeu->plateau()[_pos_a.ordonne][_pos_a.abcisse + i] == _piece_a)
        {
            _nb_piece_aligne_adversaires[0]++;
        }
        else
            align_a[2] = false;

        // diagonale HG
        if (align_j[3] && !horsPlateau(_pos_j.ordonne + i, _pos_j.abcisse - i) && _jeu->plateau()[_pos_j.ordonne + i][_pos_j.abcisse - i] == _piece)
        {
            _nb_piece_aligne_joueur[2]++;
        }
        else
            align_j[3] = false;

        if (align_a[3] && !horsPlateau(_pos_a.ordonne + i, _pos_a.abcisse - i) && _jeu->plateau()[_pos_a.ordonne + i][_pos_a.abcisse - i] == _piece_a)
        {
            _nb_piece_aligne_adversaires[2]++;
        }
        else
            align_a[3] = false;

        // diagonale HD
        if (align_j[4] && !horsPlateau(_pos_j.ordonne + i, _pos_j.abcisse + i) && _jeu->plateau()[_pos_j.ordonne + i][_pos_j.abcisse + i] == _piece)
        {
            _nb_piece_aligne_joueur[2]++;
        }
        else
            align_j[4] = false;

        if (align_a[4] && !horsPlateau(_pos_a.ordonne + i, _pos_a.abcisse + i) && _jeu->plateau()[_pos_a.ordonne + i][_pos_a.abcisse + i] == _piece_a)
        {
            _nb_piece_aligne_adversaires[2]++;
        }
        else
            align_a[4] = false;

        // diagonale BG
        if (align_j[5] && !horsPlateau(_pos_j.ordonne - i, _pos_j.abcisse - i) && _jeu->plateau()[_pos_j.ordonne - i][_pos_j.abcisse - i] == _piece)
        {
            _nb_piece_aligne_joueur[2]++;
        }
        else
            align_j[5] = false;

        if (align_a[5] && !horsPlateau(_pos_a.ordonne - i, _pos_a.abcisse - i) && _jeu->plateau()[_pos_a.ordonne - i][_pos_a.abcisse - i] == _piece_a)
        {
            _nb_piece_aligne_adversaires[2]++;
        }
        else
            align_a[5] = false;

        // diagonale BD
        if (align_j[6] && !horsPlateau(_pos_j.ordonne - i, _pos_j.abcisse + i) && _jeu->plateau()[_pos_j.ordonne - i][_pos_j.abcisse + i] == _piece)
        {
            _nb_piece_aligne_joueur[2]++;
        }
        else
            align_j[6] = false;

        if (align_a[6] && !horsPlateau(_pos_a.ordonne - i, _pos_a.abcisse + i) && _jeu->plateau()[_pos_a.ordonne - i][_pos_a.abcisse + i] == _piece_a)
        {
            _nb_piece_aligne_adversaires[2]++;
        }
        else
            align_a[6] = false;
    }
}