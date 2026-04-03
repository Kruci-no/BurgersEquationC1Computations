#include "burgersVecField.h"
#include "../GallerkinProjections/gallerkinProjections.h"
#include <iostream>
using namespace std;
using namespace Algebra;
using namespace capd;

SeriesVector linear(int size,ParamsMap params){
    SeriesVector L(1);
    Series xx(1,-2*params["alfa"],SeriesType::sin);
    xx = xx.resize(size);
    L[0]= xx *interval(-1); 
    return L;
}
SeriesVector linearC1(int size,ParamsMap params){
    SeriesVector L(2);
    Series xx(1,-2*params["alfa"],SeriesType::sin);
    Series ones(1,0,SeriesType::sin);
    xx = xx.resize(size);
    ones = ones.resize(size);
    L[0]= xx *interval(-1); 
    L[1]= xx *interval(-1); 
    //cout << "Hello"<<endl;
   // L.print();
    
    //cout << "Hello";
    return L;
}
SeriesVector nonlinearity(interval time, SeriesVector x,ParamsMap params){
    SeriesVector y(1);
    Series u_squared_x = (square(x[0]).resize(x[0].mainSize + 1)).seriesDerivative();
    interval sinValue = sin(time*params["omega"]) ;
   
    y[0]=  params["delta"]*u_squared_x ;
    
    y[0].main[0] = y[0].main[0] + params["A1"]*sinValue+ params["B1"] ;
   
    y[0].main[1] = y[0].main[1] + params["A2"]*sinValue+ params["B2"] ;
    return y;
}
SeriesVector nonlinearityC1(interval time, SeriesVector x,ParamsMap params){
    SeriesVector y(2);
    Series u_squared_x = (square(x[0]).resize(x[0].mainSize + 1)).seriesDerivative();
    interval sinValue = sin(time*params["omega"]) ;
    y[0] =  params["delta"]*u_squared_x ;
    y[0].main[0] = y[0].main[0] + params["A1"]*sinValue+ params["B1"] ;
   
    y[0].main[1] = y[0].main[1] + params["A2"]*sinValue+ params["B2"] ;

    y[1]= params["delta"]*interval(2) * (mult(x[0],x[1]).resize(x[1].mainSize + 1)).seriesDerivative();  ;
    return y;
}
IVector rest(interval time,SeriesVector x,Indexer indexer,ParamsMap params){
    SeriesVector y(1);
    auto splited_x = indexer.splitVector(x);
    SeriesVector xMain = splited_x.first ;
    SeriesVector xDiss = splited_x.second;
    Series uSqueredXRest = (mult(xDiss[0],x[0] + xMain[0]).resize(x[0].mainSize + 1 )).seriesDerivative();
    y[0] = uSqueredXRest * params["delta"];
    return indexer.getIVector(y);
}
IVector restC1(interval time,SeriesVector x,Indexer indexer,ParamsMap params){
    auto splited_x = indexer.splitVector(x);
    SeriesVector xMain = splited_x.first ;
    SeriesVector xDiss = splited_x.second;
    SeriesVector y(2);
    Series uSqueredXRest = (mult(xDiss[0],x[0] + xMain[0]).resize(x[0].mainSize + 1 )).seriesDerivative();
    y[0] = uSqueredXRest * params["delta"];
    // (u*h)_x = u_m*h_m + [ u*h_r + u_r*h_m]
    Series uhXRest = (mult(x[0], xDiss[1]).resize(x[0].mainSize + 1 ) + mult(xMain[1],xDiss[0])  ).seriesDerivative();
    y[1] = uhXRest * params["delta"] *2;
    return indexer.getIVector(y);
}
Indexer makeIndexer(int size){
    std::vector<std::pair<int,int> > pairs(size);
    for(int i=0;i<size;i++ ){
        pairs[i].first = 0;
        pairs[i].second = i;
    }
    Indexer indexer;
    indexer.pairs = pairs;
    return indexer;
}
Indexer makeIndexerC1(int size){
    std::vector<std::pair<int,int> > pairs(2*size);
    for(int i=0;i<size;i++ ){
        pairs[i].first = 0;
        pairs[i].second = i;
        pairs[i+size].first = 1;
        pairs[i+size].second = i;
    }
    Indexer indexer;
    indexer.pairs = pairs;
    return indexer;
}
VectorField getVectorField(int size,int serisesSize,ParamsMap params){
    VectorField vectorField;
    vectorField.indexer = makeIndexer(size);
    vectorField.params = params;
    vectorField.F_nonLinearity = nonlinearity;
    vectorField.F_rest = rest;
    vectorField.L = linear(serisesSize,params);
    vectorField.nMain = vectorField.indexer.size();
    vectorField.numOfNormalParams = 7;
    vectorField.f = IMap(gallerkinProjectionVecBurgers(size,true));
    vectorField.f.setParameter("alfa",params["alfa"]);
    vectorField.f.setParameter("omega",params["omega"]);
    vectorField.f.setParameter("delta",params["delta"]);
    vectorField.f.setParameter("A1",params["A1"]);
    vectorField.f.setParameter("B1",params["B1"]);
    vectorField.f.setParameter("A2",params["A2"]);
    vectorField.f.setParameter("B2",params["B2"]);
    return vectorField;

}
VectorField getVectorFieldC1(int size,int serisesSize,ParamsMap params){
    VectorField vectorField;
    vectorField.indexer = makeIndexerC1(size);
    vectorField.params = params;
    vectorField.F_nonLinearity = nonlinearityC1;
    vectorField.F_rest = restC1;
    vectorField.L = linearC1(serisesSize,params);
    vectorField.nMain = vectorField.indexer.size();
    vectorField.numOfNormalParams = 7;
    vectorField.f = IMap(gallerkinProjectionVecBurgersC1(size,true));
    vectorField.f.setParameter("alfa",params["alfa"]);
    vectorField.f.setParameter("omega",params["omega"]);
    vectorField.f.setParameter("delta",params["delta"]);
    vectorField.f.setParameter("A1",params["A1"]);
    vectorField.f.setParameter("B1",params["B1"]);
    vectorField.f.setParameter("A2",params["A2"]);
    vectorField.f.setParameter("B2",params["B2"]);
    return vectorField;
}
/*
VectorField getVectorFieldSymC1(int size,int serisesSize,ParamsMap params){
    VectorField vectorField;
    vectorField.indexer = makeIndexer(size);
    vectorField.params = params;
    vectorField.F_nonLinearity = nonlinearity;
    vectorField.F_rest = rest;
    vectorField.L = linearSym(serisesSize,params);
    vectorField.nMain = vectorField.indexer.size();
    vectorField.numOfNormalParams = 4;
    vectorField.f = IMap(gallerkinProjectionVecFieldSym(size,true));
    vectorField.f.setParameter("lambda",params["lambda"]);
    vectorField.f.setParameter("omega",params["omega"]);
    vectorField.f.setParameter("A",params["A"]);
    vectorField.f.setParameter("B",params["B"]);
    return vectorField;
}
*/