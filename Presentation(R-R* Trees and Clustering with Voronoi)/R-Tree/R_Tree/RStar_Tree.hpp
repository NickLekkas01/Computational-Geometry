#ifndef __RSTAR_TREE__
#define __RSTAR_TREE__

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <algorithm>

#include "CompareFns.hpp"
#include "R_Tree.hpp"

using namespace std;

template <typename RTreeType>
class RStar_Tree : public R_Tree<RTreeType>
{
private:
    int ChooseSubtree(const typename R_Tree<RTreeType>::Node*, const Rectangle&);
    typename R_Tree<RTreeType>::Node** OverflowTreatment(const typename R_Tree<RTreeType>::Node* curr_node, int first = 0){/*cout << "R star called over\n";*/ return splitNode(curr_node);} // This will be for now
    typename R_Tree<RTreeType>::Node** splitNode(const typename R_Tree<RTreeType>::Node*);

public:
    RStar_Tree(const int& min_records,const int& max_records) : R_Tree<RTreeType>(min_records, max_records){}
    ~RStar_Tree(){}
};

template<typename RTreeType>
int RStar_Tree<RTreeType>::ChooseSubtree(const typename R_Tree<RTreeType>::Node* curr_node, const Rectangle& bounding_box){
    int next_leaf = -1;
    float min_area_enlargement, min_overlap_enlargement;

    // If nodes on the nex level are leaves
    if(curr_node->records[0]->child->records[0]->child == NULL){
        // Getting the area enlargement of each record's bounding box
        for(int i = 0; i < curr_node->curr_records_num; i++){
            Rectangle curr_bounding_box = curr_node->records[i]->bounding_box; 
            float curr_enlargement = new_bounding_box(bounding_box, curr_bounding_box).getArea() - curr_bounding_box.getArea();

            curr_node->records[i]->area_enlargement = curr_enlargement;
        }

        const int nodes_to_check = 32;

        // Sorting the nodes by their area enlargement, in increasing order
        qsort(curr_node->records, curr_node->curr_records_num, sizeof(typename R_Tree<RTreeType>::Node::Record*), compare_area_enlargements<RTreeType>);

        // For the first nodes_to_check nodes we check the overlap enlargement
        // and take the node with the minimum overlap enlargement. Resolve ties
        // by choosing the node with the minimum area enlargement  
        for(int i = 0; (i < nodes_to_check) && (i < curr_node->curr_records_num); i++){
            Rectangle curr_bb = curr_node->records[i]->bounding_box;
            Rectangle new_bb = new_bounding_box(curr_bb, bounding_box);

            float curr_overlap_enlargement = 0;
            float curr_area_enlargement = new_bb.getArea() - curr_bb.getArea();

            float curr_bb_overlap = 0;
            float new_bb_overlap = 0;

            // Finding the current overlap and new overlap with all node records and
            // take their difference to get the overlap enlargement
            for(int j = 0; j < curr_node->curr_records_num; j++){
                if(j != i){
                    curr_bb_overlap += overlap(curr_bb, curr_node->records[j]->bounding_box);
                    new_bb_overlap  += overlap(new_bb,  curr_node->records[j]->bounding_box);
                }
            }

            curr_overlap_enlargement = new_bb_overlap - curr_bb_overlap;

            if((i == 0) || (curr_overlap_enlargement < min_overlap_enlargement)){
                next_leaf = i;
                min_area_enlargement = curr_area_enlargement;
                min_overlap_enlargement = curr_overlap_enlargement;
            }else if((curr_overlap_enlargement == min_overlap_enlargement) 
                    && (curr_area_enlargement < min_area_enlargement))
            {
                next_leaf = i;
                min_area_enlargement = curr_area_enlargement;
            }
        }
    }else{
        // if childs are not leaves we do the same procedure as the R-Tree
        for(int i = 0;i < curr_node->curr_records_num; i++){
            Rectangle curr_bounding_box = curr_node->records[i]->bounding_box; 
            float curr_enlargement = new_bounding_box(bounding_box, curr_bounding_box).getArea() - curr_bounding_box.getArea();

            if(i == 0){
                next_leaf = i;
                min_area_enlargement = curr_enlargement;
            }
            else{
                if(curr_enlargement < min_area_enlargement){
                    next_leaf = i;
                    min_area_enlargement = curr_enlargement;
                } else if(curr_enlargement == min_area_enlargement){
                    if(curr_node->records[next_leaf]->bounding_box.getArea() > curr_bounding_box.getArea()){
                        next_leaf = i;
                    }
                }
            }
        }

    }

    return next_leaf;
}

// R*-Tree node splitting
template <typename RTreeType>
typename R_Tree<RTreeType>::Node** RStar_Tree<RTreeType>::splitNode(const typename R_Tree<RTreeType>::Node* curr_node)
{
    typename R_Tree<RTreeType>::Node** new_nodes = new typename R_Tree<RTreeType>::Node*[2]; 

    new_nodes[0] = new typename R_Tree<RTreeType>::Node(R_Tree<RTreeType>::getMaxRecords());
    new_nodes[1] = new typename R_Tree<RTreeType>::Node(R_Tree<RTreeType>::getMaxRecords());

    //cout << "R star called split\n";
    
    // Choosing split axis
    int min_axis = -1;
    float min_margin;

    float curr_margin = 0;

    typename R_Tree<RTreeType>::Node::Record* high_val_distribution = new typename R_Tree<RTreeType>::Node::Record[curr_node->curr_records_num];
    typename R_Tree<RTreeType>::Node::Record* low_val_distribution  = new typename R_Tree<RTreeType>::Node::Record[curr_node->curr_records_num];

    for(int curr_axis = 0; curr_axis < 2; curr_axis++){
        curr_margin = 0;

        // Sorting the records by their low and high value along the axis
        if(curr_axis == 0){
            qsort(curr_node->records, curr_node->curr_records_num, sizeof(typename R_Tree<RTreeType>::Node::Record*), compare_x_high<RTreeType>);
            
            for(int i = 0; i < curr_node->curr_records_num; i++){
                high_val_distribution[i] = *(curr_node->records[i]);
            }

            qsort(curr_node->records, curr_node->curr_records_num, sizeof(typename R_Tree<RTreeType>::Node::Record*), compare_x_low<RTreeType>);
            
            for(int i = 0; i < curr_node->curr_records_num; i++){
                low_val_distribution[i] = *(curr_node->records[i]);
            }
        }
        else{
            qsort(curr_node->records, curr_node->curr_records_num, sizeof(typename R_Tree<RTreeType>::Node::Record*), compare_y_high<RTreeType>);
            
            for(int i = 0; i < curr_node->curr_records_num; i++){
                high_val_distribution[i] = *(curr_node->records[i]);
            }

            qsort(curr_node->records, curr_node->curr_records_num, sizeof(typename R_Tree<RTreeType>::Node::Record*), compare_y_low<RTreeType>);
            
            for(int i = 0; i < curr_node->curr_records_num; i++){
                low_val_distribution[i] = *(curr_node->records[i]);
            }            
        }


        int node_split_start = this->getMinRecords() - 1;
        int node_split_end   = node_split_start + (this->getMaxRecords() - (2 * this->getMinRecords()) + 1);

        /* I do this in order to not determine always the bounding boxes from the start */
        Rectangle high_bb_from_start = high_val_distribution[0].bounding_box;

        for(int j = 1; j <= node_split_start; j++){
            high_bb_from_start = new_bounding_box(high_bb_from_start, high_val_distribution[j].bounding_box);
        }

        Rectangle high_bb_from_end = high_val_distribution[this->getMaxRecords()].bounding_box;

        for(int j = this->getMaxRecords() - 1; j > node_split_end; j--){
            high_bb_from_end = new_bounding_box(high_bb_from_end, high_val_distribution[j].bounding_box);
        }

        Rectangle low_bb_from_start = low_val_distribution[0].bounding_box;

        for(int j = 1; j <= node_split_start; j++){
            low_bb_from_start = new_bounding_box(low_bb_from_start, low_val_distribution[j].bounding_box);
        }

        Rectangle low_bb_from_end = low_val_distribution[this->getMaxRecords()].bounding_box;

        for(int j = this->getMaxRecords() - 1; j > node_split_end; j--){
            low_bb_from_end = new_bounding_box(low_bb_from_end, low_val_distribution[j].bounding_box);
        }      
        /* ****************************************************************** */

        // Finding the total margin of all distributions in order to
        // determine the split axis, which is the one that has 
        // minimum margin
        for(int i = node_split_start; i <= node_split_end; i++){
            high_bb_from_start = new_bounding_box(high_bb_from_start, high_val_distribution[i].bounding_box);
            curr_margin += high_bb_from_start.getMargin();

            low_bb_from_start = new_bounding_box(low_bb_from_start, low_val_distribution[i].bounding_box);

            curr_margin += low_bb_from_start.getMargin();
        }
        
        for(int i = node_split_end+1; i > node_split_start; i--){
            high_bb_from_end = new_bounding_box(high_bb_from_end, high_val_distribution[i].bounding_box);

            curr_margin += high_bb_from_end.getMargin();

            low_bb_from_end = new_bounding_box(low_bb_from_end, low_val_distribution[i].bounding_box);

            curr_margin += low_bb_from_end.getMargin();
        }

        if(min_axis == -1){
            min_axis = curr_axis;
            min_margin = curr_margin;
        }else if(curr_margin < min_margin){
            min_axis = curr_axis;
            min_margin = curr_margin;
        }
    }

    int split_axis = (min_axis == 0) ? 1 : 0; // getting the axis perpendicular to min_axis

    // Sorting again the records by their low and high value along split axis
    if(split_axis == 0){
        qsort(curr_node->records, curr_node->curr_records_num, sizeof(typename R_Tree<RTreeType>::Node::Record*), compare_x_high<RTreeType>);
        
        for(int i = 0; i < curr_node->curr_records_num; i++){
            high_val_distribution[i] = *(curr_node->records[i]);
        }

        qsort(curr_node->records, curr_node->curr_records_num, sizeof(typename R_Tree<RTreeType>::Node::Record*), compare_x_low<RTreeType>);
        
        for(int i = 0; i < curr_node->curr_records_num; i++){
            low_val_distribution[i] = *(curr_node->records[i]);
        }
    }
    else{
        qsort(curr_node->records, curr_node->curr_records_num, sizeof(typename R_Tree<RTreeType>::Node::Record*), compare_y_high<RTreeType>);
        
        for(int i = 0; i < curr_node->curr_records_num; i++){
            high_val_distribution[i] = *(curr_node->records[i]);
        }

        qsort(curr_node->records, curr_node->curr_records_num, sizeof(typename R_Tree<RTreeType>::Node::Record*), compare_y_low<RTreeType>);
        
        for(int i = 0; i < curr_node->curr_records_num; i++){
            low_val_distribution[i] = *(curr_node->records[i]);
        }            
    }

    // Choosing split index, by choosing the distribution has the minimum
    // overlap value. Ties are resolved by choosing the distribution with minimum
    // area value.

    int   min_split_index = -1;
    int   split_high = 1;       // If I break high or low distribution
    float min_overlap;
    float min_area;

    int node_split_start = this->getMinRecords() - 1;
    int node_split_end   = node_split_start + (this->getMaxRecords() - (2 * this->getMinRecords()) + 1);

    /* I do this in order to not determine the bounding boxes from the start */
    Rectangle high_bb_from_start = high_val_distribution[0].bounding_box;

    for(int j = 1; j <= node_split_start; j++){
        high_bb_from_start = new_bounding_box(high_bb_from_start, high_val_distribution[j].bounding_box);
    }

    Rectangle high_bb_from_end = high_val_distribution[this->getMaxRecords()].bounding_box;

    for(int j = this->getMaxRecords() - 1; j > node_split_end; j--){
        high_bb_from_end = new_bounding_box(high_bb_from_end, high_val_distribution[j].bounding_box);
    }

    Rectangle low_bb_from_start = low_val_distribution[0].bounding_box;

    for(int j = 1; j <= node_split_start; j++){
        low_bb_from_start = new_bounding_box(low_bb_from_start, low_val_distribution[j].bounding_box);
    }

    Rectangle low_bb_from_end = low_val_distribution[this->getMaxRecords()].bounding_box;

    for(int j = this->getMaxRecords() - 1; j > node_split_end; j--){
        low_bb_from_end = new_bounding_box(low_bb_from_end, low_val_distribution[j].bounding_box);
    }

    // I store the rectangles of the 2nd group
    Rectangle* high_end_rectangles = new Rectangle[node_split_end - node_split_start + 1];
    Rectangle* low_end_rectangles   = new Rectangle[node_split_end - node_split_start + 1];

    for(int i = node_split_end - node_split_start, j=node_split_end; i >= 0; i--,j--){
        high_end_rectangles[i] = high_bb_from_end;
        low_end_rectangles[i]  = low_bb_from_end;

        high_bb_from_end = new_bounding_box(high_bb_from_end, high_val_distribution[j].bounding_box);
        low_bb_from_end  = new_bounding_box(low_bb_from_end, low_val_distribution[j].bounding_box);
    }

    /* ****************************************************************** */

    // Checking the "high" distribution
    for(int i = node_split_start; i <= node_split_end; i++){
        Rectangle curr_bb_1 = high_bb_from_start;

        high_bb_from_start = new_bounding_box(high_bb_from_start, high_val_distribution[i+1].bounding_box);

        Rectangle curr_bb_2 = high_end_rectangles[i - node_split_start];

        float curr_overlap = overlap(curr_bb_1, curr_bb_2);

        if((min_split_index == -1) || (curr_overlap < min_overlap)){
            min_overlap = curr_overlap;
            min_area = curr_bb_1.getArea() + curr_bb_2.getArea();

            split_high = 1;
            min_split_index = i;
        }
        else if(curr_overlap == min_overlap){
            if((curr_bb_1.getArea() + curr_bb_2.getArea()) < min_area){
                min_overlap = curr_overlap;
                min_area = curr_bb_1.getArea() + curr_bb_2.getArea();

                split_high = 1;
                min_split_index = i;                
            }
        }
    }

    // Checking the "low" distribution
    for(int i = node_split_start; i <= node_split_end; i++){
        Rectangle curr_bb_1 = low_bb_from_start;

        low_bb_from_start = new_bounding_box(low_bb_from_start, low_val_distribution[i].bounding_box);

        Rectangle curr_bb_2 = low_end_rectangles[i - node_split_start];

        float curr_overlap = overlap(curr_bb_1, curr_bb_2);

        if((min_split_index == -1) || (curr_overlap < min_overlap)){
            min_overlap = curr_overlap;
            min_area = curr_bb_1.getArea() + curr_bb_2.getArea();

            split_high = 0;
            min_split_index = i;
        }
        else if(curr_overlap == min_overlap){
            if((curr_bb_1.getArea() + curr_bb_2.getArea()) < min_area){
                min_overlap = curr_overlap;
                min_area = curr_bb_1.getArea() + curr_bb_2.getArea();

                split_high = 0;
                min_split_index = i;                
            }
        }
    }

    // Splitting the records at min_split_index
    if(split_high){
        for(int i = 0; i <= min_split_index; i++){
            new_nodes[0]->addRecord(&high_val_distribution[i]);

            if(new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child != NULL){
                new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child->parent_node = new_nodes[0];
                new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child->parent_record = new_nodes[0]->records[new_nodes[0]->curr_records_num - 1];
            }
        }

        for(int i = min_split_index + 1; i < curr_node->curr_records_num; i++){
            new_nodes[1]->addRecord(&high_val_distribution[i]);

            if(new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child != NULL){
                new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child->parent_node = new_nodes[1];
                new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child->parent_record = new_nodes[1]->records[new_nodes[1]->curr_records_num - 1];
            }
        }
    }
    else{
        for(int i = 0; i <= min_split_index; i++){
            new_nodes[0]->addRecord(&low_val_distribution[i]);

            if(new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child != NULL){
                new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child->parent_node = new_nodes[0];
                new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child->parent_record = new_nodes[0]->records[new_nodes[0]->curr_records_num - 1];
            }
        }

        for(int i = min_split_index + 1; i < curr_node->curr_records_num; i++){
            new_nodes[1]->addRecord(&low_val_distribution[i]);  

            if(new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child != NULL){
                new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child->parent_node = new_nodes[1];
                new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child->parent_record = new_nodes[1]->records[new_nodes[1]->curr_records_num - 1];
            }
        }      
    }

    delete[] high_end_rectangles;
    delete[] low_end_rectangles;
    delete[] high_val_distribution;
    delete[] low_val_distribution;

    return new_nodes;
}

#endif