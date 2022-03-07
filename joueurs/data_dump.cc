#include "info_coup.hh"
#include <memory>
#include "../jeu.hh"
#include "joueur_alphabeta.hh"
#include <ostream>

int main()
{
    std::shared_ptr<Jeu> initial = std::make_shared<Jeu>();
    initial->reset();
    auto coups = std::move(Joueur_AlphaBeta::rechercheCoupValide(*initial));
    for (auto coup : *coups)
    {
        info_coup c(coup, initial, 'x', 0);
        c.print("fichier_infos.csv");
    }
}