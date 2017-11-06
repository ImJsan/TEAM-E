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
		vector<string> Date, Time, Temp, RawDate, RawTime, RawTemp; //vector consisting of strings to store all the values for the columns
		string helpString; //helpstring to store junk
    string MonthFilter = "-06-";//filter string
    string TimeFilter = "18:00:00";
		// ifstream file("smhi-opendata_Lund.csv");
		ifstream file(filePath);

		//// checks if file is open and proceeds if that is the case
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
				RawDate.push_back(nDate);

				getline(file, nTime, ';');
				RawTime.push_back(nTime);

				getline(file, nTemp, ';');
				RawTemp.push_back(nTemp);

			}
			cout<<"Data extracted \n"<<endl;
		}

		else {
			cout<<"Can't open the file"<<endl;
		}

		file.close();
    for(int n=0; n<RawDate.size(); n++){ //Loop for each elments in our Rawvectors

    	if((RawDate[n].find(MonthFilter) != string::npos)&&(RawTime[n].find(TimeFilter) != string::npos)){ //Cheack if the filter criteria is match
      	Date.push_back(RawDate[n]); //Plugs in the filtered data in a new vector

        Time.push_back(RawTime[n]);

        Temp.push_back(RawTemp[n]);
      }
    }
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
