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
#include <TGraph.h> //Graph object
#include <TLine.h>	//Line properties
#include <TLegend.h> //Legend object

using namespace std;

class tempTrender {
	public:
	// Constructor, when called does initial sorting of datafile
	tempTrender(const char* filePath) {
		vector<string> Date, Time, Temp, RawDate, RawTime, RawTemp; //vector consisting of strings to store all the values for the columns
		string helpString; //helpstring to store junk
		int k = 0; //variable to store the index of the whileloop

		ifstream file(filePath);
		const string filePath_ = filePath; //Storing the filepath into a membervariable

		// checks if file is open and proceeds if that is the case
		if(file.is_open()) {
			cout<<"File "<<filePath<<" opened"<<endl;
			cout<<"Initializing data extraction and sorting..."<<endl;

			// until the end of the file read and store the values of each column in it's respective vector.
			while(!file.eof()) {

				//throws the first 11 lines away, junk data
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

		// If file cant be read, notify user
		else {
			cout<<"Can't open the file \n"<<endl;
		}
	}

	~tempTrender() {} //Destructor

	//Function to extract the data for a specific month
	void tempForMonth(string monthToCalculate) {
		string monthFilter = "-" + monthToCalculate + "-"; //filter string to find month
		string timeFilter = "18:00:00"; 	//filter string to find time
		vector<string> Date, Time, Temp; //vectors to temporarily store the extracted data

		//Loop to find and extract the desired data
		for(int n=0; n<RawDate_.size() + 1; n++){
			if(RawDate_[n].find(monthFilter) != string::npos && RawTime_[n].find(timeFilter) != string::npos){ //Cheack if the filter criteria is match
				Date.push_back(RawDate_[n]);
				Time.push_back(RawTime_[n]);
				Temp.push_back(RawTemp_[n]);
			}
		}
		//Move the data to member variables for later use
		Date_ = Date;
		Time_ = Time;
		Temp_ = Temp;
	};

	//Fucntion to extract the data for a specific day in a month
	void tempOnDayInMonth(string monthToCalculate, string dayToCalculate) {
		tempForMonth(monthToCalculate); //extract data for desired month
		string dayFilter = "-" + dayToCalculate; //filter string to find day
		vector<string> dayDate, dayTemp; //vectors to temporarily store extracted data

		//Loop to find and extract the desired data
		for(int n=0; n<Date_.size();n++) {
			if(Date_[n].find(dayFilter) != string::npos) {
				dayDate.push_back(Date_[n]);
				dayTemp.push_back(Temp_[n]);
			}
		}
		//Moves the extracted data to member variables for later use
		dayDate_ = dayDate;
		dayTemp_ = dayTemp;
	};

	// Calculating and plotting average temp of all Junes vs temperature of all midsummers
	void plotJuneVsMidsummer() {
		tempOnDayInMonth("06","23"); //Extract data for desired month and day
		vector<double> tempAvg, midsummerTemp; //vectors to store extracted data
		double helpTemp, helpMidsummerTemp; //help variables for temporary storage of data
		double tempSum = 0; //help variable for temporary storage of temperature sum
		double midsummerAvgTempSum = 0; //help variable for temporary storage of temperature sum

		//Extract and store the average temperature for month for each year
		for (int n = 0; n < Date_.size()/30; n++) {
			for (int i=0; i < 30; i++) {
				stringstream(Temp_[n*i])>>helpTemp; //converting string to double
				tempSum += helpTemp;
			}
			tempAvg.push_back(tempSum/(30));
			tempSum = 0;
		}

		//Extract and store the temperature of midsummer for each year
		for (int n = 0; n <dayTemp_.size(); n++) {
			stringstream(dayTemp_[n])>>helpMidsummerTemp; //converting string to double
			midsummerTemp.push_back(helpMidsummerTemp);
		}

		//Calculating the averaqe temperature of midsummer
		for (int n= 0; n < midsummerTemp.size(); n++) {
			midsummerAvgTempSum += midsummerTemp[n];
		}
		double midsummerAvgTemp = midsummerAvgTempSum/midsummerTemp.size();

		// creating histogram for average temperature for each june
		TH1I* histJune = new TH1I("tempAvg", "Average temperature of June", 50, 12, 22);
		histJune->SetFillColor(kGreen-10);
		histJune->SetLineColor(kBlack);

		//Creating histogram for average temperature of midsummer
		TH1I* histMidsummer = new TH1I("tempAvg", "Average temperature of Midsummer", 50, 10, 25);
		histMidsummer->SetFillColor(kRed+1);
		histMidsummer->SetLineColor(kBlack);

		//Filling histogram for average temp of june
		for (int n = 0; n < tempAvg.size() ; n++){
			cout<<tempAvg[n]<<endl;
			histJune->Fill(tempAvg[n]);
		}
		//Filling histogram for average temp of midsummer
		for (int n = 0; n<56;n++){
			histMidsummer->Fill(midsummerAvgTemp);
		}

		TCanvas* can = new TCanvas(); //Creating new canvas to plot on

		//Getting gaussian fit of June and plotting hist of june and midsummer
		histJune->Fit("gaus");
		histJune->GetFunction("gaus")->SetLineColor(kBlack);
		histJune->Draw();
		histMidsummer->Draw("same");

		//Creating legend for the plot
		TLegend* legend1 = new TLegend(0.6,0.75,0.95,0.95);
		legend1->AddEntry(histJune, "June", "f");
		legend1->AddEntry(histJune, "Gaussian fit of June", "l");
		legend1->AddEntry(histMidsummer, "Midsummer", "f");
		legend1->SetHeader("Average temp of June vs Midsummer");
		legend1->Draw();

		//Editing the axis labels
		histJune->GetXaxis()->SetTitle("Temp/#circ C");
		histJune->GetXaxis()->CenterTitle();
		histJune->GetYaxis()->SetTitle("Bins");
		histJune->GetYaxis()->CenterTitle();

		//Saving the plot as a png
		can->SaveAs("LundJuneVsMidsummer.png");
	};

	//Calculating and plotting the average temp for each day between May and September
	void plotAvergageDayMayToSep() {

		vector<double> averageDayTemp; //Vector to store the average temperature of each day

		//Filter to extract the data for each day in May
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

			//Calculating the average for each day and storing it
			for (int i = 0; i <dayTemp_.size(); i++) {
				double helpDayTemp;
				stringstream(dayTemp_[i])>>helpDayTemp;
				averageDaySum += helpDayTemp;

			}
		averageDayTemp.push_back(averageDaySum/dayTemp_.size());
		}

		//Rinse and repeat for june to sep ---------------------
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

			tempOnDayInMonth("09", helpString);

			for (int i = 0; i <dayTemp_.size(); i++) {
				double helpDayTemp;
				stringstream(dayTemp_[i])>>helpDayTemp;
				averageDaySum += helpDayTemp;

			}
		averageDayTemp.push_back(averageDaySum/dayTemp_.size());
		}

		//Creating lists with the appropriate values to feed the Graph
		Int_t n = 153;
		Double_t x[n], y[n], x2[n], y2[n];
		for(Int_t i=0;i<n;i++){
			x[i] = i;
			y[i] = averageDayTemp[i];

			//To create a divide for each month (Hardcoded....)
			if(i == 31 || i == 61 || i == 92 || i == 123 || i == 153) {
				x2[i] = i;
				y2[i] = 12;
			}
			else {
			x2[i] = i;
			y2[i] = 0;
			}
		}

		//Creating graphs for temp for each day and monthdivider
		TGraph* graphMayJuneJuly = new TGraph(n, x, y);
		TGraph* graphDivider = new TGraph(n, x2, y2);
		//Creating new canvas
		TCanvas* can2 = new TCanvas();

		//Plotting the graphs and fit a 2degree polynomial
		graphMayJuneJuly->Draw("AC");
		graphDivider->Draw("LF2");
		graphMayJuneJuly->Fit("pol2");
		graphMayJuneJuly->GetFunction("pol2")->SetLineColor(kRed);
		graphMayJuneJuly->SetLineWidth(2);

		//Create legend
		TLegend* legend1 = new TLegend(0.60,0.8,0.95,0.95);
		legend1->AddEntry(graphMayJuneJuly, "Temperature May-Sep", "l");
		legend1->SetHeader("Daily Temperature from May to September");
		legend1->Draw();

		//Modify the axis labels
		graphMayJuneJuly->GetXaxis()->SetTitle("Day");
		graphMayJuneJuly->GetXaxis()->CenterTitle();
		graphMayJuneJuly->GetYaxis()->SetTitle("Temp/#circ C");
		graphMayJuneJuly->GetYaxis()->CenterTitle();

		//Saving the plot as png
		can2->SaveAs("LundMayToSep.png");

	};

	//Calculating the temp for each midsummer and plotting
	void plotMidsummerTemp() {
		tempOnDayInMonth("06","23"); //Find temperature for month and day

		vector<double> midsummerTempVec; //Vector to store Temperature
		double helpMidsummerTemp; //Help varaible to temporarily store temperature

		//Exctracting temperature for midsummer
		for (int n = 0; n <dayTemp_.size(); n++) {
			stringstream(dayTemp_[n])>>helpMidsummerTemp; //converting string to double
			midsummerTempVec.push_back(helpMidsummerTemp); //Stroring values obtained
		}

		//Fill lists with values to feed the graph
		Int_t n = dayTemp_.size();
		Double_t x[n], y[n];
		for(Int_t i=0;i<n;i++){
			x[i] = i+1961;
			y[i] = midsummerTempVec[i];
		}

		//Creating new graph for midsummer temperatures
		TGraph* graphMidsummer = new TGraph(n, x, y);
		//Creating new canvas
		TCanvas* can3 = new TCanvas();

		can3->SetGrid(); //make canvas gridded

		//plotting the graph and fitting a 1st degree polynomial
		graphMidsummer->SetFillColor(kRed+1);
		graphMidsummer->SetLineColor(kBlack);
		graphMidsummer->Draw("AB");
		graphMidsummer->Fit("pol1");
		graphMidsummer->GetFunction("pol1")->SetLineColor(kBlack);

		//Create legend
		TLegend* legend1 = new TLegend(0.6,0.75,0.95,0.95);
		legend1->AddEntry(graphMidsummer, "Midsummer", "f");
		legend1->AddEntry(graphMidsummer, "Linear fit of Midsummer", "l");
		legend1->SetHeader("Temperature of Midsummer each year");
		legend1->Draw();

		//Modifying axis
		graphMidsummer->GetXaxis()->SetLimits(1961,2015);
		graphMidsummer->GetXaxis()->SetTitle("Year");
		graphMidsummer->GetXaxis()->CenterTitle();
		graphMidsummer->GetYaxis()->SetTitle("Temp/#circ C");
		graphMidsummer->GetYaxis()->CenterTitle();

		//Saving plot as png
		can3->SaveAs("LundMidsummer.png");
	}

	private:

	vector<string> RawDate_, RawTime_, RawTemp_; //membervariable to store RawDatas
	string filePath; //Membervariable to store the file path
	vector<string> Date_, Time_, Temp_; //membervariable to store month data
	vector<string> dayDate_, dayTemp_; //membervariable to store daily data
};

#endif
