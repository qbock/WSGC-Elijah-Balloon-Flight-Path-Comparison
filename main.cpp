//
//  Flight Path Analysis
//  2018 Elijah High Altitude Balloon Team
//
//  Created by Quinlan Bock (and Cody Nettesheim) on 6/29/18.
//  Copyright © 2018 Quinlan Bock. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

// Conversions
int hourToSec = 3600;
int minuteToSec = 60;

// Point data structure
struct Point
{
    int time;
    double latitude;
    double longitude;
    double altitude;
};

// Struct to return vector and size
struct vecSize{
    vector<Point> vec;
    int size;
};

// Method to find distance between two points
double distance(Point x, Point y){
    return sqrt(pow((y.latitude - x.latitude), 2.0) + pow((y.longitude - x.longitude), 2.0));
}

// Method to read in csv file
vecSize readFile(string path){
    vector<Point> coordinates(10000);
    
    // Read in the values from csv files
    std::ifstream in;
    in.open(path);
    if(!in.is_open()) std::cout << "ERROR: File '" + path + "' could not be opened" << '\n';
    
    int count = 0;
    
    string unixtime;
    string latitude;
    string longitude;
    string altitude;
    
    while(in.good()){
        getline(in,unixtime,',');
        getline(in,latitude,',');
        getline(in,longitude,',');
        getline(in,altitude,'\n');
        
        // Create point store converted strings as doubles
        Point p;
        
        p.time = atof(unixtime.c_str());
        p.latitude = atof(latitude.c_str());;
        p.longitude = atof(longitude.c_str());
        p.altitude = atof(altitude.c_str());
        coordinates[count] = p;
        
        count++;
    }
    in.close();
    
    vecSize toReturn;
    toReturn.vec = coordinates;
    toReturn.size = count;
    return toReturn;
}

// Method to read in UWYO predictions
vecSize readFileUWYO(string path){
    vector<Point> coordinates(1000);
    
    // Read in the values from csv files
    std::ifstream in;
    in.open(path);
    if(!in.is_open()) std::cout << "ERROR: File '" + path + "' could not be opened" << '\n';
    
    int count = 0;
    
    string hour; string minute; string second; string latitude; string longitude; string altitude;
    string DME; string VOR; string U; string V; string W; string P; string T; string RH;
    
    
    while(in.good()){
        getline(in,hour,':');
        getline(in,minute,':');
        getline(in,second,',');
        getline(in,latitude,',');
        getline(in,longitude,',');
        getline(in,altitude,',');
        getline(in,DME,',');
        getline(in,VOR,',');
        getline(in,U,',');
        getline(in,V,',');
        getline(in,W,',');
        getline(in,P,',');
        getline(in,T,',');
        getline(in,RH,'\n');
        
        // Create point store converted strings as doubles
        Point p;
        
        p.time = (atof(hour.c_str())*hourToSec) + (atof(minute.c_str())*minuteToSec) + (atof(second.c_str()));
        p.latitude = atof(latitude.c_str());;
        p.longitude = atof(longitude.c_str());
        p.altitude = atof(altitude.c_str());
        coordinates[count] = p;
        
        count++;
    }
    in.close();
    
    vecSize toReturn;
    toReturn.vec = coordinates;
    toReturn.size = count;
    return toReturn;
}

// Method to read in Actual Flight Data
vecSize readFileActual(string path){
    vector<Point> coordinates(1000);
    
    // Read in the values from csv files
    std::ifstream in;
    in.open(path);
    if(!in.is_open()) std::cout << "ERROR: File '" + path + "' could not be opened" << '\n';
    
    int count = 0;
    
    string latitude; string longitude; string altitude; string time;
    
    while(in.good()){

        getline(in,latitude,','); getline(in,longitude,','); getline(in,altitude,','); getline(in,time,'\n');
        
        // Create point store converted strings as doubles
        Point p;
        
        // break up the time string and get time in seconds
        int hours = atof((time.substr(16,17)).c_str());
        int minutes = atof((time.substr(19,20)).c_str());
        int seconds = atof((time.substr(22,23)).c_str());
        
        //Paylcout << "hours: " << hours << " minutes: " << minutes << " seconds: " << seconds << "\n";
        
        
        // initalize the point object's varibles
        p.time = (hours*hourToSec) + (minutes*minuteToSec) + seconds;
        p.latitude = atof(latitude.c_str());;
        p.longitude = atof(longitude.c_str());
        p.altitude = atof(altitude.c_str());
        coordinates[count] = p;
        
        count++;
    }
    in.close();
    
    vecSize toReturn;
    toReturn.vec = coordinates;
    toReturn.size = count;
    return toReturn;
}

// Method to read in UWYO predictions
vecSize readFileS3(string path){
    vector<Point> coordinates(1000);
    
    // Read in the values from csv files
    std::ifstream in;
    in.open(path);
    if(!in.is_open()) std::cout << "ERROR: File '" + path + "' could not be opened" << '\n';
    
    int count = 0;
    
    string unixtime; string latitude; string longitude; string altitude;
    
    int dataCount = 0;
    
    string timestamp = "\"timestamp\":";
    string lat = "\"lat\":";
    string lng = "\"lng\":";
    string alt = "\"alt\":";
    
    
    while(in.good()){
        string temp;
        int lastChar = int(temp.length()-1);
        getline(in,temp,'\n');
        
        if (temp.find(timestamp) != string::npos) {
            unixtime = temp.substr(13,lastChar);
            dataCount++;
        }
        if (temp.find(lat) != string::npos) {
            latitude = temp.substr(7,lastChar);
            dataCount++;
        }
        if (temp.find(lng) != string::npos) {
            longitude = temp.substr(7,lastChar);
            dataCount++;
        }
        if (temp.find(alt) != string::npos) {
            altitude = temp.substr(7,lastChar);
            dataCount++;
        }

        if(dataCount>=4){
            // Create point store converted strings as doubles
            Point p;
            
            cout.precision(10);
            p.time = stod(unixtime);
            p.latitude = stod(latitude);
            p.longitude = stod(longitude);
            p.altitude = stod(altitude);
            coordinates[count] = p;
            
            count++;
            dataCount=0;
        }
    }
    in.close();
    
    vecSize toReturn;
    toReturn.vec = coordinates;
    toReturn.size = count - 3;
    return toReturn;
}

// Adjust the unix timestamp so that that launch is 0
void adjust(vecSize & toAdjust){
    int x = toAdjust.vec[0].time;
    for(int i = 0; i<=toAdjust.size; i++){
        toAdjust.vec[i].time -= x;
    }
}

// Helper function to find closest point based on timestamp
int findClosest(int timestamp, vecSize & act){
    int index = 0;
    //Loop through each timestamp and check if it is closer than the last
    for (int i = 1; i < act.size; i++) {
        int difference = abs(timestamp - act.vec[i].time);
        int lastDifference = abs(timestamp - act.vec[i - 1].time);
        if (difference < lastDifference) {
            index = i;
        }
    }
    return index;
}

//Helper function to find exact point based on timestamp
Point findPoint(int timestamp, vecSize & act) {
    Point point;
    for (int i = 0; i < act.size; i++) {
        Point current = act.vec[i];
        Point prev = act.vec[i - 1];
        //Return the point with the matching timestamp if it exists
        if (current.time == timestamp) {
            point = current;
            break;
        }
        //Otherwise, find the coordinates at the given timestamp based on the coordinates at the
        // 2 closest (before and after) timestamps
        else if (current.time > timestamp && prev.time < timestamp) {
            double timeRatio = (timestamp - prev.time)/(current.time - timestamp);
            point.time = timestamp;
            point.altitude = ((timeRatio * current.altitude) + prev.altitude) / (1 + timeRatio);
            point.latitude = ((timeRatio * current.latitude) + prev.latitude) / (1 + timeRatio);
            point.longitude = ((timeRatio * current.longitude) + prev.longitude) / (1 + timeRatio);
            break;
        }  
    }
    return point;
}

// Function to print out the data in the vector
void print(vecSize & toPrint){
    cout << "time, latitude, longitude, altitude \n";
    for(int i = 0; i<toPrint.size; i++){
        cout << toPrint.vec[i].time << "," << toPrint.vec[i].latitude << ","
        << toPrint.vec[i].longitude << "," << toPrint.vec[i].altitude <<"\n";
    }
}

// Functions to go through and find the devations between given points
void calcDev(vector<double> & deviations, vecSize & pre,vecSize & act){
    // Loop through the thing that has the lower resolution
    for(int i = 0; i<=pre.size; i++)
    {
        Point p = pre.vec[i];
        int pTime = p.time;
        Point a = findPoint(pTime, act);
        cout << pTime << "," << p.latitude << "," << p.longitude << "," << p.altitude;
        cout << "," << a.time << "," << a.latitude << "," << a.longitude << "," << a.altitude;
        deviations[i] = distance(a,p);
        cout << "," << distance(a,p) << "\n";
    }
}

// Function for Standard Deviation
double stdev(vector<double> & deviations, vecSize & pre, double mean){
    // calculate the standard deviation
    double sum = 0.0;
    double size = double(pre.size);
    for(int i=0;i<size;i++){
        double summand = pow((abs(deviations[i]-mean)), 2.0);
        sum += summand;
    }
    double stddeviation = sqrt(sum/size);
    return stddeviation;
}

// Function for Mean
double findMean(vector<double> & deviations, vecSize & pre){
    // find the mean of the deviations
    double mean = 0.0;
    int size = pre.size;
    for(int i = 0; i<size; i++){
        mean += deviations[i];
    }
    mean = mean/double(size);
    
    return mean;
}

int main(int argc, const char * argv[]) {
    //**************************************** ~ File Paths ~ ******************************************************
    string Cambridge_prediction_file = "/Users/quinlanbock/Desktop/N-d polygon/N-d polygon/Cambridge_prediction.csv";
    string Cambridge_prediction_12_file = "/Users/quinlanbock/Desktop/N-d polygon/N-d polygon/Cambridge_1.2.csv";
    string Cambridge_prediction_14_file = "/Users/quinlanbock/Desktop/N-d polygon/N-d polygon/Cambridge_1.4.csv";
    string Cambridge_prediction_225_file = "/Users/quinlanbock/Desktop/N-d polygon/N-d polygon/Cambridge_2.25.csv";
    string ASTRA_prediction_file = "/Users/quinlanbock/Desktop/N-d polygon/N-d polygon/ASTRA_prediction.csv";
    string UWYO_prediction_file = "/Users/quinlanbock/Desktop/N-d polygon/N-d polygon/UWYO_prediction.csv";
    string actual_file = "/Users/quinlanbock/Desktop/N-d polygon/N-d polygon/actual.csv";
    string s3_prediction_file = "/Users/quinlanbock/Desktop/N-d polygon/N-d polygon/s3.txt";
    
    // Read in the files
    vecSize Cambridge_prediction = readFile(Cambridge_prediction_file);
    vecSize Cambridge_prediction_12 = readFile(Cambridge_prediction_12_file);
    vecSize Cambridge_prediction_14 = readFile(Cambridge_prediction_14_file);
    vecSize Cambridge_prediction_225 = readFile(Cambridge_prediction_225_file);
    vecSize ASTRA_prediction = readFile(ASTRA_prediction_file);
    vecSize UWYO_prediction = readFileUWYO(UWYO_prediction_file);
    vecSize actual = readFileActual(actual_file);
    vecSize s3_prediction = readFileS3(s3_prediction_file);
    
    // Adjust the timestamps so its starts at 0 seconds
    adjust(Cambridge_prediction);
    adjust(Cambridge_prediction_12);
    adjust(Cambridge_prediction_14);
    adjust(Cambridge_prediction_225);
    adjust(ASTRA_prediction);
    adjust(UWYO_prediction);
    adjust(actual);
    adjust(s3_prediction);
    
    // Confirm that everything has read in correctly
    
    cout << "Cambridge prediction (1.4 m/s):" << "\n";
    print(Cambridge_prediction_14);
    cout << "\n";
    cout << "ASTRA prediction:" << "\n";
    print(ASTRA_prediction);
    cout << "\n";
    cout << "UWYO prediction:" << "\n";
    print(UWYO_prediction);
    cout << "\n";
    cout << "S3 prediction:" << "\n";
    print(s3_prediction);
    cout << "\n";
    cout << "Actual:" << "\n";
    print(actual);
    cout << "\n";
    
    
    
    // Prediction Deviation Results
    vector<double> deviations(actual.size);
    calcDev(deviations,actual,Cambridge_prediction_14);
    
    cout << "Cambridge Deviation:" << "\n";
    for(int i = 0; i<actual.size -1; i++){
        cout << deviations[i] << " degrees" << "\n";
    }
    
    double mean = findMean(deviations, actual);
    double stddeviation = stdev(deviations, actual, mean);
    
    cout << "The mean devation in the Cambridge prediction is: " << mean << " and the standard deviation is " << stddeviation << "\n";
    
}
