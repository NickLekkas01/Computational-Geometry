#ifndef __R_TREE__
#define __R_TREE__

#include<cmath>
#include <vector>

//#include "CompareFns.hpp"
#include "Rectangle.hpp"

using namespace std;

// This R-tree does linear split

template<typename RTreeType>
class R_Tree {
protected:
    struct Node{
        struct Record{
            Rectangle bounding_box;
            Node* child;    // If node isn't a leaf node
            RTreeType object; // If node is a leaf node
            
            float area_enlargement; // This attribute is used to do the sort in R*-Tree ChooseSubtree

            Record(){}
            Record(const Rectangle& bb, Node* ch, const RTreeType& data): bounding_box(bb), child(ch), object(data){}

            Record operator=(const Record& other){
                bounding_box = other.bounding_box;
                child        = other.child;
                object       = other.object;
            }

        };
        
        Record* parent_record;
        Node* parent_node;
        int curr_records_num;
        Record** records;

        Node(const int& max_cap, Node* parent_node = NULL, Record* parent = NULL){
            curr_records_num = 0;
            records = new Record*[max_cap + 1];

            for(int i = 0; i <= max_cap; i++)
                records[i] = NULL;
            
            this->parent_node = parent_node;
            this->parent_record = parent;
        }

        void addRecord(const Rectangle& bounding_box,Node* child, const RTreeType& data){
            records[curr_records_num] = new Record(bounding_box, child, data);
            curr_records_num++;
            return;
        }

        void addRecord(const Record* new_record){
            records[curr_records_num] = new Record(*new_record);

            curr_records_num++;
        }
        
        void deleteRecord(const int& index){
            delete records[index];

            for(int i = index; i < curr_records_num; i++){
                records[i] = records[i+1];
            }

            curr_records_num--;
        }

        ~Node(){
            for(int i = 0;i < curr_records_num;i++) delete records[i];

            delete[] records;
        }


    };

    int getMaxRecords() const{ return max_node_records; }
    int getMinRecords() const{ return min_node_records; }

private:
    Node* root;

    int min_node_records;
    int max_node_records;

    const vector<RTreeType> search(Node*, const Rectangle&,int&);
    const long count_pages(Node*);

    int delete_rec(Node*, const Rectangle&, const RTreeType&, vector<typename Node::Record*>&);

    virtual int ChooseSubtree(const Node*, const Rectangle&);
    virtual Node** OverflowTreatment(const Node* curr_node, int first = 0);
    virtual Node** splitNode(const Node*);

    int tree_height(Node* curr_node) const{
        Node* curr_nd = curr_node;
        int curr = -1;

        while(curr_nd != NULL){
            curr++;
            curr_nd = curr_nd->records[0]->child;
        }

        return curr;
    }
    
    int Reinsert(typename Node::Record*);          //It is used for reinsertions

    void tree_destroy(Node*);

public:
    R_Tree(const int&,const int&);
    const vector<RTreeType> Search(const Rectangle&, int&);
    const long PagesNum();
    int Insert(const Rectangle&, const RTreeType&);
    int Delete(const Rectangle&, const RTreeType&);
    //const vector<RtreeType>& Range_Query(const Rectangle&);
    ~R_Tree();

    template<typename RTreeType2>
    friend int compare_x_low(const void* r1, const void* r2);
    template<typename RTreeType2>
    friend int compare_y_low(const void* r1, const void* r2);
    template<typename RTreeType2>
    friend int compare_x_high(const void* r1, const void* r2);
    template<typename RTreeType2>
    friend int compare_y_high(const void* r1, const void* r2);
    template <typename RTreeType2>
    friend int compare_area_enlargements(const void* r1, const void* r2);
};

template<typename RTreeType>
R_Tree<RTreeType>::R_Tree(const int& min_recs, const int& max_recs){
    root = NULL;
    min_node_records = min_recs;
    max_node_records = max_recs;
}

// Helper method of Search
template<typename RTreeType>
const vector<RTreeType> R_Tree<RTreeType>::search(Node* curr_node, const Rectangle& bounding_box,int& pages){
    pages++;
    vector<RTreeType> return_vector;

    if(curr_node->records[0]->child == NULL){    // If node is leaf
        for(int i = 0; i < curr_node->curr_records_num; i++){
            if(have_intersection(bounding_box, curr_node->records[i]->bounding_box)){   // If the search b.b. have intersection with record's b.b
                return_vector.push_back(curr_node->records[i]->object);                 // I put it into the result vector 
            }
        }

        return return_vector;
    }
    else{
        for(int i = 0; i < curr_node->curr_records_num; i++){
            if(have_intersection(bounding_box, curr_node->records[i]->bounding_box)){           // If the search b.b. have intersection with record's b.b
                vector<RTreeType> temp = search(curr_node->records[i]->child, bounding_box,pages);  // I go to child node by recursion

                return_vector.insert(return_vector.end(),temp.begin(),temp.end()); // concatenating
            }
        }

        return return_vector;
    }
}

template<typename RTreeType>
const vector<RTreeType> R_Tree<RTreeType>::Search(const Rectangle& bounding_box,int& pages_touched){
    vector<RTreeType> empty;

    if(root == NULL){
        return empty;
    }

    return search(root, bounding_box, pages_touched);
}

template<typename RTreeType>
const long R_Tree<RTreeType>::count_pages(typename R_Tree<RTreeType>::Node* curr_node){
    long return_val = 0;

    if(curr_node == NULL) return 0;

    for(int i = 0; i < curr_node->curr_records_num; i++){
        return_val += count_pages(curr_node->records[i]->child);
    }

    return (return_val + 1);
}

template<typename RTreeType>
const long R_Tree<RTreeType>::PagesNum(){
    if(root == NULL) return 0;

    return count_pages(root);
}

template<typename RTreeType>
int R_Tree<RTreeType>::ChooseSubtree(const typename R_Tree<RTreeType>::Node* curr_node, const Rectangle& bounding_box){
    int next_leaf = -1;
    float min_area_enlargement = -1;

    // Simple gets the area enlargement of each record and returns the index of the
    // record that needs least are enlargement
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

    return next_leaf;
}

template<typename RTreeType>
typename R_Tree<RTreeType>::Node** R_Tree<RTreeType>::OverflowTreatment(const typename R_Tree<RTreeType>::Node* curr_node, int first){
    return splitNode(curr_node);
}

// I'm gonna write the Linear split.
template<typename RTreeType>
typename R_Tree<RTreeType>::Node** R_Tree<RTreeType>::splitNode(const R_Tree<RTreeType>::Node* curr_node){
    Node** new_nodes = new Node*[2];

    new_nodes[0] = new Node(max_node_records);
    new_nodes[1] = new Node(max_node_records);

    int* inserted = new int[curr_node->curr_records_num];

    for(int i = 0; i < curr_node->curr_records_num; i++){
        inserted[i] = 0;
    }

    // Finding total width along each axis by unifying the root rectangles
    Rectangle curr_rectangle = root->records[0]->bounding_box;

    for(int i = 1; i < root->curr_records_num; i++){
        curr_rectangle = new_bounding_box(curr_rectangle, root->records[i]->bounding_box);
    }

    const float total_x_width = curr_rectangle.getXlength();
    const float total_y_width = curr_rectangle.getYlength();

    // Picking Seeds
    typename Node::Record* seed_1;
    int index_1;
    typename Node::Record* seed_2;
    int index_2;

    float max_normalized_separation;

    for(int dim = 1; dim <= 2; dim++){
        typename Node::Record* temp_seed_1;
        int temp_index_1;
        typename Node::Record* temp_seed_2;
        int temp_index_2;

        float highest_low;
        float lowest_high;
     
        for(int i = 0; i < curr_node->curr_records_num; i++){
            Rectangle curr_box = curr_node->records[i]->bounding_box;

            float curr_low = (dim == 1) ? curr_box.get_x_bound().getLow() : curr_box.get_y_bound().getLow();
            float curr_high = (dim == 1) ? curr_box.get_x_bound().getHigh() : curr_box.get_y_bound().getHigh();

            if(i == 0){
                highest_low = curr_low;
                lowest_high = curr_high;

                temp_seed_1 = curr_node->records[i];
                temp_seed_2 = curr_node->records[i];
                temp_index_1 = temp_index_2 = i;
            } else{
                if(highest_low < curr_low){
                    highest_low = curr_low;
                    temp_seed_1 = curr_node->records[i];
                    temp_index_1 = i;
                }

                if(lowest_high > curr_high){
                    lowest_high = curr_high;
                    temp_seed_2 = curr_node->records[i];
                    temp_index_2 = i;
                }
            }
        }

        float separation = (float) abs(lowest_high - highest_low);
        float curr_normalized_sep = (dim == 1) ? separation / total_x_width : separation / total_y_width;

        if(dim == 1){
            seed_1 = temp_seed_1;
            seed_2 = temp_seed_2;
            index_1 = temp_index_1;
            index_2 = temp_index_2;

            max_normalized_separation = curr_normalized_sep;
        }
        else if(curr_normalized_sep > max_normalized_separation){
            seed_1 = temp_seed_1;
            seed_2 = temp_seed_2;
            index_1 = temp_index_1;
            index_2 = temp_index_2;

            max_normalized_separation = curr_normalized_sep;
        }
    }

    // We just add the rest of the points to the nodes
    new_nodes[0]->addRecord(seed_1);
    
    if(new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child != NULL){
        new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child->parent_node = new_nodes[0];
        new_nodes[0]->records[new_nodes[0]->curr_records_num - 1]->child->parent_record = new_nodes[0]->records[new_nodes[0]->curr_records_num - 1];
    }

    if(index_1 != index_2){
        new_nodes[1]->addRecord(seed_2);

        if(new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child != NULL){
            new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child->parent_node = new_nodes[1];
            new_nodes[1]->records[new_nodes[1]->curr_records_num - 1]->child->parent_record = new_nodes[1]->records[new_nodes[1]->curr_records_num - 1];
        }
    }
    
    inserted[index_1] = 1;
    inserted[index_2] = 1;

    int to_add = (index_1 != index_2) ? 0 : 1;
    
    for(int i = 0; i < curr_node->curr_records_num; i++){
        if(inserted[i]) continue;

        new_nodes[to_add]->addRecord(curr_node->records[i]);

        if(new_nodes[to_add]->records[new_nodes[to_add]->curr_records_num - 1]->child != NULL){
            new_nodes[to_add]->records[new_nodes[to_add]->curr_records_num - 1]->child->parent_node = new_nodes[to_add];
            new_nodes[to_add]->records[new_nodes[to_add]->curr_records_num - 1]->child->parent_record = new_nodes[to_add]->records[new_nodes[to_add]->curr_records_num - 1];
        }

        to_add ^= 1;
    }

    //cout << "SPLIT " << ((new_nodes[0]->curr_records_num + new_nodes[1]->curr_records_num) == 4) << endl;
    if(!((new_nodes[0]->curr_records_num + new_nodes[1]->curr_records_num) == curr_node->curr_records_num)){
        cout << "SPLIT 0 " << new_nodes[0]->curr_records_num << " " << new_nodes[1]->curr_records_num << " ";
        cout << curr_node->curr_records_num << (index_1 == index_2) << endl;
    }
    delete[] inserted;
    return new_nodes;
}

// Insert function
template<typename RTreeType>
int R_Tree<RTreeType>::Insert(const Rectangle& bounding_box, const RTreeType& data){

    if(root == NULL){
        root = new R_Tree<RTreeType>::Node(max_node_records);
        root->addRecord(bounding_box,NULL,data);

        return 0;
    }

    typename R_Tree<RTreeType>::Node* toAdd_Leaf = root;

    /* Choose Leaf */
    while(toAdd_Leaf->records[0]->child != NULL){
        int next_child_index = ChooseSubtree(toAdd_Leaf,bounding_box);
        toAdd_Leaf = toAdd_Leaf->records[next_child_index]->child;
    }

    /* Adding the record to leaf */
    typename R_Tree<RTreeType>::Node** new_nodes;

    toAdd_Leaf->addRecord(bounding_box,NULL,data);

    if(toAdd_Leaf->curr_records_num > max_node_records){
        new_nodes = OverflowTreatment(toAdd_Leaf);
        new_nodes[0]->parent_node = toAdd_Leaf->parent_node;
        new_nodes[0]->parent_record = toAdd_Leaf->parent_record;

        delete toAdd_Leaf;
    }else{
        new_nodes = new typename R_Tree<RTreeType>::Node*[2];
        new_nodes[0] = toAdd_Leaf;
        new_nodes[1] = NULL;
    }

    /* Adjusting tree */
    typename R_Tree<RTreeType>::Node* curr_node = new_nodes[0]->parent_node;
    typename R_Tree<RTreeType>::Node::Record* curr_record = new_nodes[0]->parent_record; 

    while(curr_node != NULL){
        // Adjusting the bounding box of N and fix the pointers
        curr_record->child = new_nodes[0];

        Rectangle curr_bounding_box = curr_record->child->records[0]->bounding_box;

        for(int i = 1; i < curr_record->child->curr_records_num; i++){
            curr_bounding_box = new_bounding_box(curr_bounding_box, curr_record->child->records[i]->bounding_box);
        }

        curr_record->bounding_box = curr_bounding_box;

        new_nodes[0] = curr_node;

        // Check if NN exists
        if(new_nodes[1] != NULL){               // if NN exists

            // getting the bounding box of the new record
            curr_bounding_box = new_nodes[1]->records[0]->bounding_box;

            for(int i = 1; i < new_nodes[1]->curr_records_num; i++){
                curr_bounding_box = new_bounding_box(curr_bounding_box, new_nodes[1]->records[i]->bounding_box);
            }

            curr_node->addRecord(curr_bounding_box, new_nodes[1], RTreeType());

            curr_node->records[curr_node->curr_records_num -1]->child->parent_node = curr_node;
            curr_node->records[curr_node->curr_records_num -1]->child->parent_record = curr_node->records[curr_node->curr_records_num -1];

            // Checking if a split is needed 
            if(curr_node->curr_records_num > max_node_records){
                delete[] new_nodes;
                new_nodes = OverflowTreatment(curr_node);
                new_nodes[0]->parent_node = curr_node->parent_node;
                new_nodes[0]->parent_record = curr_node->parent_record;

                delete curr_node;
            }
            else{
                new_nodes[1] = NULL;
            }
        }else{                      // If NN does not exists we simple do nothing
            new_nodes[1] = NULL;
        }

        // Going upwards in the tree
        curr_node = new_nodes[0]->parent_node;
        curr_record = new_nodes[0]->parent_record; 
    }

    // The root was splitted
    if(new_nodes[1] != NULL){
        root = new typename R_Tree<RTreeType>::Node(max_node_records);

        for(int i = 0; i < 2; i++){
            Rectangle curr_bounding_box = new_nodes[i]->records[0]->bounding_box;

            for(int j = 1; j < new_nodes[i]->curr_records_num; j++){
                curr_bounding_box = new_bounding_box(curr_bounding_box, new_nodes[i]->records[j]->bounding_box);
            }

            root->addRecord(curr_bounding_box, new_nodes[i], RTreeType());

            new_nodes[i]->parent_node = root;
            new_nodes[i]->parent_record = root->records[root->curr_records_num - 1];            
        }
    }else{
        root = new_nodes[0];
    }

    delete[] new_nodes;
    return 0;
}

// Is the same as insert with only difference that
// the chooseSubtree is stopped calling when we get
// to the proper height
template<typename RTreeType>
int R_Tree<RTreeType>::Reinsert(typename Node::Record* record){
    if(root == NULL){
        cout << "Should not happen" << endl;
        root = new R_Tree<RTreeType>::Node(max_node_records);
        root->addRecord(record);

        return 0;
    }

    typename R_Tree<RTreeType>::Node* toAdd_Leaf = root;

    /* Choose Leaf */
    while(tree_height(toAdd_Leaf->records[0]->child) != tree_height(record->child)){
        int next_child_index = ChooseSubtree(toAdd_Leaf,record->bounding_box);
        toAdd_Leaf = toAdd_Leaf->records[next_child_index]->child;
    }

    /* Adding the record to leaf */
    typename R_Tree<RTreeType>::Node** new_nodes;

    toAdd_Leaf->addRecord(record);
    if(record->child != NULL)

    if(record->child != NULL){
        record->child->parent_node = toAdd_Leaf;
        record->child->parent_record = toAdd_Leaf->records[toAdd_Leaf->curr_records_num - 1];
    }

    if(toAdd_Leaf->curr_records_num > max_node_records){
        new_nodes = OverflowTreatment(toAdd_Leaf);
        new_nodes[0]->parent_node = toAdd_Leaf->parent_node;
        new_nodes[0]->parent_record = toAdd_Leaf->parent_record;

        delete toAdd_Leaf;
    }else{
        new_nodes = new typename R_Tree<RTreeType>::Node*[2];
        new_nodes[0] = toAdd_Leaf;
        new_nodes[1] = NULL;
    }

    /* Adjusting tree */
    typename R_Tree<RTreeType>::Node* curr_node = new_nodes[0]->parent_node;
    typename R_Tree<RTreeType>::Node::Record* curr_record = new_nodes[0]->parent_record; 

    while(curr_node != NULL){
        curr_record->child = new_nodes[0];

        Rectangle curr_bounding_box = curr_record->child->records[0]->bounding_box;

        for(int i = 1; i < curr_record->child->curr_records_num; i++){
            curr_bounding_box = new_bounding_box(curr_bounding_box, curr_record->child->records[i]->bounding_box);
        }

        curr_record->bounding_box = curr_bounding_box;

        new_nodes[0] = curr_node;

        if(new_nodes[1] != NULL){
            curr_bounding_box = new_nodes[1]->records[0]->bounding_box;

            for(int i = 1; i < new_nodes[1]->curr_records_num; i++){
                curr_bounding_box = new_bounding_box(curr_bounding_box, new_nodes[1]->records[i]->bounding_box);
            }

            curr_node->addRecord(curr_bounding_box, new_nodes[1], RTreeType());

            curr_node->records[curr_node->curr_records_num -1]->child->parent_node = curr_node;
            curr_node->records[curr_node->curr_records_num -1]->child->parent_record = curr_node->records[curr_node->curr_records_num -1];

            if(curr_node->curr_records_num > max_node_records){
                delete[] new_nodes;
                new_nodes = OverflowTreatment(curr_node);
                new_nodes[0]->parent_node = curr_node->parent_node;
                new_nodes[0]->parent_record = curr_node->parent_record;

                delete curr_node;
            }
            else{
                new_nodes[1] = NULL;
            }
        }else{
            new_nodes[1] = NULL;
        }

        curr_node = new_nodes[0]->parent_node;
        curr_record = new_nodes[0]->parent_record; 
    }

    // The root was splitted
    if(new_nodes[1] != NULL){
        root = new typename R_Tree<RTreeType>::Node(max_node_records);

        for(int i = 0; i < 2; i++){
            Rectangle curr_bounding_box = new_nodes[i]->records[0]->bounding_box;

            for(int j = 1; j < new_nodes[i]->curr_records_num; j++){
                curr_bounding_box = new_bounding_box(curr_bounding_box, new_nodes[i]->records[j]->bounding_box);
            }

            root->addRecord(curr_bounding_box, new_nodes[i], RTreeType());

            new_nodes[i]->parent_node = root;
            new_nodes[i]->parent_record = root->records[root->curr_records_num - 1];            
        }
    }else{
        root = new_nodes[0];
    }

    delete[] new_nodes;
    return 0;
}

template<typename RTreeType>
// returns 0 if record was deleted, else 1(probably because record wasn't found)
int R_Tree<RTreeType>::delete_rec(Node* curr_node, const Rectangle& bounding_box, 
                        const RTreeType& data, vector<typename Node::Record*>& to_reinsert)
{   //if(curr_node == root) cout << tree_height(curr_node) << endl;
    if(curr_node->records[0]->child == NULL){
        for(int i = 0; i < curr_node->curr_records_num; i++){
            if((curr_node->records[i]->bounding_box == bounding_box) 
                && (curr_node->records[i]->object == data))
            {
                curr_node->deleteRecord(i);
                return 0;
            }
        }

        return 1;
    }

    int found = 0;
    int next_leaf = -1;
    for(int i = 0; i < curr_node->curr_records_num; i++){
        if(have_intersection(curr_node->records[i]->bounding_box, bounding_box)){
            if(delete_rec(curr_node->records[i]->child, bounding_box, data, to_reinsert) == 0){
                next_leaf = i;
                found = 1;
                break;
            }
        }
    }

    if(!found) return 1;

    int child_records_num = curr_node->records[next_leaf]->child->curr_records_num;
    Node* child_node = curr_node->records[next_leaf]->child;

    if(child_records_num < min_node_records){
        for(int i = 0; i < child_records_num;i++){
            typename Node::Record* to_insert = new typename Node::Record(*(child_node->records[i]));
            to_reinsert.push_back(to_insert);
        }

        delete child_node;

        curr_node->deleteRecord(next_leaf);
    } else{
        Rectangle curr_bounding_box = child_node->records[0]->bounding_box;

        for(int i = 0;i < child_records_num; i++){
            curr_bounding_box = new_bounding_box(curr_bounding_box, child_node->records[i]->bounding_box);
        }

        curr_node->records[next_leaf]->bounding_box = curr_bounding_box;
    }

    return 0;
}

template<typename RTreeType>
int R_Tree<RTreeType>::Delete(const Rectangle& bounding_box, const RTreeType& data){
    vector<typename Node::Record*> reinsert_records;
    int result = R_Tree<RTreeType>::delete_rec(root, bounding_box, data, reinsert_records);

    if(result) return 1;

    for(int i = 0; i < reinsert_records.size(); i++){
        this->Reinsert(reinsert_records[i]);
    }

    for(int i = 0; i < reinsert_records.size(); i++){
        delete reinsert_records[i];
    }

    if(root->curr_records_num == 0){
        delete root;
        root = NULL;
        return 0;
    }

    if(root->curr_records_num == 1){
        if(root->records[0]->child == NULL){
            return 0;
        }

        Node* temp = root;
        root = root->records[0]->child;
        root->parent_node = NULL;
        root->parent_record = NULL;
        delete temp;

        return 0;
    }
}

template<typename RTreeType>
void R_Tree<RTreeType>::tree_destroy(Node* curr_node){
    if(curr_node->records[0]->child == NULL){
        delete curr_node;
        return;
    }

    for(int i = 0; i < curr_node->curr_records_num; i++){
        tree_destroy(curr_node->records[i]->child);
    }

    delete curr_node;
}

template<typename RTreeType>
R_Tree<RTreeType>::~R_Tree(){
    if(root != NULL)
        tree_destroy(root);
}

#endif