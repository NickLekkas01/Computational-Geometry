#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

#include "../R_Tree/QuadR-Tree.hpp"
#include "../R_Tree/RStar_Tree.hpp"

using namespace std;

#define POINTS 500000
//#define R_MAX_ENTRIES 102
//#define R_MIN_ENTRIES 2
//#define R_STAR_MIN_ENTRIES 41

#define RADIUS 10000
#define RANGE_QUERIES 10000

struct Range_Query_Result{
    float time_avg;
    int pages_touched_avg;
    int points_checked_avg;
};

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

Range_Query_Result range_queries(R_Tree<Point>*, float*, float*);
void tree_insert(R_Tree<Point>*, float*, float*);

int main(void){
    srand(time(NULL));

    float* points_x = new float[POINTS];
    float* points_y = new float[POINTS];
    //int* deleted = new int[POINTS];

    for(int i = 0; i < POINTS; i++){
        points_x[i] = rand() % 1000000;
        points_y[i] = rand() % 1000000; 
        //deleted[i] = 0;
    }

    int max_entries[9] = {10,50,100,150,200,250,300,400,500};

    for(int i = 0;i < 9; i++){
        RStar_Tree<Point>* rstar_40_tree = new RStar_Tree<Point>((int) (0.4*max_entries[i]), max_entries[i]);
        RStar_Tree<Point>* rstar_half_tree = new RStar_Tree<Point>((int) (0.5*max_entries[i]), max_entries[i]);

        R_Tree<Point>* r_2_tree = new R_Tree<Point>(2, max_entries[i]);
        R_Tree<Point>* r_half_tree = new R_Tree<Point>((int) (0.5*max_entries[i]), max_entries[i]);

        QuadR_Tree<Point>* r_quad_2_tree = new QuadR_Tree<Point>(2, max_entries[i]);
        QuadR_Tree<Point>* r_quad_half_tree = new QuadR_Tree<Point>((int) (0.5*max_entries[i]), max_entries[i]);

        clock_t start = clock();
        tree_insert(rstar_40_tree, points_x, points_y);
        float rstar_40_time = (float)(clock() - start) / CLOCKS_PER_SEC;
        Range_Query_Result results = range_queries(rstar_40_tree,points_x,points_y);
        cout << "R*-Tree with m=" << 0.4*max_entries[i] << " and M=" << max_entries[i] << endl;
        cout << "Insert took " << rstar_40_time / 60 << " minutes" << endl;
        cout << "Tree has " << rstar_40_tree->PagesNum() << " pages" << endl;
        cout << "Average Range Query Time " << results.time_avg << " seconds" << endl;
        cout << "Range Query touched " << results.pages_touched_avg << " pages on average." << endl;

        cout << endl << "-----------------------------------------------------------" << endl << endl;

        delete rstar_40_tree;

        start = clock();
        tree_insert(rstar_half_tree, points_x, points_y);
        float rstar_half_time = (float)(clock() - start) / CLOCKS_PER_SEC;

        results = range_queries(rstar_half_tree,points_x,points_y);
        cout << "R*-Tree with m=" << 0.5*max_entries[i] << " and M=" << max_entries[i] << endl;
        cout << "Insert took " << rstar_half_time / 60 << " minutes" << endl;
        cout << "Tree has " << rstar_half_tree->PagesNum() << " pages" << endl;
        cout << "Average Range Query Time is " << results.time_avg << " seconds" << endl;
        cout << "Range Query touched " << results.pages_touched_avg << " pages on average." << endl;

        cout << endl << "-----------------------------------------------------------" << endl << endl;

        delete rstar_half_tree;

        start = clock();
        tree_insert(r_2_tree, points_x, points_y);
        float r_2_time = (float)(clock() - start) / CLOCKS_PER_SEC;

        results = range_queries(r_2_tree,points_x,points_y);
        cout << "R-Tree with Linear Split, m=" << 2 << " and M=" << max_entries[i] << endl;
        cout << "Insert took " << r_2_time / 60 << " minutes" << endl;
        cout << "Tree has " << r_2_tree->PagesNum() << " pages" << endl;
        cout << "Average Range Query Time is " << results.time_avg << " seconds" << endl;
        cout << "Range Query touched " << results.pages_touched_avg << " pages on average." << endl;

        cout << endl << "-----------------------------------------------------------" << endl << endl;

        delete r_2_tree;

        start = clock();
        tree_insert(r_half_tree, points_x, points_y);
        float r_half_time = (float)(clock() - start) / CLOCKS_PER_SEC;

        results = range_queries(r_half_tree,points_x,points_y);
        cout << "R-Tree with Linear Split, m=" << 0.5*max_entries[i] << " and M=" << max_entries[i] << endl;
        cout << "Insert took " << r_half_time / 60 << " minutes" << endl;
        cout << "Tree has " << r_half_tree->PagesNum() << " pages" << endl;
        cout << "Average Range Query Time is " << results.time_avg << " seconds" << endl;
        cout << "Range Query touched " << results.pages_touched_avg << " pages on average." << endl;

        cout << endl << "-----------------------------------------------------------" << endl << endl;

        delete r_half_tree;

        start = clock();
        tree_insert(r_quad_2_tree, points_x, points_y);
        float r_quad_2_time = (float)(clock() - start) / CLOCKS_PER_SEC;

        results = range_queries(r_quad_2_tree,points_x,points_y);
        cout << "R-Tree with Quadratic Split, m=" << 2 << " and M=" << max_entries[i] << endl;
        cout << "Insert took " << r_quad_2_time / 60 << " minutes" << endl;
        cout << "Tree has " << r_quad_2_tree->PagesNum() << " pages" << endl;
        cout << "Average Range Query Time is " << results.time_avg << " seconds" << endl;
        cout << "Range Query touched " << results.pages_touched_avg << " pages on average." << endl;

        cout << endl << "-----------------------------------------------------------" << endl << endl;

        delete r_quad_2_tree;

        start = clock();
        tree_insert(r_quad_half_tree, points_x, points_y);
        float r_quad_half_time = (float)(clock() - start) / CLOCKS_PER_SEC;

        results = range_queries(r_quad_half_tree,points_x,points_y);
        cout << "R-Tree with Quadratic Split, m=" << 0.5*max_entries[i] << " and M=" << max_entries[i] << endl;
        cout << "Insert took " << r_quad_half_time / 60 << " minutes" << endl;
        cout << "Tree has " << r_quad_half_tree->PagesNum() << " pages" << endl;
        cout << "Average Range Query Time is " << results.time_avg << " seconds" << endl;
        cout << "Range Query touched " << results.pages_touched_avg << " pages on average." << endl;

        cout << endl << "-----------------------------------------------------------" << endl << endl;

        delete r_quad_half_tree;
    }

    long n_points = 0;
    clock_t start = clock();

    for(int i = 0; i < RANGE_QUERIES;i++){
        //if(i % 500 == 0) cout << i << endl;
        Point query_point = Point(points_x[i],points_y[i]);
        for(int j = 0; j < POINTS; j++){
            Point tmp = Point(points_x[j],points_y[j]);
            if(distance(query_point, tmp) <= RADIUS /*&& deleted[j] == 0*/){
                //cout << "N " << tmp << endl;
                n_points++;
            }
        }
    }

    clock_t end = clock();
    //cout << "Naive found " << n_points << " points" << endl ;
    cout << "Naive took " << (float)(end - start) / CLOCKS_PER_SEC << " seconds on average" << endl;

    delete[] points_y;
    delete[] points_x;
    //delete[] deleted;
    return 0;
}

void tree_insert(R_Tree<Point>* tree, float* points_x, float* points_y){
    for(int i = 0; i < POINTS; i++){
        //if(i % 50000 == 0)
        //    cout << i<< endl;

        Rectangle r(Interval(points_x[i],points_x[i]), Interval(points_y[i],points_y[i]));

        tree->Insert(r,Point(points_x[i],points_y[i]));
    }

    return;
}

Range_Query_Result range_queries(R_Tree<Point>* tree, float* points_x, float* points_y){
    //long r_points = 0;
    long r_checked = 0;
    long p_touch = 0;
    clock_t start = clock();

    for(int i = 0; i < RANGE_QUERIES;i++){
        int temp_touched = 0;
        //if(i % 1 == 0) cout << i << endl;
        Point query_point = Point(points_x[i],points_y[i]);
        Rectangle query(Interval(points_x[i] - RADIUS, points_x[i] + RADIUS), Interval(points_y[i] - RADIUS, points_y[i] + RADIUS));

        vector<Point> res = tree->Search(query,temp_touched);
        r_checked += res.size();

        for(int j = 0; j < res.size(); j++){
            if(distance(query_point,res[j]) <= RADIUS){
                //r_points++;
                //cout << "R " << res[i] << endl;
            }
        }

        p_touch += temp_touched;
    }

    float total_time = (float)(clock() - start) / CLOCKS_PER_SEC;

    Range_Query_Result result;
    result.time_avg = total_time / RANGE_QUERIES;
    result.pages_touched_avg = p_touch / RANGE_QUERIES;
    result.points_checked_avg = r_checked / RANGE_QUERIES;

    return result;
}