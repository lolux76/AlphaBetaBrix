#include "joueur_alphabeta.hh"
#include <memory>
#include <iostream>

#include <thread>
#include <future>

Joueur_AlphaBeta::Joueur_AlphaBeta(std::string nom, bool joueur)
    : Joueur(nom, joueur)
{
}

/*char Joueur_AlphaBeta::nom_abbrege() const
{
    return 'A';
}
*/

std::unique_ptr<std::vector<Brix>> Joueur_AlphaBeta::rechercheCoupValide(Jeu const &jeu)
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

int Joueur_AlphaBeta::alphabeta(Jeu const &jeu, int alpha, int beta, std::chrono::high_resolution_clock::time_point const &start, unsigned int profondeur_max, unsigned int profondeur, Brix const &coupAJouer)
{
    // std::cout << "profondeur : " << profondeur << std::endl;
    // algorithme alpha beta de base

    // On est sur une feuille
    if (profondeur == profondeur_max)
    {
        info_coup scoreCoup(coupAJouer, std::make_shared<Jeu>(jeu), (this->joueur() ? 'o' : 'x'), 0);
        // std::cout << "prof max atteinte, eval : " << score << std::endl;

        return scoreCoup.eval(); // coup_select(coup, score);
    }

    // Vérification qu'il nous reste du temps pour jouer
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;

    if (temps_ecoule(start, TEMPS_POUR_UN_COUP))
    {
        std::cerr << "timeout" << std::endl;
        return alphabeta(jeu, alpha, beta, start, PROF_MAX, PROF_MAX, coupAJouer); // On évalue la feuille actuelle car manque de temps pour explorer
    }

    // On doit faire un appel récursif pour continuer
    auto coups_valides = std::move(rechercheCoupValide(jeu)); // Liste des coups valides

    bool NoeudMin = (profondeur % 2) == 1;

    int select; // Pour le moment aucun coups n'est joué, donc la valeur actuelle de la partie est nulle
    // Pour chaque coup possible, on explore l'arbre

    if (NoeudMin)
    {
        beta = PLUS_INFINI;

        for (auto coups : *coups_valides)
        {
            // i++;
            // std::cout << "minimise (" << i << "/" << taille_tab << ") a : " << alpha << ", b : " << beta << std::endl;
            // std::cout << "coup : " << coups << std::endl;

            Jeu jeuBis = jeu;
            jeuBis.joue(coups);

            select = alphabeta(jeuBis, alpha, beta, start, PROF_MAX, profondeur + 1, coups);

            if (select < beta)
            {
                beta = select;

                if (beta <= alpha)
                {
                    break;
                }
            }
        }
        return beta;
        // std::cout << "return " << mini.coup << std::endl;
    }
    else
    {

        alpha = MOINS_INFINI;
        for (auto coups : *coups_valides)
        {
            // i++;
            // std::cout << "maximise (" << i << "/" << taille_tab << ") a : " << alpha << ", b : " << beta << std::endl;
            // std::cout << "coup : " << coups << std::endl;

            Jeu jeuBis = jeu;
            jeuBis.joue(coups);

            select = alphabeta(jeuBis, alpha, beta, start, PROF_MAX, profondeur + 1, coups);
            if (select > alpha)
            {
                alpha = select;

                if (beta <= alpha)
                {
                    // std::cout << "dans max, beta (" << beta << ") <= alpha (" << alpha << ") return " << select.coup << std::endl;
                    break;
                }
            }
        }
        return alpha;
        // std::cout << "return " << maxi.coup << std::endl;
    }
}

// Le threading concède l'élagage sur la première couche, et n'est donc pas rentable. Il fallait essayer....
Brix Joueur_AlphaBeta::alphabetaThreadCallback(Jeu const &jeu, std::chrono::high_resolution_clock::time_point const &start, unsigned int profondeur_max)
{
    int alpha = MOINS_INFINI;
    int beta = PLUS_INFINI;
    int maxi;
    int select;
    maxi = alpha;
    int i_max = -1;

    std::unique_ptr<std::vector<Brix>> coups_valides = std::move(rechercheCoupValide(jeu)); // Liste des coups valides
    std::future<int> *res_coups = new std::future<int>[coups_valides->size()];

    for (long unsigned int i = 0; i < coups_valides->size(); i++)
    {
        Jeu jeuBis = jeu;
        jeuBis.joue(coups_valides->at(i));
        res_coups[i] = std::async(std::launch::async, &Joueur_AlphaBeta::alphabeta, this, jeuBis, alpha, beta, start, profondeur_max, 1, coups_valides->at(i));
    }
    // std::cout << "coups préparés" << std::endl;
    for (long unsigned int i = 0; i < coups_valides->size(); i++)
    {
        select = res_coups[i].get();

        if (select > maxi)
        {
            maxi = select;
            i_max = i;
        }
    }
    delete[] res_coups;
    return coups_valides->at(i_max);
}

Brix Joueur_AlphaBeta::alphabetaCallback(Jeu const &jeu, std::chrono::high_resolution_clock::time_point const &start, unsigned int profondeur_max)
{
    // On doit faire un appel récursif pour continuer
    auto coups_valides = std::move(rechercheCoupValide(jeu)); // Liste des coups valides

    int select;

    int alpha = MOINS_INFINI;
    int beta = PLUS_INFINI;

    int maxi = alpha;
    int i_maxi = -1;
    for (unsigned long i = 0; i < coups_valides->size(); i++)
    {
        // i++;
        // std::cout << "maximise (" << i << "/" << taille_tab << ") a : " << alpha << ", b : " << beta << std::endl;
        // std::cout << "coup : " << coups << std::endl;

        Jeu jeuBis = jeu;
        jeuBis.joue(coups_valides->at(i));

        select = alphabeta(jeuBis, alpha, beta, start, PROF_MAX, 1, coups_valides->at(i));
        if (select > maxi)
        {
            maxi = select;
            alpha = select;
            i_maxi = i;
        }
    }
    return coups_valides->at(i_maxi);
    // std::cout << "return " << maxi.coup << std::endl;
}

void Joueur_AlphaBeta::recherche_coup(Jeu jeu, Brix &coup)
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    // std::cout << "Création coup" << std::endl;
    // std::cout << "test" << std::endl;
    coup = alphabetaCallback(jeu, start, PROF_MAX);
    // std::cout << "Coup JOUÉ :" << ret << std::endl;
    // coup.setAllCoord(ret.getAx(), ret.getOx(), ret.getAo(), ret.getOo());
}
