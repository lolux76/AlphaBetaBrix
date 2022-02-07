#include "joueur_alphabeta.hh"
#include <memory>

Joueur_AlphaBeta::Joueur_AlphaBeta(std::string nom, bool joueur)
    :Joueur(nom,joueur)
{}



/*char Joueur_AlphaBeta::nom_abbrege() const
{
    return 'A';
}
*/

std::unique_ptr<std::vector<Brix>> Joueur_AlphaBeta::rechercheCoupValide(Jeu jeu){
    
    //VARIABLES LOCALES
    std::unique_ptr<std::vector<Brix>> coupValide;
    Brix b_candidate;
    int tour = jeu.nbCoupJoue()+1;//la b_candidate devra être valide au tour auquel on va la jouer,i.e. au tour suivant

    //ALGO QUI LISTE LES COUPS POSSIBLES

    for(int abscisse = 0; abscisse < MAX_LARGEUR; abscisse++){ // Pour chaque ligne on va chercher dans chaque colonne
        int ordonnee = 0;

        while(ordonnee < MAX_HAUTEUR && jeu.plateau()[ordonnee][abscisse] != '0'){ //Tant que la case n'est pas jouable et la colonne n'est pas pleine
            ordonnee++; //On cherche à la case plus haute
        }

        if(ordonnee < MAX_HAUTEUR){ //On a une case jouable qui est dans les limites du jeu
            //On cherche tous les coups valide à partir de cette case vide
            b_candidate.setAllCoord(abscisse, ordonnee, abscisse, ordonnee + 1); //Brix verticale de bottom 'X'
            if (jeu.coup_licite(b_candidate, tour))  //Le coup est valide
            {
                coupValide->push_back(b_candidate); //On ajoute notre coup à la liste des coups jouables
            }

            b_candidate.setAllCoord(abscisse,ordonnee+1,abscisse,ordonnee); //Brix verticale de bottom est 'O'
            if (jeu.coup_licite(b_candidate,tour))
            {
                coupValide->push_back(b_candidate);
            }

            b_candidate.setAllCoord(abscisse,ordonnee ,abscisse+1,ordonnee); //Brix horizontale commençant par 'X'
            if (jeu.coup_licite(b_candidate,tour))
            {
                coupValide->push_back(b_candidate);
            }

            b_candidate.setAllCoord(abscisse+1,ordonnee,abscisse,ordonnee); //Brix terminant commençant par 'X'
            if (jeu.coup_licite(b_candidate,tour))
            {
                coupValide->push_back(b_candidate);
            }
        }
        //Fin des Brix valide à cette abscisse et cette ordonnee, on passe à l'abscisse suivante
    }
    //Affichage de la liste des coups valides pour vérification
        for(auto b : *coupValide){
            std::cout << b << " | ";
        }
        std::cout << std::endl;
        
        
        return coupValide;
}

void alphabeta(char noeud, int alpha, int beta){
    //algorithme alpha beta de base

}

void Joueur_AlphaBeta::recherche_coup(Jeu jeu, Brix &coup)
{
    auto coups_valides = std::move(rechercheCoupValide(jeu));
}
