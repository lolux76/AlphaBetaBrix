#pragma once


#include "joueur.hh"
#include <vector>
#include "../arbitre.hh"

#define PLUS_INFINI = std::numeric_limits<int>::max(); //+ infini, NE JAMAIS FAIRE PLUS_INFINI + QQCHOSE (dépassement de bits)
#define MOINS_INFINI = std::numeric_limits<int>::min(); //- infini, NE JAMAIS FAIRE MOINS_INFINI - QQCHOSE (dépassement de bits)

class Joueur_AlphaBeta : public Joueur
{
public:
  Joueur_AlphaBeta(std::string nom, bool joueur);
  //char nom_abbrege() const override;

  std::vector<Brix> rechercheCoupValide(Jeu jeu);

  void alphabeta(char noeud, int alpha, int beta); //Algorithme alpha beta de base

  void recherche_coup(Jeu jeu, Brix & coup) override;

};
