#ifndef __SET_H__
#define __SET_H__


#include <iostream>
#include "capd/capdlib.h"
#include "../Algebra/algebra.h"
#include <vector>
struct Indexer{
    std::vector<std::pair<int,int> > pairs;
    std::pair<Algebra::SeriesVector, Algebra::SeriesVector > splitVector (Algebra::SeriesVector& x);
    int size();
    capd::IVector getIVector(Algebra::SeriesVector& x);
    void intersectRepresentations(capd::InclRect2Set& mainModes,Algebra::SeriesVector& x);
    void makeConsistent(capd::InclRect2Set& mainModes,Algebra::SeriesVector& x);
};
struct Set{
    Algebra::SeriesVector vector;
    capd::InclRect2Set mainModes;
    Set(Algebra::SeriesVector vector,capd::InclRect2Set mainModes);
    void intersectRepresentations(Indexer indexer);
    void makeConsistent(Indexer indexer);
    capd::interval getCurrentTime();
};


#endif 