#pragma once

#include "joueur.hh"
#include <vector>
#include "../arbitre.hh"
#include <memory>
#include <chrono>
#include "info_coup.hh"
#include "jeuBinaire.hh"

#define PLUS_INFINI std::numeric_limits<int>::max()  //+ infini, NE JAMAIS FAIRE PLUS_INFINI + QQCHOSE (dépassement de bits)
#define MOINS_INFINI std::numeric_limits<int>::min() //- infini, NE JAMAIS FAIRE MOINS_INFINI - QQCHOSE (dépassement de bits)


struct coup_select
{
  Brix coup;
  int score;
};

class Joueur_AlphaBeta : public Joueur
{
public:
  Joueur_AlphaBeta(std::string nom, bool joueur);
  // char nom_abbrege() const override;

  static std::unique_ptr<std::vector<Brix>> rechercheCoupValide(JeuBinaire &plateauBinaire);
  coup_select alphabeta(JeuBinaire &plateauBinaire, int &alpha, int &beta, std::chrono::high_resolution_clock::time_point &start, unsigned int &profondeur_max, unsigned int profondeur, Brix &coupAJouer); // Algorithme alpha beta de base

  void recherche_coup(Jeu jeu, Brix &coup) override;
};
