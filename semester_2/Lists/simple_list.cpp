#include <iostream>

struct elem
{
    int data;
    elem* next;
};

elem* push_after_current(elem* head, int data)
{
        if (head==nullptr)
        {
            head=new elem;
            head->data=data;
            head->next=nullptr;
            return head;
        } else
        {
            elem* tmp=new elem;
            tmp->data=data;
            tmp->next=head;
            return tmp;
        }
}

elem* delete_after_current(elem* head)
{
    if (head==nullptr) return nullptr;
    elem* tmp=head;
    head=head->next;
    delete tmp;
    return head;
}

void print_list(elem* head)
{
    std::cout<<"Our list: ";
    while (head!=nullptr) {
        std::cout << head->data<<" ";
        head=head->next;
    }
    std::cout<<std::endl;
}

elem* find_and_delete(elem* head,int val)
{
    if (head==nullptr) return nullptr;
    if (head->data==val) return delete_after_current(head);
    elem* head_copy=head;
    while (head->next!=NULL&&head->next->data!=val) head=head->next;
    if (head->next!=nullptr) {
        elem *to_delete = head->next;
        head->next = head->next->next;
        delete to_delete;
    }
    return head_copy;
}


int main() {
    elem* head=nullptr;
    for (int i=0;i<10;i++)
        head= push_after_current(head, i);
    print_list(head);
    head=find_and_delete(head,9);
    //head=delete_after_current(head);
    print_list(head);
    return 0;
}
