#ifndef __INTERVAL__
#define __INTERVAL__

#include <iostream>
using namespace std;

class Interval{
    float low;
    float high;

public:
    // Now I dont check for special cases
    Interval(float l=0, float h=0): low(l), high(h){}

    float getLow() const{ return low;}
    float getHigh() const{return high;}
    float getLength() const{ return high - low + 1;}
    
    void setLow(const float& l){low = l;}
    void setHigh(const float& h){ high = h;}
};

int have_intersection(const Interval& i1, const Interval& i2){
    const float i1_low = i1.getLow();
    const float i1_high = i1.getHigh();

    const float i2_low = i2.getLow();
    const float i2_high = i2.getHigh();

    if((i1_low >= i2_low) && (i1_low <= i2_high)){
        return 1;
    }

    if((i1_high >= i2_low) && (i1_high <= i2_high)){
        return 1;
    }

    if((i1_low < i2_low) && (i1_high > i2_high)){
        return 1;
    }

    return 0;
}

const Interval union_interval(const Interval& i1, const Interval& i2){
    const float i1_low = i1.getLow();
    const float i1_high = i1.getHigh();

    const float i2_low = i2.getLow();
    const float i2_high = i2.getHigh();

    const float new_low  = (i1_low <= i2_low) ? i1_low : i2_low;
    const float new_high = (i1_high >= i2_high) ? i1_high : i2_high;

    return Interval(new_low, new_high);
}

const Interval intersect_interval(const Interval& i1, const Interval& i2){
    const float i1_low = i1.getLow();
    const float i1_high = i1.getHigh();

    const float i2_low = i2.getLow();
    const float i2_high = i2.getHigh();

    float intersect_low, intersect_high;

    if(!have_intersection(i1,i2)){
        cout << "You should check already if the intervals have intersection" << endl;
        return Interval(-1,-1);
    }
    
    if(i1_low >= i2_low){
        intersect_low = i1_low;
    }
    else{
        intersect_low = i2_low;     
    }

    if(i1_high <= i2_high){
        intersect_high = i1_high;
    }else{
        intersect_high = i2_high;
    }
}

bool operator==(const Interval& i1, const Interval& i2){
    return (i1.getHigh() == i2.getHigh()) && (i1.getLow() == i2.getLow());
}
#endif