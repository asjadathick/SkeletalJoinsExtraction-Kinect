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

const double PI = 3.14159265;

using json = nlohmann::json;
using namespace std;

struct Coordinate{
	double x, y;
	Coordinate(){
		x = 0;
		y = 0;
	}
	Coordinate(double x, double y){
		this->x = x;
		this->y = y;
	}
};

Coordinate operator+(const Coordinate lh, const Coordinate rh){
	return Coordinate(lh.x + rh.x, lh.y + rh.y);
}

Coordinate operator/(const Coordinate op, double divisor){
	return Coordinate(op.x / divisor, op.y / divisor);
}

struct Line{
	Coordinate c1, c2;

	Line();
	Line(Coordinate c1, Coordinate c2){
		this->c1 = c1;
		this->c2 = c2;
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

	Coordinate HipLeft;
	Coordinate KneeLeft;
	Coordinate AnkleLeft;

	Coordinate HipRight;
	Coordinate KneeRight;
	Coordinate AnkleRight;

	//calculated measurements
	double height;
	double leftHandLength;
	double rightHandLength;
	double leftLegLength;
	double rightLegLegth;

	//calculated angles;
	double lowerLeftArmAngle;
	double lowerRightArmAngle;

	double upperLeftLegAngle;
	double upperRightLegAngle;

	double lowerLeftLegAngle;
	double lowerRightLegAngle;

	Coordinate centerPoint;

	SkeletalFrame(){
		height = 0;
		leftHandLength = 0;
		rightHandLength = 0;
		leftLegLength = 0;
		rightLegLegth = 0;
		lowerLeftLegAngle = 0;
		lowerRightLegAngle = 0;
		upperLeftLegAngle = 0;
		upperRightLegAngle = 0;
		lowerLeftLegAngle = 0;
		lowerRightLegAngle = 0;
	}
};

void extractCoordinateVal(string value, Coordinate &coord){
	auto pos = value.find(',');
	coord.x = stod(value.substr(0, pos));
	coord.y = stod(value.substr(pos + 1));
}

double distanceBetweenCoordinates(Coordinate c1, Coordinate c2){
	double dist = sqrt((pow(c2.x - c1.x, 2) + pow(c2.y - c1.y, 2)));
	return abs(dist);
}

double gradient(Line l1){
	return (l1.c2.y - l1.c1.y) / (l1.c2.x - l1.c1.x);
}

double degreeToRadians(double degree){
	return degree * (PI/180);
}

double radianToDegrees(double radian){
	return radian * (180/PI);
}

double angleBetweenLines(Line l1, Line l2){
	double angle = 0;
	double m1 = gradient(l1);
	double m2 = gradient(l2);

	angle = (PI - abs(atan(m1) - atan(m2)));

	return radianToDegrees(angle);
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

		extractCoordinateVal(j[i]["HipLeft"], temp.HipLeft);
		extractCoordinateVal(j[i]["KneeLeft"], temp.KneeLeft);
		extractCoordinateVal(j[i]["AnkleLeft"], temp.AnkleLeft);

		extractCoordinateVal(j[i]["HipRight"], temp.HipRight);
		extractCoordinateVal(j[i]["KneeRight"], temp.KneeRight);
		extractCoordinateVal(j[i]["AnkleRight"], temp.AnkleRight);

		frames.push_back(temp);
	}

	//calculate height
	//calculate length of right arm
	//calculate length of left arm
	//calculate angles between 3 key bones

	for (long i = 0; i < frames.size(); ++i) {
		frames[i].leftHandLength = distanceBetweenCoordinates(frames[i].ShoulderLeft, frames[i].ElbowLeft) + distanceBetweenCoordinates(frames[i].ElbowLeft, frames[i].WristLeft) + distanceBetweenCoordinates(frames[i].WristLeft, frames[i].HandLeft);
		frames[i].rightHandLength = distanceBetweenCoordinates(frames[i].ShoulderRight, frames[i].ElbowRight) + distanceBetweenCoordinates(frames[i].ElbowRight, frames[i].WristRight) + distanceBetweenCoordinates(frames[i].WristRight, frames[i].HandRight);

		frames[i].leftLegLength = distanceBetweenCoordinates(frames[i].HipLeft, frames[i].KneeLeft) + distanceBetweenCoordinates(frames[i].KneeLeft, frames[i].AnkleLeft);
		frames[i].rightLegLegth = distanceBetweenCoordinates(frames[i].HipRight, frames[i].KneeRight) + distanceBetweenCoordinates(frames[i].KneeRight, frames[i].AnkleRight);

		frames[i].height = ((frames[i].leftLegLength + frames[i].rightLegLegth) / 2) + distanceBetweenCoordinates(frames[i].SpineBase, frames[i].SpineMid) + distanceBetweenCoordinates(frames[i].SpineMid, frames[i].Neck) + distanceBetweenCoordinates(frames[i].Neck, frames[i].Head);

		frames[i].lowerLeftArmAngle = angleBetweenLines(Line(frames[i].ShoulderLeft, frames[i].ElbowLeft), Line(frames[i].ElbowLeft, frames[i].WristLeft));
		frames[i].lowerRightArmAngle = angleBetweenLines(Line(frames[i].ShoulderRight, frames[i].ElbowRight), Line(frames[i].ElbowRight, frames[i].WristRight));

		frames[i].upperLeftLegAngle = angleBetweenLines(Line(frames[i].SpineBase, frames[i].HipLeft), Line(frames[i].HipLeft, frames[i].KneeLeft));
		frames[i].upperRightLegAngle = angleBetweenLines(Line(frames[i].SpineBase, frames[i].HipRight), Line(frames[i].HipRight, frames[i].KneeRight));

		frames[i].lowerLeftLegAngle = angleBetweenLines(Line(frames[i].HipLeft, frames[i].KneeLeft), Line(frames[i].KneeLeft, frames[i].AnkleLeft));
		frames[i].lowerRightLegAngle = angleBetweenLines(Line(frames[i].HipRight, frames[i].KneeRight), Line(frames[i].KneeRight, frames[i].AnkleRight));

		frames[i].centerPoint = frames[i].SpineBase + frames[i].SpineMid + frames[i].Neck + frames[i].Head + frames[i].ShoulderLeft + frames[i].ElbowLeft + frames[i].WristLeft + frames[i].HandLeft + frames[i].ShoulderRight + frames[i].ElbowRight + frames[i].WristRight + frames[i].HandRight + frames[i].HipLeft + frames[i].KneeLeft + frames[i].AnkleLeft + frames[i].HipLeft + frames[i].KneeRight + frames[i].AnkleRight;
		frames[i].centerPoint = frames[i].centerPoint / 18;

	}


	

	return 0;
}
