#pragma once

#include "joueur.hh"
#include <vector>
#include "../arbitre.hh"
#include <memory>
#include <chrono>
#include "info_coup.hh"

#define PLUS_INFINI std::numeric_limits<int>::max()  //+ infini, NE JAMAIS FAIRE PLUS_INFINI + QQCHOSE (dépassement de bits)
#define MOINS_INFINI std::numeric_limits<int>::min() //- infini, NE JAMAIS FAIRE MOINS_INFINI - QQCHOSE (dépassement de bits)
#define PROF_MAX 2

struct coup_select
{
  int score;
  bool timeout;
};

class Joueur_AlphaBeta : public Joueur
{
public:
  Joueur_AlphaBeta(std::string nom, bool joueur);
  // char nom_abbrege() const override;

  static std::unique_ptr<std::vector<Brix>> rechercheCoupValide(Jeu const &jeu);

  coup_select alphabeta(Jeu const &jeu, int alpha, int beta, std::chrono::high_resolution_clock::time_point const &start, unsigned int profondeur_max, unsigned int profondeur); // Algorithme alpha beta de base

  Brix alphabetaThreadCallback(Jeu const &jeu, std::chrono::high_resolution_clock::time_point const &start, unsigned int profondeur_max);
  Brix alphabetaCallback(Jeu const &jeu, std::chrono::high_resolution_clock::time_point const &start, unsigned int profondeur_max);

  int alphaExtractValThread(Jeu const &jeu, int alpha, int beta, std::chrono::high_resolution_clock::time_point const &start, unsigned int profondeur);
  coup_select alphaExtractVal(Jeu const &jeu, int alpha, int beta, std::chrono::high_resolution_clock::time_point const &start, unsigned int profondeur);

  void recherche_coup(Jeu jeu, Brix &coup) override;

  bool inline temps_ecoule(std::chrono::high_resolution_clock::time_point const &start, short unsigned int TEMPS_POUR_UN_COUP) const
  {
    return ((std::chrono::high_resolution_clock::now() - start) > std::chrono::microseconds(10000 - 500));
  }

  bool inline partie_terminee(Jeu const &j, int &score)
  {
    if (j.fini())
    {
      // std::cout << "jeu fini" << std::endl;
      // std::cout << "joueur " << this->joueur() << std::endl;
      // std::cout << "partie X " << j.partie_X() << std::endl;
      
       if ((this->joueur() && j.partie_X()) || (!this->joueur() && j.partie_O()))
      {
        score = std::numeric_limits<int>::max();
      }
      else
      {
        score = std::numeric_limits<int>::min();
      }
      return true;
    }
    return false;
  }
};
