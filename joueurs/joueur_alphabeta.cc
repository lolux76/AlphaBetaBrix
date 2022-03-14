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

std::unique_ptr<std::vector<Brix>> Joueur_AlphaBeta::rechercheCoupValide(JeuBinaire &plateauBinaire)
{

    // VARIABLES LOCALES
    std::unique_ptr<std::vector<Brix>> coupValide = std::make_unique<std::vector<Brix>>();
    
    Brix b_candidate;
    int tour = plateauBinaire.nbCoupJoue() + 1; // la b_candidate devra être valide au tour auquel on va la jouer,i.e. au tour suivant
    // ALGO QUI LISTE LES COUPS POSSIBLES

    for (int abscisse = 0; abscisse < MAX_LARGEUR; abscisse++)
    { // Pour chaque ligne on va chercher dans chaque colonne
        int ordonnee = 0;

        while (ordonnee < MAX_HAUTEUR && plateauBinaire.plateau()[ordonnee][abscisse] != '0')
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

coup_select Joueur_AlphaBeta::alphabeta(JeuBinaire &plateauBinaire, int &alpha, int &beta, std::chrono::high_resolution_clock::time_point &start, unsigned int &profondeur_max, unsigned int profondeur, Brix &coupAJouer)
{
    // algorithme alpha beta de base

    // Vérification qu'il nous reste du temps pour jouer
    if (std::chrono::high_resolution_clock::now() - start > std::chrono::milliseconds(TEMPS_POUR_UN_COUP - 9))
    {
        start -= std::chrono::milliseconds(50000);
        return alphabeta(plateauBinaire, alpha, beta, start, profondeur, profondeur, coupAJouer); // On évalue la feuille actuelle car manque de temps pour explorer
    }

    // On est sur une feuille
    if (profondeur == profondeur_max)
    {
        auto jeuPointeur = std::make_shared<JeuBinaire>(plateauBinaire);
        info_coup scoreCoup(coupAJouer, jeuPointeur, (this->joueur() ? 'o' : 'x'), 0);
        return {coupAJouer,scoreCoup.eval()}; // coup_select(coup, score);
    }
    // On doit faire un appel récursif pour continuer
    auto coups_valides = std::move(rechercheCoupValide(plateauBinaire)); // Liste des coups valides
    bool NoeudMin = false;                                    // Le noeud est-il un noeud min ou max?
    if (profondeur % 2 == 0)

        NoeudMin = true;

    coup_select select; // Pour le moment aucun coups n'est joué, donc la valeur actuelle de la partie est nulle
    select.score = 0;
    // Pour chaque coup possible, on explore l'arbre
    for (auto coups : *coups_valides)
    {
        auto jeuBis = plateauBinaire;
        jeuBis.joue(coups);
        auto jeuPointeur = std::make_shared<Jeu>(jeuBis);

        select = alphabeta(plateauBinaire, alpha, beta, start, profondeur_max, profondeur + 1, coupAJouer);
        if (NoeudMin)
        {
            if (alpha > select.score)
                alpha = select.score;
            if (alpha <= beta)
            {
                if (profondeur == 0){
                    std::cout << "profondeur 0" << std::endl;
                    coupAJouer = coups; // C'est le premier coup
                }
                info_coup scoreCoup(coupAJouer, jeuPointeur, this->joueur() ? 'o' : 'x', 0);
                select.coup = coupAJouer;
                select.score = scoreCoup.eval();
                return select;
            }
        }
        else
        {
            if (beta < select.score)
                beta = select.score;
            if (beta >= select.score)
            {
                if (profondeur == 0)
                    coupAJouer = coups; // C'est le premier coup
                info_coup scoreCoup(coupAJouer, jeuPointeur, (this->joueur() ? 'o' : 'x'), 0);
                select.coup = coupAJouer;
                select.score = scoreCoup.eval();
                return select;
            }
        }
    }
    
    return select;
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
    auto start = std::chrono::high_resolution_clock::now();
    unsigned int profondeur_max = 3;
    unsigned int profondeur = 0;
    int alpha = PLUS_INFINI;
    int beta = MOINS_INFINI;
    auto plateauBinaire = initialiserBitset(jeu);
    //plateauBinaireAffiche(std::cout, plateauBinaire);
    Brix coupAJouer;
    alphabeta(plateauBinaire, alpha, beta, start, profondeur_max, profondeur, coupAJouer);
    coup.setAllCoord(coupAJouer.getAx(), coupAJouer.getOx(), coupAJouer.getAo(), coupAJouer.getOo());
}
