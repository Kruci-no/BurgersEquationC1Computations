#include "set.h"
#include "../../Utils/Debug/debugException.h"
using namespace std;
using namespace capd;
using namespace Algebra;
std::pair<SeriesVector, SeriesVector > Indexer::splitVector(SeriesVector& x) 
{
    SeriesVector x1 = x*interval(0);
    SeriesVector x2 = x;
    for(int i=0;i<this->pairs.size();i++){
            int l = pairs[i].first;
            int r = pairs[i].second; 
            x1[l].main[r] = x2[l].main[r];
            x2[l].main[r] = interval(0);
    }
    return std::pair<SeriesVector, SeriesVector>(x1,x2); 
}

int Indexer::size() 
{
    return pairs.size();    
}

capd::IVector Indexer::getIVector(SeriesVector& x) 
{
    IVector result(size());
        for(int i=0;i<size();i++){
            int l = pairs[i].first;
            int r = pairs[i].second; 
            result[i] = x[l].main[r];
        }
        return result;
}

void Indexer::intersectRepresentations(capd::InclRect2Set& mainModes,SeriesVector& x)
{
    IVector main = (IVector) mainModes;
    for(int i=0;i<size();i++){
        int l = pairs[i].first;
        int r = pairs[i].second; 
        if(!intersection(main[i], x[l].main[r], x[l].main[r]) ){
            std::cout<< "\nAt main modes index: " << i <<"\n";
            std::cout<< "\nAt Vector Series index: " << l << "-" << r<<"\n";
            std::cout<< main<<"\n";
            x.print();
            throw debugException("Set intersectRepresentations - empty intersection\n");
        }
    }
}

void Indexer::makeConsistent(capd::InclRect2Set& mainModes,SeriesVector& x)
{
    IVector main = (IVector) mainModes;
    for(int i=0;i<size();i++){
        int l = pairs[i].first;
        int r = pairs[i].second; 
        x[l].main[r] = main[i];
    }
}

Set::Set(SeriesVector vector,capd::InclRect2Set mainModes) 
:vector(vector),mainModes(mainModes)
{
}

void Set::intersectRepresentations(Indexer indexer)
{
    indexer.intersectRepresentations(mainModes,vector);
}

void Set::makeConsistent(Indexer indexer)
{
    indexer.makeConsistent(mainModes,vector);
}

interval Set::getCurrentTime() 
{
    return mainModes.getCurrentTime();
}