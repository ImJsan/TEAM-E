#include "tempTrender.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

int main() {
	// string pathToFile = "smhi-opendata_Lund.csv"; //Put the path to your data file here
	//tempTrender t(pathToFile); //Instantiate your analysis object

	//t.tempOnDay(8, 23); //Call some functions that you've implemented
	//t.tempOnDay(235);
	//t.tempPerDay();
	//t.hotCold();
	//t.tempPerYear(2050);
	string nC1, nC2, nC3, nC4, nC5, nC6;
	vector<double> C1, C2, C3, C4, C5, C6;
	string helpString;
	ifstream file("smhi-opendata_Lund.csv");

	if(file.is_open()) {
		cout<<"File is open \n"<<endl;
	}
	else {
		cout<<"Can't open the file"<<endl;
	}

	int n = 0;

	for (int n = 0; n > 7; n++) {
		getline(file, helpString);
	}


	// Need to find a way to push back the values of nC to the vectors C, following while loop does sort the colums right but compilers is complaining with pushback
	while(!file.eof()) {
			getline(file, nC1, ';');
			// C1.push_back(nC1);
			cout<<nC1;

			getline(file, nC2, ';');
			// C2.push_back(nC2);
			cout<<nC2<<endl;


			getline(file, nC3, ';');
			// C3.push_back(nC3);

			getline(file, nC4, ';');
			// C4.push_back(nC4);

			getline(file, nC5, ';');
			// C5.push_back(nC5);

			getline(file, nC6);
			// C6.push_back(nC6);


	}



	file.close();
	return 0;
}
