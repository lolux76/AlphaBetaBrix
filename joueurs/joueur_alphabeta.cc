#include "joueur_alphabeta.hh"
#include <memory>
#include <iostream>
Joueur_AlphaBeta::Joueur_AlphaBeta(std::string nom, bool joueur)
    : Joueur(nom, joueur)
{
}

/*char Joueur_AlphaBeta::nom_abbrege() const
{
    return 'A';
}
*/

std::unique_ptr<std::vector<Brix>> Joueur_AlphaBeta::rechercheCoupValide(Jeu jeu)
{

    // VARIABLES LOCALES
    std::unique_ptr<std::vector<Brix>> coupValide = std::make_unique<std::vector<Brix>>();
    Brix b_candidate;
    int tour = jeu.nbCoupJoue() + 1; // la b_candidate devra être valide au tour auquel on va la jouer,i.e. au tour suivant
    // ALGO QUI LISTE LES COUPS POSSIBLES

    for (int abscisse = 0; abscisse < MAX_LARGEUR; abscisse++)
    { // Pour chaque ligne on va chercher dans chaque colonne
        int ordonnee = 0;

        while (ordonnee < MAX_HAUTEUR && jeu.plateau()[ordonnee][abscisse] != '0')
        {               // Tant que la case n'est pas jouable et la colonne n'est pas pleine
            ordonnee++; // On cherche à la case plus haute
        }

        if (ordonnee < MAX_HAUTEUR)
        { // On a une case jouable qui est dans les limites du jeu
            // On cherche tous les coups valide à partir de cette case vide
            b_candidate.setAllCoord(abscisse, ordonnee, abscisse, ordonnee + 1); // Brix verticale de bottom 'X'
            if (jeu.coup_licite(b_candidate, tour))                              // Le coup est valide
            {
                coupValide->push_back(b_candidate); // On ajoute notre coup à la liste des coups jouables
            }

            b_candidate.setAllCoord(abscisse, ordonnee + 1, abscisse, ordonnee); // Brix verticale de bottom est 'O'
            if (jeu.coup_licite(b_candidate, tour))
            {
                coupValide->push_back(b_candidate);
            }

            b_candidate.setAllCoord(abscisse, ordonnee, abscisse + 1, ordonnee); // Brix horizontale commençant par 'X'
            if (jeu.coup_licite(b_candidate, tour))
            {
                coupValide->push_back(b_candidate);
            }

            b_candidate.setAllCoord(abscisse + 1, ordonnee, abscisse, ordonnee); // Brix terminant commençant par 'X'
            if (jeu.coup_licite(b_candidate, tour))
            {
                coupValide->push_back(b_candidate);
            }
        }
        // Fin des Brix valides à cette abscisse et cette ordonnee, on passe à l'abscisse suivante
    }
    // Affichage de la liste des coups valides pour vérification
    /*
    for (auto b : *coupValide)
    {
        std::cout << b << " | ";
    }
    std::cout << std::endl;
    */
    return coupValide;
}

coup_select Joueur_AlphaBeta::alphabeta(Jeu const &jeu, int alpha, int beta, std::chrono::high_resolution_clock::time_point &start, unsigned int profondeur_max, unsigned int profondeur, Brix &coupAJouer)
{
    // std::cout << "profondeur : " << profondeur << std::endl;
    // algorithme alpha beta de base

    /***
     *
     * TODO
     * récupérer le coup à jouer
     * idée : variable meilleur_coup
     *
     * Ensuite tester
     *
     ***/

    // On est sur une feuille
    if (profondeur == profondeur_max)
    {
        auto jeuPointeur = std::make_shared<Jeu>(jeu);
        info_coup scoreCoup(coupAJouer, jeuPointeur, (this->joueur() ? 'o' : 'x'), 0);
        int score = scoreCoup.eval();

        // std::cout << "prof max atteinte, eval : " << score << std::endl;

        return {coupAJouer, score}; // coup_select(coup, score);
    }

    // Vérification qu'il nous reste du temps pour jouer
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() > (TEMPS_POUR_UN_COUP - 2))
    {
        std::cerr << "timeout" << std::endl;
        return alphabeta(jeu, alpha, beta, start, PROF_MAX, PROF_MAX, coupAJouer); // On évalue la feuille actuelle car manque de temps pour explorer
    }

    // On doit faire un appel récursif pour continuer
    auto coups_valides = std::move(rechercheCoupValide(jeu)); // Liste des coups valides
    bool NoeudMin = false;                                    // Le noeud est-il un noeud min ou max?
    if (profondeur % 2 == 0)
        NoeudMin = true;

    coup_select select; // Pour le moment aucun coups n'est joué, donc la valeur actuelle de la partie est nulle
    select.score = 0;
    // Pour chaque coup possible, on explore l'arbre
    // std::cout << "Taille coups valides : " << coups_valides->size() << std::endl;

    int i = 0;
    int taille_tab = coups_valides->size();
    if (NoeudMin)
    {

        beta = PLUS_INFINI;
        coup_select mini;
        mini.score = beta;

        for (auto coups : *coups_valides)
        {
            i++;
            // std::cout << "minimise (" << i << "/" << taille_tab << ") a : " << alpha << ", b : " << beta << std::endl;
            // std::cout << "coup : " << coups << std::endl;

            Jeu jeuBis = jeu;
            jeuBis.joue(coups);
            auto jeuPointeur = std::make_shared<Jeu>(jeuBis);

            select = alphabeta(jeuBis, alpha, beta, start, PROF_MAX, profondeur + 1, coups);
            if (select.score < mini.score)
            {
                mini.coup = coups;
                mini.score = select.score;
                beta = mini.score;

                if (beta <= alpha)
                {
                    // std::cout << "dans min, beta (" << beta << ") <= alpha (" << alpha << ") return " << select.coup << std::endl;
                    return mini;
                }
            }
        }

        // std::cout << "return " << mini.coup << std::endl;
        return mini;
    }
    else
    {

        alpha = MOINS_INFINI;
        coup_select maxi;
        maxi.score = alpha;

        for (auto coups : *coups_valides)
        {
            i++;
            // std::cout << "maximise (" << i << "/" << taille_tab << ") a : " << alpha << ", b : " << beta << std::endl;
            // std::cout << "coup : " << coups << std::endl;

            Jeu jeuBis = jeu;
            jeuBis.joue(coups);
            auto jeuPointeur = std::make_shared<Jeu>(jeuBis);

            select = alphabeta(jeuBis, alpha, beta, start, PROF_MAX, profondeur + 1, coups);
            if (select.score > maxi.score)
            {
                maxi.score = select.score;
                maxi.coup = coups; 
                alpha = maxi.score;

                if (beta <= alpha)
                {
                    // std::cout << "dans max, beta (" << beta << ") <= alpha (" << alpha << ") return " << select.coup << std::endl;
                    return maxi;
                }
            }
        }

        // std::cout << "return " << maxi.coup << std::endl;
        return maxi;
    }

    // for (auto coups : *coups_valides)
    // {
    //     std::cout << "i : " << i << std::endl;
    //     i++;
    //     Jeu jeuBis = jeu;
    //     jeuBis.joue(coups);
    //     auto jeuPointeur = std::make_shared<Jeu>(jeuBis);

    //     select = alphabeta(jeuBis, alpha, beta, start, PROF_MAX, profondeur + 1, coupAJouer);
    //     if (NoeudMin)
    //     {
    //         if (alpha > select.score)
    //             alpha = select.score;
    //         if (alpha <= beta)
    //         {
    //             if (profondeur == 0)
    //                 coupAJouer = coups; // C'est le premier coup
    //             info_coup scoreCoup(coupAJouer, jeuPointeur, this->joueur() ? 'o' : 'x', 0);
    //             select.coup = coupAJouer;
    //             select.score = scoreCoup.eval();
    //             return select;
    //         }
    //     }
    //     else
    //     {
    //         if (beta < select.score)
    //             beta = select.score;
    //         if (beta >= select.score)
    //         {
    //             if (profondeur == 0)
    //                 coupAJouer = coups; // C'est le premier coup
    //             info_coup scoreCoup(coupAJouer, jeuPointeur, (this->joueur() ? 'o' : 'x'), 0);
    //             select.coup = coupAJouer;
    //             select.score = scoreCoup.eval();
    //             return select;
    //         }
    //     }
    // }
    // // TODO:
    // return select;
}

void Joueur_AlphaBeta::recherche_coup(Jeu jeu, Brix &coup)
{
    // std::cout << "Entrée dans recherche" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    unsigned int profondeur = 0;
    int alpha = MOINS_INFINI;
    int beta = PLUS_INFINI;
    // std::cout << "Création coup" << std::endl;
    Brix coupAJouer;
    // std::cout << "test" << std::endl;
    coup = alphabeta(jeu, alpha, beta, start, PROF_MAX, profondeur, coupAJouer).coup;
    // std::cout << "Coup JOUÉ :" << ret << std::endl;
    // coup.setAllCoord(ret.getAx(), ret.getOx(), ret.getAo(), ret.getOo());
}
