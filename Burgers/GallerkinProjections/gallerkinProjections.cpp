#include "../../DissipativePDE/VectorFieldMaker/vectorFieldMaker.h"
#include "gallerkinProjections.h"
using namespace std;
using namespace VectorFieldMaker;
//DOROBIC INKLUZJE

string gallerkinProjectionVecBurgers(int size,bool addInclusionTerms){

	std::vector<string> u =  makeStringVector(size,"u");
	std::vector<string> nonLinearity = multiply("delta",sinSqueredDerivative(u,size));
	nonLinearity.resize(u.size());
	nonLinearity[0] = nonLinearity[0] + "+A1*sin(omega*t)+B1";
	nonLinearity[1] = nonLinearity[1] + "+A2*sin(omega*t)+B2";
	std::vector<string> uLinear = std::vector<string>(size);
	for(int i=0;i<size;i++){
		uLinear[i] = "(-(" + to_string((i+1)) + ")^ (2*alfa))" ;
	}
	
	uLinear = multiply(uLinear,u); 
	std::vector<string> ut = add(uLinear,nonLinearity);
	std::vector<string> parameters(7);
	
	if(!addInclusionTerms){
		parameters[0] = "alfa";parameters[1] = "omega";parameters[2] = "delta"; 
		parameters[3] = "A1";parameters[4] = "B1";  parameters[5] = "A2";parameters[6] = "B2";
	}
	else{
		parameters.resize(7+size);
		parameters[0] = "alfa";parameters[1] = "omega";parameters[2] = "delta"; 
		parameters[3] = "A1";parameters[4] = "B1";  parameters[5] = "A2";parameters[6] = "B2";
		std::vector<string> utI = makeStringVector(size,"utI");
		ut = add(ut,utI);
		for(int i=7;i<size + 7;i++){
			parameters[i] = "utI" + to_string(i-7);
		}
	}
	return toFormula(parameters,"t",u,ut);
}


string gallerkinProjectionVecBurgersC1(int size,bool addInclusionTerms){
	std::vector<string> u = makeStringVector(size,"u");
	std::vector<string> h = makeStringVector(size,"h");
	std::vector<string> uLinear = std::vector<string>(size);
	std::vector<string> hLinear = std::vector<string>(size);
	for(int i=0;i<size;i++){
		uLinear[i] = "(-(" + to_string((i+1)) + ")^ (2*alfa))" ;
		hLinear[i] = "(-(" + to_string((i+1)) + ")^ (2*alfa))" ;
	}
	uLinear = multiply(uLinear,u); 
	hLinear = multiply(hLinear,h); 
	std::vector<string> uNonLinearity = multiply("delta",sinSqueredDerivative(u,size));
	uNonLinearity.resize(u.size());
	uNonLinearity[0] = uNonLinearity[0] + "+A1*sin(omega*t)+B1";
	uNonLinearity[1] = uNonLinearity[1] + "+A2*sin(omega*t)+B2";
	auto hNonLinearity =  multiply("2*delta",sinTimesSinDerivative(u,h));//To Do
	uNonLinearity.resize(u.size());
	hNonLinearity.resize(u.size());
	std::vector<string> ut = add(uLinear,uNonLinearity);
	std::vector<string> ht = add(hLinear,hNonLinearity);
	std::vector<string> parameters(7);
	
	if(!addInclusionTerms){
		parameters[0] = "alfa";parameters[1] = "omega";parameters[2] = "delta"; 
		parameters[3] = "A1";parameters[4] = "B1";  parameters[5] = "A2";parameters[6] = "B2";

	}
	else{
		parameters.resize(7+2*size);
		parameters[0] = "alfa";parameters[1] = "omega";parameters[2] = "delta"; 
		parameters[3] = "A1";parameters[4] = "B1";  parameters[5] = "A2";parameters[6] = "B2";
		std::vector<string> utI = makeStringVector(size,"utI");
		std::vector<string> htI = makeStringVector(size,"htI");
		ut = add(ut,utI);
		ht = add(ht,htI);
		for(int i=7;i<size + 7;i++){
			parameters[i] = "utI" + to_string(i-7);
			parameters[i+size] = "htI" + to_string(i-7);
		}
	}

	return toFormula(parameters,"t",merge(u,h),merge(ut,ht));
}
