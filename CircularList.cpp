#include <iterator>
#include <bits/stdc++.h>
#define dim 2
using ll = long long;
using namespace std;

struct vertex {
    ll idx;
    ll point[dim];
    bool ear;
    friend std::ostream& operator<<(std::ostream& os, const vertex& v) {
        os << "Vertex " << v.idx << ": (";
        for (int i = 0; i < dim - 1; ++i) {
            os << v.point[i] << ", ";
        }
        os << v.point[dim - 1] << ") Ear: " << std::boolalpha << v.ear;
        return os;
    }

    bool operator==(vertex b) const{
        return idx == b.idx;
    }

    bool operator!=(vertex b) const{
        return !(*this == b);
    }
};

template <typename T>
class CircularList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        explicit Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
    };

    Node* head;

public:
    template <typename U>
    class Iterator {
    private:
        Node* current;
        CircularList* list;

    public:
        Iterator(Node* node, CircularList* lst) : current(node), list(lst) {}

        U& operator*() const {
            return current->data;
        }

        U* operator->() const {
            return &(current->data);
        }

        Iterator<U> nxt() const {
            return Iterator<U>();
        }

        Iterator<U> prv() const {
            return Iterator<U>();
        }

        // 전위 증가 연산자 (++it)
        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        // 전위 감소 연산자 (--it)
        Iterator& operator--() {
            current = current->prev;
            return *this;
        }

        // 후위 증가 연산자 (it++)
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        // 후위 감소 연산자 (it--)
        Iterator operator--(int) {
            Iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        // 값 삽입
        void insert(const U& value) {
            Node* newNode = new Node(value);
            newNode->next = current->next;
            newNode->prev = current;
            current->next->prev = newNode;
            current->next = newNode;
        }

        // 값 삭제
        void remove() {
            Node* nextNode = current->next;
            list->remove(current->data);
            current = nextNode;
        }

        // 값 수정
        void modify(const U& value) {
            current->data = value;
        }
    };

    CircularList() : head(nullptr) {}

    // 값 삽입
    void insert(const T& value) {
        Node* newNode = new Node(value);

        if (!head) {
            head = newNode;
            head->next = head;
            head->prev = head;
        } else {
            newNode->next = head;
            newNode->prev = head->prev;
            head->prev->next = newNode;
            head->prev = newNode;
        }
    }

    // 값 삭제
    void remove(const T& value) {
        if (!head)
            return;

        Node* current = head;
        do {
            if (current->data == value) {
                if (current->next == current) {
                    // 리스트에 하나의 노드만 있는 경우
                    delete current;
                    head = nullptr;
                } else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;

                    if (current == head)
                        head = current->next;

                    delete current;
                }

                return;
            }

            current = current->next;
        } while (current != head);
    }

    Iterator<T> begin() const {
        return Iterator<T>();
    }

    Iterator<T> end() const {
        return Iterator<T>();
    }

    void display() const {
        if (!head)
            return;

        Node* current = head;
        do {
            std::cout << (current->data) << " ";
            current = current->next;
        } while (current != head);

        std::cout << std::endl;
    }
};