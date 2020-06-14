#ifndef __COMPARE__
#define __COMPARE__

#include "Rectangle.hpp"
#include "R_Tree.hpp"

// Comparing the intervals by their low value of x-axis
template <typename RTreeType>
int compare_x_low(const void* r1, const void* r2){
    const typename R_Tree<RTreeType>::Node::Record** rec1 = (const typename R_Tree<RTreeType>::Node::Record**) r1;
    const typename R_Tree<RTreeType>::Node::Record** rec2 = (const typename R_Tree<RTreeType>::Node::Record**) r2;

    float rec1_x_low = (*rec1)->bounding_box.get_x_bound().getLow();
    float rec2_x_low = (*rec2)->bounding_box.get_x_bound().getLow();

    if(rec1_x_low < rec2_x_low)
        return -1;
    
    if(rec1_x_low == rec2_x_low)
        return 0;    

    if(rec1_x_low > rec2_x_low)
        return 1;
}

// Comparing the intervals by their high value of x-axis
template <typename RTreeType>
int compare_x_high(const void* r1, const void* r2){
    const typename R_Tree<RTreeType>::Node::Record** rec1 = (const typename R_Tree<RTreeType>::Node::Record**) r1;
    const typename R_Tree<RTreeType>::Node::Record** rec2 = (const typename R_Tree<RTreeType>::Node::Record**) r2;

    float rec1_x_high = (*rec1)->bounding_box.get_x_bound().getHigh();
    float rec2_x_high = (*rec2)->bounding_box.get_x_bound().getHigh();

    if(rec1_x_high < rec2_x_high)
        return -1;
    
    if(rec1_x_high == rec2_x_high)
        return 0;    

    if(rec1_x_high > rec2_x_high)
        return 1;
}

// Comparing the intervals by their low value of y-axis
template <typename RTreeType>
int compare_y_low(const void* r1, const void* r2){
    const typename R_Tree<RTreeType>::Node::Record** rec1 = (const typename R_Tree<RTreeType>::Node::Record**) r1;
    const typename R_Tree<RTreeType>::Node::Record** rec2 = (const typename R_Tree<RTreeType>::Node::Record**) r2;

    float rec1_y_low = (*rec1)->bounding_box.get_y_bound().getLow();
    float rec2_y_low = (*rec2)->bounding_box.get_y_bound().getLow();

    if(rec1_y_low < rec2_y_low)
        return -1;
    
    if(rec1_y_low == rec2_y_low)
        return 0;    

    if(rec1_y_low > rec2_y_low)
        return 1;
}

// Comparing the intervals by their high value of y-axis
template <typename RTreeType>
int compare_y_high(const void* r1, const void* r2){
    const typename R_Tree<RTreeType>::Node::Record** rec1 = (const typename R_Tree<RTreeType>::Node::Record**) r1;
    const typename R_Tree<RTreeType>::Node::Record** rec2 = (const typename R_Tree<RTreeType>::Node::Record**) r2;

    float rec1_y_high = (*rec1)->bounding_box.get_y_bound().getHigh();
    float rec2_y_high = (*rec2)->bounding_box.get_y_bound().getHigh();

    if(rec1_y_high < rec2_y_high)
        return -1;
    
    if(rec1_y_high == rec2_y_high)
        return 0;    

    if(rec1_y_high > rec2_y_high)
        return 1;
}

// Comparing the rectangles by their area enlargements
// It is needed in R*-Tree Choose Subtree
template <typename RTreeType>
int compare_area_enlargements(const void* r1, const void* r2){
    const typename R_Tree<RTreeType>::Node::Record** rec1 = (const typename R_Tree<RTreeType>::Node::Record**) r1;
    const typename R_Tree<RTreeType>::Node::Record** rec2 = (const typename R_Tree<RTreeType>::Node::Record**) r2;

    float rec1_enlargement = (*rec1)->area_enlargement;
    float rec2_enlargement = (*rec2)->area_enlargement;

    if(rec1_enlargement < rec2_enlargement)
        return -1;
    
    if(rec1_enlargement == rec2_enlargement)
        return 0;    

    if(rec1_enlargement > rec2_enlargement)
        return 1;
}

#endif