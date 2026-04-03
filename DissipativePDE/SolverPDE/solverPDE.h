#ifndef __SOLVERPDE_H__
#define __SOLVERPDE_H__
#include "../Algebra/algebra.h"
#include "../Set/set.h"
#include "../VectorField/vectorField.h"
#include <vector>
#include <memory>

struct Encloser{
    Algebra::SeriesVector enclosureExtent; 
    Algebra::SeriesVector enclosurePointWiseExtent; 
    capd::interval validatedTimeStep;

    // Configurable parameters for the enclosure algorithm
    int maxIterations = 100;
    double minDt = 1e-50;
    capd::interval initialEpsRange = capd::interval(-0.01, 0.01);
    capd::interval enclosureExpansionFactor = capd::interval(-0.05, 1.1);

    void enclose(
                Set& x,
                VectorField& vectorField,
                capd::interval dt,
                int refineNum,
                bool constStep, 
                bool comPointWiseEnclose);
};

struct Mover{

    capd::interval step;
    Encloser encloser;
    capd::IMap perturbMap;
	std::unique_ptr<capd::IMultiMap> multiMap;
	std::unique_ptr<capd::CWDiffInclSolver> solver;
    capd::interval maxDecay;
    capd::interval minDecay;
    Mover(VectorField& vectorField,Encloser encloser);
    void setStep(capd::interval step);
    void move(Set& x,VectorField& vectorField,bool constStep);
    void static perturb(capd::autodiff::Node t, capd::autodiff::Node in[], int dimIn, 
                        capd::autodiff::Node out[], int dimOut, capd::autodiff::Node params[], int noParams);
};
#endif