#include <iostream>
struct node
{
    node* left,*right,*parent;
    int data;
    node():left(nullptr),right(nullptr),parent(nullptr),data(0)   {}
    node(int data):left(nullptr),right(nullptr),parent(nullptr),data(data){}
};

class BST
{
    node* root;
    node* add_elem(node*& cur_elem,int val)
    {
        if (cur_elem == nullptr)
            cur_elem=new node(val);
        else
            if (val<=cur_elem->data) cur_elem->left=add_elem(cur_elem->left,val);
            else cur_elem->right=add_elem(cur_elem->right,val);
        return cur_elem;
    }
    void print_tree_sym(node* cur_elem, int depth)
    {
        if (cur_elem== nullptr) return;
        print_tree_sym(cur_elem->left,depth+1);
        std::cout<<cur_elem->data<<"["<<depth<<"]";
        print_tree_sym(cur_elem->right,depth+1);
    }
    void print_tree_GV_internal(node* cur_elem,std::string path)
    {
        if (cur_elem->left!=nullptr) {
            std::cout<<path<<cur_elem->data<<"->"<<path+"l"<<cur_elem->left->data<<std::endl;
            print_tree_GV_internal(cur_elem->left,path+"l");
            }
        if (cur_elem->right!=nullptr) {
            std::cout<<path<<cur_elem->data<<"->"<<path+"r"<<cur_elem->right->data<<std::endl;
            print_tree_GV_internal(cur_elem->right,path+"r");
        }

    }
    bool find_elem_internal(node* cur_elem,int val)
    {
        if (cur_elem==nullptr) return false;
        if (cur_elem->data==val) return true;
        else return find_elem_internal(cur_elem->left,val)||find_elem_internal(cur_elem->right,val);
    }
    public:
    BST(): root(nullptr){};
    void add_elem(int val)
    {
        add_elem(root, val);
    }
    void print_tree()
    {
        print_tree_sym(root,0);
    }
    void print_tree_GV()
    {
        std::cout<<"digraph G {"<<std::endl;
        print_tree_GV_internal(root,"");
        std::cout<<"}"<<std::endl;
    }
    bool find_elem(int val)
    {
        return find_elem_internal(root,val);
    }
};


int main() {
    BST my_tree;
    for (int i=10;i<20;i++)
        my_tree.add_elem((i*i*i)%101);
    my_tree.print_tree_GV();
    std::cout<<my_tree.find_elem(43);
    return 0;
}
