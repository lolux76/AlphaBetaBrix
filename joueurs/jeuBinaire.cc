#include "jeuBinaire.hh"

JeuBinaire::JeuBinaire(Jeu jeu): nb_tours(jeu.nbCoupJoue()){
    this->plateauBinaire = initialiserBitset(jeu);
}

JeuBinaireBitset JeuBinaire::initialiserBitset(Jeu &jeu){
    JeuBinaireBitset plateauBinaire; // Plateau convertis en binaire, si XO -> bit à 10, si OX -> bit à 01, sinon bit à 00
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

int JeuBinaire::nbCoupJoue() const{
    return this->nb_tours;
}

bool JeuBinaire::coordValide(Brix const & coup) const //La piece est dans le plateau
{
    return ((coup.getAx() >= 0) && (coup.getAx() < MAX_LARGEUR)
         && (coup.getAo() >= 0) && (coup.getAo() < MAX_LARGEUR)
         && (coup.getOo() >= 0) && (coup.getOo() < MAX_LARGEUR)
         && (coup.getOx() >= 0) && (coup.getOx() < MAX_LARGEUR));
}

bool JeuBinaire::voisin(Brix const & coup) const{
    int min;
    if (coup.horizontale())
        {if (coup.getAx()<coup.getAo())

                {if ((coup.getAx() == 0) && plateauBinaire[coup.getOx()][2]== '0')
                        return false;
                    else if ((coup.getAo()== MAX_LARGEUR -1) && _plateau[coup.getOx()][MAX_LARGEUR - 3]== '0')
                        return false;
                    else return (_plateau[coup.getOx()][coup.getAx()-1] !='0') || (_plateau[coup.getOx()][coup.getAo()+1] !='0');
                }
            else {if ((coup.getAo() == 0) && _plateau[coup.getOx()][2]== '0')
                        return false;
                    else if ((coup.getAx()== MAX_LARGEUR -1) && _plateau[coup.getOx()][MAX_LARGEUR - 3]== '0')
                        return false;
                    return (_plateau[coup.getOx()][coup.getAo()-1] !='0') || (_plateau[coup.getOx()][coup.getAx()+1] !='0');
                };
        }
    else if (b.verticale())
        {   min=b.getOo()<b.getOx()?b.getOo():b.getOx();
            if ((b.getAx() == 0) && _plateau[min][1]== '0')
                return false;
            else if ((b.getAx()== MAX_LARGEUR -1) && _plateau[min][MAX_LARGEUR - 2]== '0')
                return false;
            else return (_plateau[min][b.getAx() -1] !='0') || (_plateau[min][b.getAx() +1] !='0');
        }
    else return false;
    return true;
}

bool JeuBinaire::coup_licite(Brix const & coup, int nb) const{
    if (!coup.getDefinie()
          ||  !coordValide(coup)
          || !coup.bienformee()
          || (plateauBinaire[coup.getOo() * 2 + coup.getAo() * MAX_HAUTEUR]== 0 && plateauBinaire[coup.getOx() * 2 + coup.getAx() * MAX_HAUTEUR + 1] == 1)
          || (plateauBinaire[coup.getOx() * 2 + coup.getAx() * MAX_HAUTEUR] == 1 && plateauBinaire[coup.getOo() * 2 + coup.getAo() * MAX_HAUTEUR + 1] == 0))
        return false;
    if  (coup.verticale())
        {
            int min ;
            min=coup.getOo()<coup.getOx()?coup.getOo():coup.getOx();
            if (min != 0 && plateauBinaire[(min-1) * 2 + coup.getAo() * MAX_HAUTEUR] == 0 && plateauBinaire[(min-1) * 2 + coup.getAx() * MAX_HAUTEUR + 1] == 1)
                return false;
        };
    if (coup.horizontale())
        {
        if ((coup.getOx()!=0 && plateauBinaire[(coup.getOx()-1) * 2 + coup.getAx() * MAX_HAUTEUR] == 0 && plateauBinaire[(coup.getOo() -1) * 2 + coup.getAo() * MAX_HAUTEUR + 1] == 1) ||
                (coup.getOo()!=0 && plateauBinaire[(coup.getOo()-1) * 2 + coup.getAo() * MAX_HAUTEUR] == 0) && plateauBinaire[(coup.getOx() - 1) * 2 + coup.getAx() + 1] == 0)
        return false;
        };
    if ((nb !=1) && (coup.getOx() == 0 || coup.getOo() == 0) && !voisin(coup))
        return false;
    return true;
}