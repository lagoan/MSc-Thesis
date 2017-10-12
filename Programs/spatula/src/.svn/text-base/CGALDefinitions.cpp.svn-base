
#include "CGALDefinitions.h"

Point_3::Point_3() {
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
}

Point_3::Point_3(double x, double y, double z) {
	data[0] = x;
	data[1] = y;
	data[2] = z;
}

double & Point_3::x() {
	return data[0];
}

double & Point_3::y() {
	return data[1];
}

double & Point_3::z() {
	return data[2];
}

double Point_3::getDistanceTo(const Point_3& p){
	
	double a = pow(data[0] - p.x(), 2.0);
	double b = pow(data[1] - p.y(), 2.0);
	double c = pow(data[2] - p.z(), 2.0);
	
	return sqrt(a + b + c);
	
}

double Point_3::dotP(const Point_3& b){
	return data[0] * b.x() + data[1] * b.y() + data[2] * b.z();
}