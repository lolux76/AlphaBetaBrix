#include "joueur_alphabeta.hh"
#include <memory>

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
    std::bitset<8 * 44 * 2> plateauBinaire; // Plateau convertis en binaire, si X -> bit à 10, si O -> bit à 01, sinon bit à 00

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
            int i;
            unsigned int ligneNumero = 0;
            b_candidate.setAllCoord(abscisse, ordonnee, abscisse, ordonnee + 1); // Brix verticale de bottom 'X'
            if (jeu.coup_licite(b_candidate, tour))                              // Le coup est valide
            {
                     plateauBinaire[i * (MAX_HAUTEUR - ligneNumero) * 2 * ligneNumero] = 1;
                coupValide->push_back(b_candidate); // On ajoute notre coup à la liste des coups jouables
            }

            b_candidate.setAllCoord(abscisse, ordonnee + 1, abscisse, ordonnee); // Brix verticale de bottom est 'O'
            if (jeu.coup_licite(b_candidate, tour))
            {
                    plateauBinaire[i * (MAX_HAUTEUR - ligneNumero) * 2 * ligneNumero + 1] = 1;
                coupValide->push_back(b_candidate);
            }

            b_candidate.setAllCoord(abscisse, ordonnee, abscisse + 1, ordonnee); // Brix horizontale commençant par 'X'
            if (jeu.coup_licite(b_candidate, tour))
            {
                    plateauBinaire[i * (MAX_HAUTEUR - ligneNumero) * 2 * ligneNumero] = 1;
                coupValide->push_back(b_candidate);
            }

            b_candidate.setAllCoord(abscisse + 1, ordonnee, abscisse, ordonnee); // Brix terminant commençant par 'X'
            if (jeu.coup_licite(b_candidate, tour))
            {
                    plateauBinaire[i * (MAX_HAUTEUR - ligneNumero) * 2 * ligneNumero + 1] = 1;
                coupValide->push_back(b_candidate);
            }
            i++;
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

int Joueur_AlphaBeta::alphabeta(Jeu &jeu, int &alpha, int &beta, std::chrono::high_resolution_clock::time_point &start, unsigned int &profondeur_max, unsigned int profondeur, Brix &coupAJouer)
{
    // algorithme alpha beta de base

    // Vérification qu'il nous reste du temps pour jouer
    if (std::chrono::high_resolution_clock::now() - start > std::chrono::milliseconds(TEMPS_POUR_UN_COUP - 9))
    {
        start -= std::chrono::milliseconds(50000);
        return alphabeta(jeu, alpha, beta, start, profondeur, profondeur, coupAJouer); // On évalue la feuille actuelle car manque de temps pour explorer
    }

    // On est sur une feuille
    if (profondeur == profondeur_max)
    {
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() << " microsecondes" << std::endl;
        return rand() % 200 + 1 - 100;
    }
    // On doit faire un appel récursif pour continuer
    auto coups_valides = std::move(rechercheCoupValide(jeu)); // Liste des coups valides
    bool NoeudMin = false;                                    // Le noeud est-il un noeud min ou max?
    if (profondeur % 2 == 0)

        NoeudMin = true;

    int score = 0; // Pour le moment aucun coups n'est joué, donc la valeur actuelle de la partie est nulle

    // Pour chaque coup possible, on explore l'arbre
    for (auto coups : *coups_valides)
    {
        Jeu jeuBis = jeu;
        jeuBis.joue(coups);
        score = alphabeta(jeuBis, alpha, beta, start, profondeur_max, profondeur + 1, coupAJouer);
        if (NoeudMin)
        {
            if (alpha > score)
                alpha = score;
            if (alpha <= beta)
            {
                if (profondeur == 0)
                    coupAJouer = coups; // C'est le premier coup, celui qu'il faudra jouer
                return score;
            }
        }
        else
        {
            if (beta < score)
                beta = score;
            if (beta >= score)
            {
                if (profondeur == 0)
                    coupAJouer = coups; // C'est le premier coup, celui qu'il faudra jouer
                return score;
            }
        }
    }
    // TODO:
    return score;
}

std::bitset<8 * 44 * 2> Joueur_AlphaBeta::initialiserBitset(Jeu jeu)
{
    std::bitset<8 * 44 * 2> plateauBinaire; // Plateau convertis en binaire, si X -> bit à 10, si O -> bit à 01, sinon bit à 00
    for (int i = MAX_HAUTEUR; i >= 0; i--)
    {
        auto ligne = jeu.plateau()[i];
        unsigned int ligneNumero = 0;
        for (auto colonne : ligne)
        {
            if (colonne == 'o'){
                plateauBinaire[i * 2 + ligneNumero * MAX_HAUTEUR + 1] = 1;
                std::cout << "01";
            }
            else if (colonne == 'x')
            {
                plateauBinaire[i * 2 + ligneNumero * MAX_HAUTEUR] = 1;
                std::cout << "10";
            }
            else{
                std::cout << " | ";
            }
            ligneNumero++;
        }
        std::cout << std::endl;
    }
    return plateauBinaire;
}

/*
std::ostream& plateauBinaireAffiche( std::ostream &flux, std::bitset<8 * 44 * 2> const& jeu ){
//void Jeu::afficher( std::ostream &flux) const {
    int j;
    for(int i=MAX_HAUTEUR -1; i>=0; i--){
            flux<<'|';
            for (int j=0; j < 8; j+= 2){
                std::cout << jeu[i * 2 + j * MAX_HAUTEUR] << jeu[i * 2 + j * MAX_HAUTEUR + 1];
                    flux<< " |";
            flux<<"  "<<i<<std::endl;
            }
        }
    flux<<std::endl << '|';
    for (j=0 ; j<MAX_LARGEUR ; j++) {
            flux << j << '|';
        }
    flux << std::endl;
    return flux;
}
*/

void Joueur_AlphaBeta::recherche_coup(Jeu jeu, Brix &coup)
{
    std::cout << "Entrée dans recherche" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    unsigned int profondeur_max = 3;
    unsigned int profondeur = 0;
    int alpha = PLUS_INFINI;
    int beta = MOINS_INFINI;
    auto plateauBinaire = initialiserBitset(jeu);
    //plateauBinaireAffiche(std::cout, plateauBinaire);
    Brix coupAJouer;
    alphabeta(jeu, alpha, beta, start, profondeur_max, profondeur, coupAJouer);
    std::cout << "Coup :" << coupAJouer << std::endl;
    coup.setAllCoord(coupAJouer.getAx(), coupAJouer.getOx(), coupAJouer.getAo(), coupAJouer.getOo());
}
