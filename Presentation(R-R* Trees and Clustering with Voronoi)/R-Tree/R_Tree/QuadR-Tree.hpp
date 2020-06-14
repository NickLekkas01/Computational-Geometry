#ifndef __QUADRATIC__
#define __QUADRATIC__

#include <cassert>
#include <iostream>
#include "R_Tree.hpp"

using namespace std;

// This is an R-Tree that does quadratic split
template<typename RTreeType2>
class QuadR_Tree : public R_Tree<RTreeType2>
{
private:
    typename R_Tree<RTreeType2>::Node** splitNode(const typename R_Tree<RTreeType2>::Node*);

public:
    QuadR_Tree(const int& min_records,const int& max_records) : R_Tree<RTreeType2>(min_records, max_records){}
    ~QuadR_Tree(){}
};

// This function will do the Guttman Quadratic split of an R-Tree
template<typename RTreeType2>
typename R_Tree<RTreeType2>::Node** QuadR_Tree<RTreeType2>::splitNode(const typename R_Tree<RTreeType2>::Node* curr_node)
{   
    typename R_Tree<RTreeType2>::Node** new_nodes = new typename R_Tree<RTreeType2>::Node*[2]; 

    new_nodes[0] = new typename R_Tree<RTreeType2>::Node(R_Tree<RTreeType2>::getMaxRecords());
    new_nodes[1] = new typename R_Tree<RTreeType2>::Node(R_Tree<RTreeType2>::getMaxRecords());

    int* inserted = new int[curr_node->curr_records_num];

    for(int i = 0; i < curr_node->curr_records_num; i++){
        inserted[i] = 0;
    }

    // Picking the first point of each group
    typename R_Tree<RTreeType2>::Node::Record* max_rec_1 = NULL;
    typename R_Tree<RTreeType2>::Node::Record* max_rec_2 = NULL;
    int max_rec_1_index;
    int max_rec_2_index;

    float max_d;

    for(int i = 0; i < curr_node->curr_records_num; i++){
        typename R_Tree<RTreeType2>::Node::Record* cand_rec_1 = curr_node->records[i];

        for(int j = i + 1; j < curr_node->curr_records_num; j++){
            typename R_Tree<RTreeType2>::Node::Record* cand_rec_2 = curr_node->records[j];

            float curr_d = new_bounding_box(cand_rec_1->bounding_box, cand_rec_2->bounding_box).getArea();
            curr_d -= cand_rec_1->bounding_box.getArea();
            curr_d -= cand_rec_2->bounding_box.getArea();

            if((i == 0 && j == 1) || curr_d > max_d){
                max_rec_1 = cand_rec_1;
                max_rec_2 = cand_rec_2;

                max_rec_1_index = i;
                max_rec_2_index = j;
                
                max_d = curr_d;
            }
        }
    }


    assert(max_rec_2_index != max_rec_1_index);
    assert(max_rec_1 != max_rec_2);

    inserted[max_rec_1_index] = 1;
    inserted[max_rec_2_index] = 1;

    new_nodes[0]->addRecord(max_rec_1);

    if(new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child != NULL){
        new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child->parent_node = new_nodes[0];
        new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child->parent_record = new_nodes[0]->records[new_nodes[0]->curr_records_num - 1];
    }
    
    new_nodes[1]->addRecord(max_rec_2);

    if(new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child != NULL){
        new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child->parent_node = new_nodes[1];
        new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child->parent_record = new_nodes[1]->records[new_nodes[1]->curr_records_num - 1];
    }

    Rectangle group_1_bb = max_rec_1->bounding_box;
    Rectangle group_2_bb = max_rec_2->bounding_box;

    int insert_count = 2;

    for(int i = 2; i < curr_node->curr_records_num;i++){
        // The two cases below are the cases that we should give the rest records to
        // a node because if not will be underfull
        if((new_nodes[0]->curr_records_num < R_Tree<RTreeType2>::getMinRecords()) &&
            ((new_nodes[0]->curr_records_num + (curr_node->curr_records_num - insert_count)) == R_Tree<RTreeType2>::getMinRecords()) )
        {
            for(int j = 0; j < curr_node->curr_records_num;j++){
                if(inserted[j]) continue;

                new_nodes[0]->addRecord(curr_node->records[j]);

                if(new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child != NULL)
                {
                new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child->parent_node = new_nodes[0];
                new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child->parent_record = new_nodes[0]->records[new_nodes[0]->curr_records_num - 1];
                }
            
                inserted[j] = 1;
                insert_count++;
            }
            break;                
        }

        if((new_nodes[1]->curr_records_num < R_Tree<RTreeType2>::getMinRecords()) &&
            ((new_nodes[1]->curr_records_num + (curr_node->curr_records_num - insert_count)) == R_Tree<RTreeType2>::getMinRecords()) )
        {
            for(int j = 0; j < curr_node->curr_records_num;j++){
                if(inserted[j]) continue;

                new_nodes[1]->addRecord(curr_node->records[j]);

                if(new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child != NULL)
                {
                new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child->parent_node = new_nodes[1];
                new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child->parent_record = new_nodes[1]->records[new_nodes[1]->curr_records_num - 1];
                }

                inserted[j] = 1;
                insert_count++;
            }
            break;
        }

        // Picking next(the record that has the maximum difference between the area
        // enlargements of the two groups)
        typename R_Tree<RTreeType2>::Node::Record* next_rec;
        int next_rec_index;
        float max_enlargement_diff = -1.0;
        float max_enlargement_group_1;
        float max_enlargement_group_2;

        for(int j = 0; j < curr_node->curr_records_num; j++){
            if(inserted[j]) continue;

            typename R_Tree<RTreeType2>::Node::Record* curr_rec = curr_node->records[j];

            float group_1_area = group_1_bb.getArea();
            float group_2_area = group_2_bb.getArea();

            float enlargement_group_1 = new_bounding_box(group_1_bb, curr_rec->bounding_box).getArea() - group_1_bb.getArea();
            float enlargement_group_2 = new_bounding_box(group_2_bb, curr_rec->bounding_box).getArea() - group_2_bb.getArea();

            float enlargement_diff = abs(enlargement_group_1 - enlargement_group_2);

            if(enlargement_diff > max_enlargement_diff){
                max_enlargement_diff = enlargement_diff;

                next_rec = curr_rec;
                next_rec_index = j;

                max_enlargement_group_1 = enlargement_group_1;
                max_enlargement_group_2 = enlargement_group_2;
            }
        }

        assert(max_enlargement_diff >= 0.0);

        //Resolving ties in advance
        if(max_enlargement_group_1 == max_enlargement_group_2){
            if(group_1_bb.getArea() < group_2_bb.getArea()){
                max_enlargement_group_1 -= 1.0;
            }else{
                max_enlargement_group_2 -= 1.0;
            }
        }

        // Inserting the max record into a group
        if(max_enlargement_group_1 < max_enlargement_group_2){
            new_nodes[0]->addRecord(next_rec);

            if(new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child != NULL)
            {
            new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child->parent_node = new_nodes[0];
            new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child->parent_record = new_nodes[0]->records[new_nodes[0]->curr_records_num - 1];
            }
            assert(!inserted[next_rec_index]);
            inserted[next_rec_index] = 1;
            insert_count++;

            group_1_bb = new_bounding_box(group_1_bb, next_rec->bounding_box);
        }
        else{
            new_nodes[1]->addRecord(next_rec);

            if(new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child != NULL)
            {
            new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child->parent_node = new_nodes[1];
            new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child->parent_record = new_nodes[1]->records[new_nodes[1]->curr_records_num - 1];
            }
            assert(!inserted[next_rec_index]);
            inserted[next_rec_index] = 1;
            insert_count++;

            group_2_bb = new_bounding_box(group_2_bb, next_rec->bounding_box);     
        }
    }

    for(int i = 0; i < curr_node->curr_records_num;i++){
        assert(inserted[i] == 1);
    }

    assert(insert_count == curr_node->curr_records_num);
    
    assert((new_nodes[0]->curr_records_num + new_nodes[1]->curr_records_num) == curr_node->curr_records_num);
    assert(new_nodes[0]->curr_records_num >= R_Tree<RTreeType2>::getMinRecords());
    assert(new_nodes[1]->curr_records_num >= R_Tree<RTreeType2>::getMinRecords());

    delete[] inserted;

    return new_nodes;
}

#endif