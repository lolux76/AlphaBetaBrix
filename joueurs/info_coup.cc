#include "info_coup.hh"
#include "brix.hh"
#include "jeu.hh"
#include <memory>

info_coup::info_coup(Brix const &coup, std::shared_ptr<board> const &plateau, char piece, unsigned int nb_tour) : _nb_piece_aligne_joueur{0, 0, 0}, _nb_piece_aligne_adversaires{0, 0, 0}, _coup(coup), _plateau(plateau), _piece(piece), _nb_tour(nb_tour){}
