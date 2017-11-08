#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>

//Root libraries
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
#include <TGraph.h>

using namespace std;

class tempTrender {
	public:
	tempTrender(const char* filePath) {
		vector<string> Date, Time, Temp, RawDate, RawTime, RawTemp; //vector consisting of strings to store all the values for the columns
		string helpString; //helpstring to store junk
		int k = 0; //variable to store the index of the whileloop

		ifstream file(filePath);
		const string filePath_ = filePath; //Storing the filepath into a membervariable

		//// checks if file is open and proceeds if that is the case
		if(file.is_open()) {
			cout<<"File "<<filePath<<" opened"<<endl;
			cout<<"Initializing data extraction and sorting..."<<endl;

			// until the end of the file read and store the values of each column in it's respective vector.
			while(!file.eof()) {

				//throws the first lines away, junk data
				if (k < 12) {
					getline(file,helpString);
					k++;
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
				k++;
			}

			file.close();
			cout<<"Data extracted"<<endl;
			cout<<"File "<<filePath<<" closed \n"<<endl;

			RawDate_ = RawDate;
			RawTime_ = RawTime;
			RawTemp_ = RawTemp;

		}

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
	}; //Make a histogram of the temperature on this day

	void tempOnDayInMonth(string monthToCalculate, string dayToCalculate) {
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
	// Calculating and plotting average temp of all Junes vs temperature of all midsummers
	void plotJuneVsMidsummer() {
		// tempForMonth("06");
		tempOnDayInMonth("06","23");
		vector<double> tempAvg, midsummerTemp;
		double helpTemp, helpMidsummerTemp;
		double tempSum = 0;

		cout<<"Calculating average temperature of June for every year..."<<endl;
		for (int n = 0; n < Date_.size()/30; n++) {
			for (int i=0; i < 30; i++) {
				stringstream(Temp_[n*i])>>helpTemp;
				tempSum += helpTemp;
			}
			tempAvg.push_back(tempSum/(30));
			tempSum = 0;
		}
		cout<<"Done \n"<<endl;

		for (int n = 0; n <dayTemp_.size(); n++) {
			stringstream(dayTemp_[n])>>helpMidsummerTemp;
			midsummerTemp.push_back(helpMidsummerTemp);
		}

		double midsummerAvgTempSum = 0;
		for (int n= 0; n < midsummerTemp.size(); n++) {
			midsummerAvgTempSum += midsummerTemp[n];
		}
		double midsummerAvgTemp = midsummerAvgTempSum/midsummerTemp.size();

		// Plotting histogram for average temperature for each in june and temperature for each midsummer
		TH1I* histJune = new TH1I("tempAvg", "Average temperature of June", 100, 0, 30);
		histJune->SetFillColor(kWhite);

		TH1I* histMidsummer = new TH1I("tempAvg", "Average temperature of Midsummer", 100, 0, 30);
		histMidsummer->SetFillColor(4);

		for (int n = 0; n < tempAvg.size(); n++){
			cout<<tempAvg[n]<<endl;
			histJune->Fill(tempAvg[n]); //Increment the bin corresponding to -3.2 C
		}
		for (int n = 0; n<56;n++){
			histMidsummer->Fill(midsummerAvgTemp); //Increment the bin corresponding to -3.2 C
		}
		TCanvas* can = new TCanvas();
		histJune->Fit("gaus");
		histJune->Draw();
		histMidsummer->Draw("same");


	}; //Make a histogram of the average temperature of each day of the year
	
	void plotAvergageDayMayJuneJuly() {

		vector<double> averageDayTemp;
		
		for (int n = 1; n < 32; n++) {
			double averageDaySum = 0;
			char stringNumber[5];
			sprintf(stringNumber, "%d", n);
			string helpString;
			if (n < 10) {
				string filterZero = "0";
				helpString += filterZero + stringNumber;
			}
			else {
				helpString = stringNumber;
			}	
			
			tempOnDayInMonth("05", helpString);
			
			for (int i = 0; i <dayTemp_.size(); i++) {
				double helpDayTemp;
				stringstream(dayTemp_[i])>>helpDayTemp;
				averageDaySum += helpDayTemp;
				
		}
		averageDayTemp.push_back(averageDaySum/dayTemp_.size());
	}

		for (int n = 1; n < 31; n++) {
			double averageDaySum = 0;
			char stringNumber[5];
			sprintf(stringNumber, "%d", n);
			string helpString;
			if (n < 10) {
				string filterZero = "0";
				helpString += filterZero + stringNumber;
			}
			else {
				helpString = stringNumber;
			}	
			
			tempOnDayInMonth("06", helpString);
			
			for (int i = 0; i <dayTemp_.size(); i++) {
				double helpDayTemp;
				stringstream(dayTemp_[i])>>helpDayTemp;
				averageDaySum += helpDayTemp;
				
		}
		averageDayTemp.push_back(averageDaySum/dayTemp_.size());
	}
	
		for (int n = 1; n < 32; n++) {
			double averageDaySum = 0;
			char stringNumber[5];
			sprintf(stringNumber, "%d", n);
			string helpString;
			if (n < 10) {
				string filterZero = "0";
				helpString += filterZero + stringNumber;
			}
			else {
				helpString = stringNumber;
			}	
			
			tempOnDayInMonth("07", helpString);
			
			for (int i = 0; i <dayTemp_.size(); i++) {
				double helpDayTemp;
				stringstream(dayTemp_[i])>>helpDayTemp;
				averageDaySum += helpDayTemp;
				
		}
		averageDayTemp.push_back(averageDaySum/dayTemp_.size());
	}

		for (int n = 1; n < 32; n++) {
			double averageDaySum = 0;
			char stringNumber[5];
			sprintf(stringNumber, "%d", n);
			string helpString;
			if (n < 10) {
				string filterZero = "0";
				helpString += filterZero + stringNumber;
			}
			else {
				helpString = stringNumber;
			}	
			
			tempOnDayInMonth("08", helpString);
			
			for (int i = 0; i <dayTemp_.size(); i++) {
				double helpDayTemp;
				stringstream(dayTemp_[i])>>helpDayTemp;
				averageDaySum += helpDayTemp;
				
		}
		averageDayTemp.push_back(averageDaySum/dayTemp_.size());
	}

		Int_t n = 123;
		Double_t x[n], y[n];
		for(Int_t i=0;i<n;i++){
		x[i] = i;
		y[i] = averageDayTemp[i];
		}		
		TGraph* graphMayJuneJuly = new TGraph(n, x, y);
		graphMayJuneJuly->Draw("AC*");

		//~ for (int n = 0; n < averageDayTemp.size(); n++){
			//~ cout<<n<<" "<<averageDayTemp[n]<<endl;
			//~ graphMayJuneJuly->Draw(averageDayTemp[n]); //Increment the bin corresponding to -3.2 C
		//~ }

		//~ TCanvas* can = new TCanvas();

	};

	private:

	vector<string> RawDate_, RawTime_, RawTemp_; //membervariable to store RawData
	string filePath; //Membervariable to store the file path
	vector<string> Date_, Time_, Temp_; //membervariable to store specific data
	vector<string> dayDate_, dayTemp_;

};

#endif
