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
<<<<<<< HEAD
    string MonthFilter = "-06-";//filter string
    string TimeFilter = "18:00:00";
		// ifstream file("smhi-opendata_Lund.csv");
=======
		int n = 0; //variable to store the index of the whileloop

>>>>>>> origin/master
		ifstream file(filePath);
		const string filePath_ = filePath; //Storing the filepath into a membervariable

		//// checks if file is open and proceeds if that is the case
		if(file.is_open()) {
			cout<<"File "<<filePath<<" opened \n"<<endl;
			cout<<"Initializing data extraction and sorting..."<<endl;

			// until the end of the file read and store the values of each column in it's respective vector.
			while(!file.eof()) {

				//throws the first lines away, junk data
				if (n < 12) {
					getline(file,helpString);
					n++;
					continue;
				}

				string nDate, nTime, nTemp; //string to temporarily store the values of the columns
				getline(file, nDate, ';'); //Find and stores the dates
				RawDate.push_back(nDate);

				getline(file, nTime, ';'); //Find and stores the times
				RawTime.push_back(nTime);

				getline(file, nTemp, ';'); //Find and stores the Temperature
				RawTemp.push_back(nTemp);

				getline(file, helpString); //Throws the rest away
				n++;
			}

			file.close();
			cout<<"Data extracted"<<endl;
			cout<<"File "<<filePath<<" closed"<<endl;

<<<<<<< HEAD
		file.close();
    for(int n=0; n<RawDate.size(); n++){ //Loop for each elments in our Rawvectors

    	if((RawDate[n].find(MonthFilter) != string::npos)&&(RawTime[n].find(TimeFilter) != string::npos)){ //Cheack if the filter criteria is match
      	Date.push_back(RawDate[n]); //Plugs in the filtered data in a new vector

        Time.push_back(RawTime[n]);
=======
			RawDate_ = RawDate;
			RawTime_ = RawTime;
			RawTemp_ = RawTemp;

		}
>>>>>>> origin/master

		else {
			cout<<"Can't open the file \n"<<endl;
		}
	}

	~tempTrender() {} //Destructor

	void tempForMonth(string monthToCalculate) {
		string monthFilter = "-" + monthToCalculate + "-";//filter string
		string timeFilter = "18:00:00";
		vector<string> Date, Time, Temp;

		cout<<"Extracting data for month "<<monthToCalculate<<"..."<<endl;
		for(int n=0; n<RawDate_.size() + 1; n++){ //Loop for each elments in our Rawvectors
			if(RawDate_[n].find(monthFilter) != string::npos && RawTime_[n].find(timeFilter) != string::npos){ //Cheack if the filter criteria is match

				Date.push_back(RawDate_[n]); //Plugs in the filtered data in a new vector
				Time.push_back(RawTime_[n]);
				Temp.push_back(RawTemp_[n]);
			}
		}
		Date_ = Date;
		Time_ = Time;
		Temp_ = Temp;
		cout<<"Done \n"<<endl;
		cout<<Date_[1451]<<endl;
	}; //Make a histogram of the temperature on this day

	void tempOnDay(string monthToCalculate, string dayToCalculate) {
		tempForMonth(monthToCalculate);
		string dayFilter = "-" + dayToCalculate;
		vector<string> dayDate, dayTemp;

		cout<<"Extracting data from month "<<monthToCalculate<<" for day "<<dayToCalculate<<"..."<<endl;
		for(int n=0; n<Date_.size();n++) {
			if(Date_[n].find(dayFilter) != string::npos) {
				dayDate.push_back(Date_[n]);
				dayTemp.push_back(Temp_[n]);
			}
		}
		dayDate_ = dayDate;
		dayTemp_ = dayTemp;
		cout<<"Done \n"<<endl;

	};
	//void tempPerDay(); //Make a histogram of the average temperature of each day of the year
	//void hotCold(); //Make a histogram of the hottest and coldest day of the year
	//void tempPerYear(int yearToExtrapolate); //Make a histogram of average temperature per year, then fit and extrapolate to the given year

	private:

	vector<string> RawDate_, RawTime_, RawTemp_; //membervariable to store RawData
	string filePath; //Membervariable to store the file path
	vector<string> Date_, Time_, Temp_; //membervariable to store specific data


};

#endif
