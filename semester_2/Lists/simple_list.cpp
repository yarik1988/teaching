#include <iostream>

class List {
    struct Node {
        int data;
        Node* next;
        Node(int val, Node* n = nullptr) : data(val), next(n) {}
    };
    Node* head;

public:
    List() : head(nullptr) {}

    // Destructor: Automatically cleans up memory when the List object goes out of scope
    ~List() {
        while (head != nullptr) {
            const Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    void push_front(const int data) {
        head = new Node(data, head);
    }

    void find_and_delete(const int val) {
        if (head == nullptr) return;

        if (head->data == val) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
            return;
        }

        Node* current = head;
        while (current->next != nullptr && current->next->data != val) {
            current = current->next;
        }

        if (current->next != nullptr) {
            const Node* to_delete = current->next;
            current->next = current->next->next;
            delete to_delete;
        }
    }

    void print() const {
        std::cout << "Our list: ";
        const Node* tmp = head;
        while (tmp != nullptr) {
            std::cout << tmp->data << " ";
            tmp = tmp->next;
        }
        std::cout << std::endl;
    }
};

int main() {
    List myList;

    for (int i = 0; i < 10; i++) {
        myList.push_front(i);
    }
    myList.print();
    myList.find_and_delete(9);
    myList.print();
    return 0;
}