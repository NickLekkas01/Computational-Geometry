#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <unistd.h>
#include "R_Tree.hpp"

using namespace std;

#define POINTS 200000

struct Point{
    float x,y;

    Point(float x=0, float y=0): x(x), y(y){}
};

bool operator==(const Point& p1, const Point& p2){
    return (p1.x == p2.x) && (p1.y == p2.y);
}

float distance(Point p1, Point p2){
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

ostream& operator<<(ostream& stream, const Point& p){
    stream << "(" << p.x << "," << p.y << ")";
    return stream;
}

float avg(float* nums, int size){
    float sum = 0;

    for(int i = 0; i < size; i++)
        sum += nums[i];

    return sum / (float) size;
}

int main(void){
    srand(time(NULL));

    float* points_x = new float[POINTS];
    float* points_y = new float[POINTS];

    for(int i = 0; i < POINTS; i++){
        points_x[i] = rand() % 200;
        points_y[i] = rand() % 200; 
    }

    R_Tree<Point> r_tree(2,3);

    for(int i = 0; i < POINTS; i++){
        if(i % 1 == 0)
            cout << i<< endl;

        Rectangle r(Interval(points_x[i],points_x[i]), Interval(points_y[i],points_y[i]));

        r_tree.Insert(r,Point(points_x[i],points_y[i]));
    }

    cout << "Insertions end" << endl;

    for(int i = 0; i < POINTS; i++){
        if(i % 1 == 0)
            cout << i<< endl;

        Rectangle r(Interval(points_x[i],points_x[i]), Interval(points_y[i],points_y[i]));

        r_tree.Delete(r,Point(points_x[i],points_y[i]));
    }

    delete[] points_y;
    delete[] points_x;

    return 0;
}