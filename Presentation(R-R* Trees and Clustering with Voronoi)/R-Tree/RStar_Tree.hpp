#ifndef __RSTAR_TREE__
#define __RSTAR_TREE__

#include <iostream>
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

    if(curr_node->records[0]->child->records[0]->child == NULL){
        for(int i = 0; i < curr_node->curr_records_num; i++){
            Rectangle curr_bb = curr_node->records[i]->bounding_box;
            Rectangle new_bb = new_bounding_box(curr_bb, bounding_box);

            float curr_overlap_enlargement = 0;
            float curr_area_enlargement = new_bb.getArea() - curr_bb.getArea();

            float curr_bb_overlap = 0;
            float new_bb_overlap = 0;

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
        for(int i = 0;i < curr_node->curr_records_num; i++){
            Rectangle curr_bounding_box = curr_node->records[i]->bounding_box; 
            float curr_enlargement = new_bounding_box(bounding_box, curr_bounding_box).getArea() - bounding_box.getArea();

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

        for(int i = node_split_start; i <= node_split_end; i++){
            Rectangle curr_bb = high_val_distribution[0].bounding_box;

            for(int j = 1; j <= i; j++){
                curr_bb = new_bounding_box(curr_bb, high_val_distribution[j].bounding_box);
            }

            curr_margin += curr_bb.getMargin();

            curr_bb = high_val_distribution[i+1].bounding_box;

            for(int j = i+2; j < curr_node->curr_records_num; j++){
                curr_bb = new_bounding_box(curr_bb, high_val_distribution[j].bounding_box);
            }

            curr_margin += curr_bb.getMargin();

            curr_bb = low_val_distribution[0].bounding_box;

            for(int j = 1; j <= i;j++){
                curr_bb = new_bounding_box(curr_bb, low_val_distribution[j].bounding_box);
            }

            curr_margin += curr_bb.getMargin();

            curr_bb = low_val_distribution[i+1].bounding_box;

            for(int j = i+2; j < curr_node->curr_records_num; j++){
                curr_bb = new_bounding_box(curr_bb, low_val_distribution[j].bounding_box);
            }

            curr_margin += curr_bb.getMargin();
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

    // Choosing split index
    int   min_split_index = -1;
    int   split_high = 1;       // If I break high or low distribution
    float min_overlap;
    float min_area;

    int node_split_start = this->getMinRecords() - 1;
    int node_split_end   = node_split_start + (this->getMaxRecords() - (2 * this->getMinRecords()) + 1);

    for(int i = node_split_start; i <= node_split_end; i++){
        Rectangle curr_bb_1 = high_val_distribution[0].bounding_box;

        for(int j = 1; j <= i;j++){
            curr_bb_1 = new_bounding_box(curr_bb_1, high_val_distribution[j].bounding_box);
        }

        Rectangle curr_bb_2 = high_val_distribution[i+1].bounding_box;

        for(int j = i+2; j < curr_node->curr_records_num; j++){
            curr_bb_2 = new_bounding_box(curr_bb_2, high_val_distribution[j].bounding_box);
        }

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

    for(int i = node_split_start; i <= node_split_end; i++){
        Rectangle curr_bb_1 = low_val_distribution[0].bounding_box;

        for(int j = 1; j <= i;j++){
            curr_bb_1 = new_bounding_box(curr_bb_1, low_val_distribution[j].bounding_box);
        }

        Rectangle curr_bb_2 = low_val_distribution[i+1].bounding_box;

        for(int j = i+2; j < curr_node->curr_records_num; j++){
            curr_bb_2 = new_bounding_box(curr_bb_2, low_val_distribution[j].bounding_box);
        }

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

    delete[] high_val_distribution;
    delete[] low_val_distribution;

    return new_nodes;
}

#endif