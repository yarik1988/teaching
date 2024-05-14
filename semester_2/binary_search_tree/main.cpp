#include <iostream>
#include <conio.h>
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
            if (val<=cur_elem->data) {
                node *left=add_elem(cur_elem->left, val);
                cur_elem->left =left;
                left->parent = cur_elem;
            }
            else {
                node* right = add_elem(cur_elem->right, val);
                cur_elem->right = right;
                right->parent=cur_elem;
            }
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
            std::cout<<path<<"_"<<cur_elem->data<<"->"<<path+"l"<<"_"<<cur_elem->left->data<<std::endl;
            print_tree_GV_internal(cur_elem->left,path+"l");
            }
        if (cur_elem->right!=nullptr) {
            std::cout<<path<<"_"<<cur_elem->data<<"->"<<path+"r"<<"_"<<cur_elem->right->data<<std::endl;
            print_tree_GV_internal(cur_elem->right,path+"r");
        }

    }
    node* find_elem_internal(node* cur_elem,int val)
    {
        if (cur_elem==nullptr) return nullptr;
        if (cur_elem->data==val) return cur_elem;
        node* left=find_elem_internal(cur_elem->left,val);
        if (left!= nullptr) return left;
        node* right=find_elem_internal(cur_elem->right,val);
        if (right!= nullptr) return right;
        return nullptr;
    }

    void check_tree(node* cur)
    {
        if (cur==nullptr) return;
        if (cur->parent!=nullptr&&(cur->parent->left!=cur&&cur->parent->right!=cur))
        std::cout<<"ERROR: "<<cur->data;
        check_tree(cur->left);
        check_tree(cur->right);
    }

    void left_rotate(node*& cur)
    {
        if (cur->left== nullptr) return;
        node* old_cur=cur;
        node* cur_parent=cur->parent;
        if (cur_parent!=nullptr) {
            if (cur_parent->left == cur)
                cur_parent->left = cur->left;
            else cur_parent->right = cur->left;
        }
        node* B=cur->left->right;
        cur=cur->left;
        cur->right=old_cur;
        old_cur->parent=cur;
        old_cur->left=B;
        if (B!=nullptr) B->parent=old_cur;
        cur->parent=cur_parent;
    }

    void right_rotate(node*& cur)
    {
        if (cur->right==nullptr) return;
        node* old_cur=cur;
        node* cur_parent=cur->parent;
        if (cur_parent!=nullptr) {
            if (cur_parent->left == cur)
                cur_parent->left = cur->right;
            else cur_parent->right = cur->right;
        }
        node* B=cur->right->left;
        cur=cur->right;
        cur->left=old_cur;
        old_cur->parent=cur;
        old_cur->right=B;
        if (B!=nullptr) B->parent=old_cur;
        cur->parent=cur_parent;
    }

    void move_to_root_rec(node* cur)
    {
       while (cur->parent!= nullptr) {
           if (cur->parent->left == cur) {
               left_rotate(cur->parent);
           } else if (cur->parent->right == cur) {
               right_rotate(cur->parent);
           }
          print_tree_GV();
          check_tree();
          std::cout<<"--------------------------------";
          getch();
       }
       root=cur;
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

    void move_to_root(int val)
    {
        node* cur=find_elem_internal(root,val);
        if (cur==nullptr||cur==root) return;
        move_to_root_rec(cur);
    }
    void check_tree()
    {
        check_tree(root);
    }
};


int main() {
    BST my_tree;
    for (int i=10;i<20;i++)
        my_tree.add_elem(rand()%1000);
    my_tree.print_tree_GV();
    my_tree.move_to_root(464);
    my_tree.print_tree_GV();
    return 0;
}
