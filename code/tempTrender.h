#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

class tempTrender {
	public:
	tempTrender(const char* filePath) {
		vector<string> Date, Time, Temp; //vector consisting of strings to store all the values for the columns
		string helpString; //helpstring to store junk
		// ifstream file("smhi-opendata_Lund.csv");
		ifstream file(filePath);

		// checks if file is open and proceeds if that is the case
		if(file.is_open()) {
			cout<<"File "<<filePath<<" opened \n"<<endl;
			cout<<"Initializing data extracting and sorting..."<<endl;
			//reads the first 7 lines as junk
			for (int n = 0; n > 7; n++) {
				getline(file, helpString);
			}


			// until the end of the file read and store the values of each column in it's respective vector.
			while(!file.eof()) {
				string nDate, nTime, nTemp; //string to temporarily store the values of the columns
				getline(file, nDate, ';');
				Date.push_back(nDate);

				getline(file, nTime, ';');
				Time.push_back(nTime);

				getline(file, nTemp, ';');
				Temp.push_back(nTemp);

			}
			cout<<"Data extracted \n"<<endl;
		}

		else {
			cout<<"Can't open the file"<<endl;
		}

		file.close();
	}

	~tempTrender() {} //Destructor

	void tempOnDay(int monthToCalculate, int dayToCalculate); //Make a histogram of the temperature on this day
	void tempOnDay(int dateToCalculate); //Make a histogram of the temperature on this date
	//void tempPerDay(); //Make a histogram of the average temperature of each day of the year
	//void hotCold(); //Make a histogram of the hottest and coldest day of the year
	//void tempPerYear(int yearToExtrapolate); //Make a histogram of average temperature per year, then fit and extrapolate to the given year

	private:

	int monthToCalculate;
	int dayToCalculate;
	int dateToCalculate;
	string filePath;

};

#endif
