#include "info_coup.hh"
#include <memory>
#include "../jeu.hh"
#include "joueur_alphabeta.hh"
#include <ostream>

int main()
{
    std::shared_ptr<Jeu> initial = std::make_shared<Jeu>();

    auto coups = std::move(Joueur_AlphaBeta::rechercheCoupValide(*initial));
    std::cout << "Taille du tableau : " << coups->size() << std::endl;
    for (auto coup : *coups)
    {
        info_coup c(coup, initial, 'O', 0);
        c.print("fichier_infos.csv");
    }
}