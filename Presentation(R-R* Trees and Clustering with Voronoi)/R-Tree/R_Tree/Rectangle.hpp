#ifndef __RECTANGLE__
#define __RECTANGLE__

#include "Interval.hpp"

class Rectangle{
    Interval x_axis_bound;
    Interval y_axis_bound;

public:
    Rectangle(){}
    Rectangle(const Interval& x_bound, const Interval& y_bound): x_axis_bound(x_bound), y_axis_bound(y_bound){} 
    const Interval& get_x_bound() const{ return x_axis_bound; }
    const Interval& get_y_bound() const{ return y_axis_bound; } 
    float getXlength() const{ return x_axis_bound.getLength();}
    float getYlength() const{ return y_axis_bound.getLength(); }
    float getArea() const{return x_axis_bound.getLength() * y_axis_bound.getLength();}   
    float getMargin() const{ return (2 * x_axis_bound.getLength()) + (2 * y_axis_bound.getLength()); }
};

int have_intersection(const Rectangle& r1, const Rectangle& r2){
    return (have_intersection(r1.get_x_bound(),r2.get_x_bound()) &&
            have_intersection(r1.get_y_bound(), r2.get_y_bound()));
}

const Rectangle new_bounding_box(const Rectangle& r1, const Rectangle& r2){
    const Interval& r1_x_bound = r1.get_x_bound();
    const Interval& r1_y_bound = r1.get_y_bound();
    const Interval& r2_x_bound = r2.get_x_bound();
    const Interval& r2_y_bound = r2.get_y_bound();

    return Rectangle(union_interval(r1_x_bound,r2_x_bound),union_interval(r1_y_bound, r2_y_bound));
}

const Rectangle intersect_rectangle(const Rectangle& r1, const Rectangle& r2){
    const Interval& r1_x_bound = r1.get_x_bound();
    const Interval& r1_y_bound = r1.get_y_bound();
    const Interval& r2_x_bound = r2.get_x_bound();
    const Interval& r2_y_bound = r2.get_y_bound();

    return Rectangle(intersect_interval(r1_x_bound, r2_x_bound), intersect_interval(r1_y_bound, r2_y_bound));
}

float overlap(const Rectangle& r1, const Rectangle& r2){
    if(!have_intersection(r1,r2))
        return 0;
    
    return intersect_rectangle(r1, r2).getArea();
}

bool operator==(const Rectangle& r1,const Rectangle& r2){
    return (r1.get_x_bound() == r2.get_x_bound()) && (r1.get_y_bound() == r2.get_y_bound());
}

#endif