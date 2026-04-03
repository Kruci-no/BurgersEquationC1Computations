#include "InOut/inOut.h"
#include "capd/capdlib.h"
#include "GallerkinProjections/gallerkinProjections.h"
#include "BurgersVecField/burgersVecField.h"
#include "../DissipativePDE/Algebra/algebra.h"
#include "../DissipativePDE/Set/set.h"
#include "../DissipativePDE/SolverPDE/solverPDE.h"
#include "../Utils/Debug/debugException.h"
#include<iostream>
#include<utility>
#include <chrono>
using namespace std;
using namespace Algebra;
using namespace capd;
std::pair<Series, bool> checkInclusion
    (Series u,int mainSize, int fullSize, DVector paramsDVector){
    ParamsMap params = {{"alfa",paramsDVector[0]},{"omega",2*Interval::pi()},{"delta",paramsDVector[2]} ,{"A1",paramsDVector[3]},{"B1",paramsDVector[4]},{"A2",paramsDVector[5]},{"B2",paramsDVector[6]}};
    SeriesVector vec(1);
    vec[0] = u;
    vec[0] = vec[0].resize(fullSize);
    IVector ivectorforRect2Set(mainSize);
    for(int i=0;i<mainSize;i++){
        
        ivectorforRect2Set[i] = vec[0].main[i];
    }
    //vec[0].print();
    InclRect2Set mainSet(ivectorforRect2Set);
    VectorField Burgers = getVectorField(mainSize,fullSize,params);
    Encloser encloser;
    Mover mover(Burgers,encloser);
    mover.setStep(1./1024);
    mover.minDecay = vec[0].s+3; 
    mover.maxDecay = vec[0].s+6; 
    Set set(vec,mainSet);
    for(int i=0;i<1024;i++){
        mover.move(set,Burgers,true);
    }
   // set.vector[0].print();
    return {set.vector[0], set.vector[0].subset(vec[0])};
        //return false;
}
Series C1Computation(Series u,Series uH,int mainSize, int fullSize ,DVector paramsDVector){
    ParamsMap params = {{"alfa",paramsDVector[0]},{"omega",2*Interval::pi()},{"delta",paramsDVector[2]} ,{"A1",paramsDVector[3]},{"B1",paramsDVector[4]},{"A1",paramsDVector[5]},{"B1",paramsDVector[6]}};
    SeriesVector vec(2);
    vec[0] = u;
    vec[1] = uH; 
    vec[0] = vec[0].resize(fullSize);
    vec[1] = vec[1].resize(fullSize);
    //vec.print();
    VectorField vectorFieldBurgersC1 = getVectorFieldC1(mainSize,fullSize,params);
    Encloser encloser;
    Mover mover(vectorFieldBurgersC1,encloser);
    mover.setStep(1./1024);
    IVector allVariables(2*mainSize);
     for(int i =0 ;i<mainSize;i++){
        allVariables[i]= vec[0].main[i];
        allVariables[i+mainSize]= vec[1].main[i];
    }
   
    InclRect2Set mainSet(allVariables);
    Set set(vec,mainSet);
    for(int i=0;i<1024;i++){
            //Tu jest coś nie tak
            mover.move(set,vectorFieldBurgersC1,true);
        }
    //return set.vector[1];
    //set.vector.print();
    return set.vector[1];
}
DVector findFixedPoint(int dim, DVector params ,DMap f, double eps){
    f.setParameters(params);
    DOdeSolver solver(f,22);
    DTimeMap timeMap(solver);
    DVector initialData(dim);

    DVector prev =  initialData ;
    DVector next;
    int MaxIter = 1000;
    int iter = 0; 
    double err = eps+1;
    do{
        double startTime = 0; double endTime = 1;
        iter = iter +1 ;
        next = prev;
        next = timeMap(endTime,next,startTime);
		err = (next-prev).euclNorm();
        prev = next;
    }while( eps<err  &&iter < MaxIter);
    if(iter == MaxIter){
        throw debugException("Cannot Find fixed Point");
    }
    double startTime = 0; double endTime = 1;
    DMatrix initMatrix = DMatrix::Identity(dim) ;
    DMatrix monodromyMatrix(dim,dim);
    //cout << "finded Fixed Point"<< next <<"\n";
    next = timeMap(endTime,next,initMatrix,monodromyMatrix,startTime);
   // cout <<"Monodromi Matrix:" << monodromyMatrix << "\n";
    DMaxNorm maxNorm;
	//cout <<"maxNorm of monodromyMatrix: " <<maxNorm(monodromyMatrix)<<"\n";
    //cout << "finded Iterred Fixed Point"<< next <<"\n";
    return next;
}
void ComputerAssistedProof(int mainC0Size, int fullC0Size, int mainC1Size, int fullC1Size, double eps, double s_double, DVector paramsDVector){
    InOut inOut;
    DMap Burgers(gallerkinProjectionVecBurgers(mainC0Size,false));

    DVector DmainInitialData = findFixedPoint(mainC0Size,paramsDVector,Burgers,10e-15) ;
    
    ParamsMap params = {{"alfa",paramsDVector[0]},{"omega",2*Interval::pi()},{"delta",paramsDVector[2]} ,{"A1",paramsDVector[3]},{"B1",paramsDVector[4]},{"A1",paramsDVector[5]},{"B1",paramsDVector[6]}};
    cout << "alfa= " << paramsDVector[0]<<"\n";
    cout << "omega= " << paramsDVector[1]<<"\n";
    cout << "delta= " << paramsDVector[2]<<"\n";
    cout << "A1= " << paramsDVector[3]<<"\n";
    cout << "B1= " << paramsDVector[4]<<"\n";
    cout << "A2= " << paramsDVector[5]<<"\n";
    cout << "B2= " << paramsDVector[6]<<"\n";
    interval s = interval(s_double);
    IVector ImainInitialData(DmainInitialData);

    Series startingSet(ImainInitialData,interval(-1,1),s,SeriesType::sin);
    startingSet = startingSet.resize(fullC0Size);
    startingSet = startingSet + Series(eps*interval(-1,1),s,SeriesType::sin);
    startingSet.s = s+1;
    Series startingSetH(interval(-1,1),interval(2),SeriesType::sin);
    auto start = std::chrono::high_resolution_clock::now();
    auto [result, isInclusion] = checkInclusion(startingSet, mainC0Size, fullC0Size, paramsDVector);
    auto stop = std::chrono::high_resolution_clock::now();
    if(isInclusion){
        std::chrono::duration<double> czas = stop - start;
        cout << "Periodic orbit found"<<endl;
        std::cout << "Time of C0 computations: " << czas.count() << " s" << std::endl;
        cout <<"Main C0 Size: " <<mainC0Size<<endl;
        cout <<"Full C0 Size: " <<fullC0Size<<endl;
        start = std::chrono::high_resolution_clock::now();
        Series h_result = C1Computation(result, startingSetH, mainC1Size, fullC1Size, paramsDVector );
        stop = std::chrono::high_resolution_clock::now();
        czas = stop - start;
        cout << h_result.normHs(interval(2))<<endl; 
        std::cout << "Time of C1 computations: " << czas.count() << " s" << std::endl;
        cout<< "h_norm:"<< h_result.normHs(interval(2)).right() <<endl;
        if (h_result.normHs(interval(2)) <1){
            cout << "Periodic orbit is locally attracting\n"<<"h_norm:"<< h_result.normHs(interval(2)).right()<<endl ;
            
        }
        else 
            cout << "Local atraction not validated" <<endl;
            
        
    }
    else{
     
        cout << DmainInitialData<<endl;
        startingSet.print();
        result.print();
        cout << "Periodic orbit not found"<<endl;
    }
}
void ProofList(){
    InOut inOut;
    DVector paramsDVector;
    inOut.paramsFile >> paramsDVector;
    int mainC0Size = 10;
    int fullC0Size = 20;

    for(int i=0;i<32;i++){
        double alfa = 1.- i*1./64;
        paramsDVector[0] = alfa;
        try{
            ComputerAssistedProof(mainC0Size,fullC0Size,mainC0Size,fullC0Size,0.001,2,paramsDVector);
        }
        catch(const std::exception& e){
            cout << "Exception: " << e.what() << endl;
            cout << "Change of dimetions" <<endl;
            i = i-1;
            mainC0Size = mainC0Size+10;
            fullC0Size = fullC0Size+20;
            cout << "MainSize: "<< mainC0Size<<endl;
            cout << "FullSize: "<< fullC0Size<<endl;
            if(mainC0Size >100){
                break;
            }
        }
    }
}
int main(){  
    ProofList();
}
