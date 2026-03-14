#ifndef LogisticModelTools_H
#define LogisticModelTools_H
#include <iostream>
#include <vector>
#include "capd/capdlib.h"
#include "../../DissipativePDE/VectorFieldMaker/vectorFieldMaker.h"
std::string gallerkinProjectionVecBurgers(int size, bool addInclusionTerms);
std::string gallerkinProjectionVecBurgersC1(int size,bool addInclusionTerms) ;

#endif
