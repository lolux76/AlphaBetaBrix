#include "info_coup.hh"
#include <memory>
#include "../jeu.hh"
#include "joueur_alphabeta.hh"
#include <ostream>
#include <fstream>

int main()
{
    std::ofstream fl;
    fl.open("fichier_infos.csv");
    if (fl.is_open())
    {
        fl << "nb_tour" << ";" << "nb_piece_aligne_joueur_horizont" << ";" << "nb_piece_aligne_joueur_vert"<< ";" << "nb_piece_aligne_joueur_diag_montante" << ";"<< "nb_piece_aligne_joueur_diag_descendante" << ";" << "nb_piece_aligne_joueur_horiz_adv" << ";" << "nb_piece_aligne_joueur_vert_adv" << ";" << "nb_piece_aligne_joueur_diag_montante_adv" << ";" << "nb_piece_aligne_joueur_diag_descendante_adv" << ";" << "POS J ABS" << ";" << "POS J ORD" << ";" << "POS A ABS" << ";" << "POS A ORD" << ";" << "taux_victoire" << std::endl;
        fl.close();
    }

    std::shared_ptr<Jeu> initial = std::make_shared<Jeu>();
    initial->reset();
    auto coups = std::move(Joueur_AlphaBeta::rechercheCoupValide(*initial));
    for (auto coup : *coups)
    {
        info_coup c(coup, initial, 'x', 0);
        c.print("fichier_infos.csv");
    }
}