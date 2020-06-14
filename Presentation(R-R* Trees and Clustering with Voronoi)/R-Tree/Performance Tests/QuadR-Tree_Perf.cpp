#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

#include "../R_Tree/QuadR-Tree.hpp"

using namespace std;

#define POINTS 500000
#define R_MAX_ENTRIES 10
#define R_MIN_ENTRIES 2
#define RADIUS 10000
#define RANGE_QUERIES 10000

struct Point{
    float x,y;

    Point(float x=0, float y=0): x(x), y(y){}
};

float distance(Point p1, Point p2){
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

bool operator==(const Point& p1, const Point& p2){
    return (p1.x == p2.x) && (p1.y == p2.y);
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
    int* deleted = new int[POINTS];

    for(int i = 0; i < POINTS; i++){
        points_x[i] = rand() % 1000000;
        points_y[i] = rand() % 1000000; 
        deleted[i] = 0;
    }

    QuadR_Tree<Point> r_tree(R_MIN_ENTRIES,R_MAX_ENTRIES);

    for(int i = 0; i < POINTS; i++){
        if(i % 50000 == 0)
            cout << i<< endl;

        Rectangle r(Interval(points_x[i],points_x[i]), Interval(points_y[i],points_y[i]));

        r_tree.Insert(r,Point(points_x[i],points_y[i]));
    }

/*
    float x_query = avg(points_x, POINTS);
    float y_query = avg(points_y, POINTS);
    Point query_point = Point(x_query,y_query);

    float radius = 10000;

    Point p(160051520,2040332928);

    Rectangle query(Interval(x_query - radius, x_query + radius), Interval(y_query - radius, y_query + radius));
    */

    float radius = RADIUS;

    clock_t start = clock();
    long r_points = 0;
    long r_checked = 0;
    long p_touch = 0;

    for(int i = 0; i < RANGE_QUERIES;i++){
        int temp_touched = 0;
        if(i % 50 == 0) cout << i << endl;
        Point query_point = Point(points_x[i],points_y[i]);
        Rectangle query(Interval(points_x[i] - radius, points_x[i] + radius), Interval(points_y[i] - radius, points_y[i] + radius));

        vector<Point> res = r_tree.Search(query,temp_touched);
        r_checked += res.size();

        for(int j = 0; j < res.size(); j++){
            if(distance(query_point,res[j]) <= radius){
                r_points++;
                //cout << "R " << res[i] << endl;
            }
        }

        p_touch += temp_touched;
    }
    clock_t end = clock();
    cout << "R_Tree touched " << p_touch << " pages" << endl;
    cout << "R_Tree checked " << r_checked << " points." << endl;
    cout << "R_Tree found " << r_points << " points" << endl;
    cout << "R_Tree took " << (float)(end - start) / CLOCKS_PER_SEC << " seconds" << endl << endl;

    long n_points = 0;
    start = clock();

    for(int i = 0; i < RANGE_QUERIES;i++){
        if(i % 50 == 0) cout << i << endl;
        Point query_point = Point(points_x[i],points_y[i]);
        for(int j = 0; j < POINTS; j++){
            Point tmp = Point(points_x[j],points_y[j]);
            if(distance(query_point, tmp) <= radius && deleted[j] == 0){
                //cout << "N " << tmp << endl;
                n_points++;
            }
        }
    }
    end = clock();
    cout << "Naive found " << n_points << " points" << endl ;
    cout << "Naive took " << (float)(end - start) / CLOCKS_PER_SEC << " seconds" << endl;

    delete[] points_y;
    delete[] points_x;
    delete[] deleted;
    return 0;
}
