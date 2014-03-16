//Sean M. Law

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <map>
#include <string>

const double PI = 4.0*atan(1.0);

//Reference: CRC Handbook for Chemistry and Physics, 1983/1984
//CHARMM Source: source/ltm/consta_ltm.src
//SI Units

const double ONEKCAL = 4184.0; // 1Kcal = 4184 J
const double NAVO = 6.022045E23; //Avogadro constant, 1/mol
const double KBOLTZ = (1.380662E-23); //Boltzmann constant, J/K
const double AMU = 1.6605655E-27; //Atomic mass unit, kg
const double RGAS = 8.314472; //Molar gas constant, J/K
const double PLANCK = 6.6260693E-34; //Planck constant, J*s
const double SPEEDL = 2.99792458E8; //Speed of light, m/s

//Conversions
const double DEG2RAD = PI/180.0;
const double RAD2DEG = 180.0/PI;
const double FRQ2INVCM = sqrt(ONEKCAL/(NAVO*AMU))/(SPEEDL*1E-8*2*PI); //Convert sqrt eigenvalues to 1/cm
const double PS2SEC = 1E-12; //1 ps = 10E-12 seconds
const double SEC2PS = 1E12; //1 second = 10E12 ps
const double CM2MET = 1E-2; //1 cm = 10E-2 m
const double MET2CM = 1E2; //1 meter = 100 cm

//AKMA Units
const double AKMATPS = 4.88882129E-02; //One AKMA time unit as picoseconds
const double AKMATS = 4.88882129E-14; //One AKMA time unit as seconds
const double kB = KBOLTZ*NAVO/ONEKCAL; //Boltzmann constant, kcal/mol/K
const double Rgas = 8.314472*NAVO/ONEKCAL; //Molar gas constant kcal/mol/K
const double planck = PLANCK*NAVO/ONEKCAL; //Planck constant, (kcal/mol)*s
const double speedl = SPEEDL*MET2CM/SEC2PS; //Speed of light, cm/ps

#endif
