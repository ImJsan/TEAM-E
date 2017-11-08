#include "tempTrender.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

//Root libraries
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object

using namespace std;

void project() {
	const char* pathToFile = "datasets/smhi-opendata_Lund.csv"; //Put the path to your data file here
	tempTrender t(pathToFile); //Instantiate your analysis object
	//~ t.plotJuneVsMidsummer();
	t.plotAvergageDayMayJuneJuly();
}
