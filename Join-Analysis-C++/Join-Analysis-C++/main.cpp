//
//  main.cpp
//  Join-Analysis-C++
//
//  Created by Asjad Athick on 7/5/17.
//  Copyright © 2017 Asjad Athick. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "json.hpp"


using json = nlohmann::json;
using namespace std;

struct Coordinate{
	double x, y;
	Coordinate(){
		x = 0;
		y = 0;
	}
};

struct SkeletalFrame{
	//base vals
	Coordinate SpineBase;
	Coordinate SpineMid;
	Coordinate Neck;
	Coordinate Head;

	//movement vals
	Coordinate ShoulderLeft;
	Coordinate ElbowLeft;
	Coordinate WristLeft;
	Coordinate HandLeft;

	Coordinate ShoulderRight;
	Coordinate ElbowRight;
	Coordinate WristRight;
	Coordinate HandRight;

	//calculated measurements
	double height;
	double leftHandLength;
	double rightHandLength;

	SkeletalFrame(){
		height = 0;
		leftHandLength = 0;
		rightHandLength = 0;
	}
};

void extractCoordinateVal(string value, Coordinate &coord){
	auto pos = value.find(',');
	coord.x = stod(value.substr(0, pos));
	coord.y = stod(value.substr(pos + 1));
}

double distanceBetweenCoordinates(Coordinate c1, Coordinate c2){
	double dist = sqrt((pow(c2.x - c1.x, 2) + pow(c2.y - c1.y, 2)));
	if (dist < 0) {
		dist *= -1;
	}
	return dist;
}

int main(int argc, const char * argv[]) {
	ifstream input("/Users/Asjad/Documents/Repos/SkeletalJoinsExtraction-Kinect/DataExtracts/MotionOutput-Samples/stand.txt");
	if (!input.good()) {
		cout << "File open error" << endl;
		return -1;
	}

	json j;
	input >> j;

	//read all frames

	vector<SkeletalFrame> frames;

	for (long i = 0; i < j.size(); ++i) {
		SkeletalFrame temp;
		extractCoordinateVal(j[i]["SpineBase"], temp.SpineBase);
		extractCoordinateVal(j[i]["SpineMid"], temp.SpineMid);
		extractCoordinateVal(j[i]["Neck"], temp.Neck);
		extractCoordinateVal(j[i]["Head"], temp.Head);

		extractCoordinateVal(j[i]["ShoulderLeft"], temp.ShoulderLeft);
		extractCoordinateVal(j[i]["ElbowLeft"], temp.ElbowLeft);
		extractCoordinateVal(j[i]["WristLeft"], temp.WristLeft);
		extractCoordinateVal(j[i]["HandLeft"], temp.HandLeft);

		extractCoordinateVal(j[i]["ShoulderRight"], temp.ShoulderRight);
		extractCoordinateVal(j[i]["ElbowRight"], temp.ElbowRight);
		extractCoordinateVal(j[i]["WristRight"], temp.WristRight);
		extractCoordinateVal(j[i]["HandRight"], temp.HandRight);

		frames.push_back(temp);
	}

	//calculate height
	//calculate length of right arm
	//calculate length of left arm

	for (long i = 0; i < frames.size(); ++i) {
		frames[i].height = distanceBetweenCoordinates(frames[i].SpineBase, frames[i].SpineMid) + distanceBetweenCoordinates(frames[i].SpineMid, frames[i].Neck) + distanceBetweenCoordinates(frames[i].Neck, frames[i].Head);
		frames[i].leftHandLength = distanceBetweenCoordinates(frames[i].ShoulderLeft, frames[i].ElbowLeft) + distanceBetweenCoordinates(frames[i].ElbowLeft, frames[i].WristLeft) + distanceBetweenCoordinates(frames[i].WristLeft, frames[i].HandLeft);
		frames[i].rightHandLength = distanceBetweenCoordinates(frames[i].ShoulderRight, frames[i].ElbowRight) + distanceBetweenCoordinates(frames[i].ElbowRight, frames[i].WristRight) + distanceBetweenCoordinates(frames[i].WristRight, frames[i].HandRight);
	}


	

	return 0;
}
