#include <iostream>

struct elem
{
    int data;
    elem* next;
};

bool is_cyclic(elem* cur)
{
    elem* tmp=cur;
    do {
        cur = cur->next;
    }
    while (cur!=tmp&&cur!=nullptr);
    return (cur!=nullptr);
}

elem* push_after_current(elem* head, int data)
{
        if (head==nullptr)
        {
            head=new elem;
            head->data=data;
            head->next=head;
            return head;
        } else
        {
            elem* elem_after_head=head->next;
            elem* tmp=new elem;
            tmp->data=data;
            tmp->next=elem_after_head;
            head->next=tmp;
            return tmp;
        }
}

elem* delete_after_current(elem* head)
{
    if (head==nullptr) return nullptr;
    elem* to_delete=head->next;
    if (to_delete==head)
    {
        delete to_delete;
        return nullptr;
    } else {
        head->next = to_delete->next;
        delete to_delete;
        return head;
    }
}

void print_list(elem* head)
{
    if (head==nullptr) return;
    std::cout<<"Our list: ";
    elem* tmp=head;
    do  {
        std::cout << head->data<<" ";
        head=head->next;
    } while (head!=tmp&&head!=nullptr);
    std::cout<<std::endl;
}

elem* find_and_delete(elem* head,int val)
{
    if (head==nullptr) return nullptr;
    elem* tmp=head;
    do {
        head = head->next;
    }
    while (head->next->data!=val&&head!=tmp);
    if (head->next->data!=val) return tmp;
    else return delete_after_current(head);
}

elem* break_cyclic(elem* cur)
{
    if (cur==nullptr) return nullptr;
    if (!is_cyclic(cur)) return cur;
    elem* new_head=cur->next;
    cur->next=nullptr;
    return new_head;
}

elem* make_cyclic(elem* cur)
{
    if (is_cyclic(cur)) return cur;
    elem* head=cur;
    while (cur->next!= nullptr) cur=cur->next;
    cur->next=head;
    return head;
}

int main() {
    int k=7;
    elem* cur=nullptr;
    for (int i=0;i<10;i++)
        cur= push_after_current(cur, i);
    cur=break_cyclic(cur);
    print_list(cur);
    std::cout<<is_cyclic(cur)<<std::endl;


    cur=make_cyclic(cur);
    print_list(cur);
    std::cout<<is_cyclic(cur)<<std::endl;

    while (cur!=nullptr)
    {
        for (int i=0;i<k;i++) cur=cur->next;
        std::cout<<cur->next->data<<" ";
        cur=delete_after_current(cur);
    }

    return 0;
}
