#ifndef ELECTRON_PRESENCE_H
#define ELECTRON_PRESENCE_H

#include <iostream>
#include <cmath>

class Electron_presence
{

public:
   Electron_presence(unsigned int const mode_number, std::size_t const energy_level_number,
                     unsigned int const *laser_levels, double const q);

void affiche(std::ostream &flux) const;

//void initialise_size(std::size_t const mode_number, std::size_t const energy_level_number);

~Electron_presence();

unsigned int getMode_number() const;

unsigned int getEnergy_level_number() const;

double **getProbability() const;

private:
double **probability;
unsigned int mode_number;
unsigned int energy_level_number;

   void initialise_occuancies(unsigned int const *laser_levels, double const q) ;


};

#endif // ELECTRON_PRESENCE_H
