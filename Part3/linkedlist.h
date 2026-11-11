#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "node.h"

#include <utility>

// Do not add any #include statements here.  If you have a convincing need for adding a different `#include` please post in the forum on KEATS.

// TODO your code goes here:

template<typename T>
class LinkedList {

private:

    Node<T>* head;
    Node<T>* tail;
    int count;

public:

    LinkedList()
        : head(nullptr), tail(nullptr), count(0) {
    }

    LinkedList(std::initializer_list<T> list)
        : head(nullptr), tail(nullptr), count(0) {
        for(const T & item : list) {
            push_back(item);
        }
    }

    LinkedList(const LinkedList<T> & other)
        : head(nullptr), tail(nullptr), count(0) {
        for(const T & item : other) {
            push_back(item);
        }
    }

    ~LinkedList() {
        int i = 0;
        while(i < count) {
            Node<T>* nextNode = head->next;
            delete head;
            head = nextNode;
            ++i;
        }
    }

    void push_front(const T & item) {
        if(count == 0) {
            head = tail = new Node<T>(item);
        }
        else {
            Node<T>* newNode = new Node<T>(item);
            head->previous = newNode;
            newNode->next = head;
            head = newNode;
        }
        ++count;
    }

    void push_back(const T & item) {
        if(count == 0) {
            head = tail = new Node<T>(item);
        }
        else {
            Node<T>* newNode = new Node<T>(item);
            tail->next = newNode;
            newNode->previous = tail;
            tail = newNode;
        }
        ++count;
    }

    NodeIterator<T> insert(NodeIterator<T> itr, const T & item) {
        Node<T>* currentNode = itr.getNode();
        if(itr == end()) {
            push_back(item);
            return NodeIterator<T>(tail);
        }
        else if(itr == begin()) {
            push_front(item);
            return NodeIterator<T>(head);
        }
        else {
            Node<T>* newNode = new Node<T>(item);
            Node<T>* previousNode = currentNode->previous;
            newNode->next = currentNode;
            newNode->previous = previousNode;
            previousNode->next = newNode;
            currentNode->previous = newNode;
            ++count;
            return NodeIterator<T>(newNode);
        }
    }

    NodeIterator<T> erase(NodeIterator<T> itr) {
        Node<T>* currentNode = itr.getNode();
        if(currentNode == nullptr) {return end();}
        if(head == tail) {
            delete currentNode;
            --count;
            tail = head = nullptr;
            return end();
        }
        if(currentNode == tail) {
            Node<T>* previousNode = currentNode->previous;
            delete currentNode;
            previousNode->next = nullptr;
            tail = previousNode;
            --count;
            return end();
        }
        else if(itr == begin()) {
            Node<T>* nextNode = currentNode->next;
            delete currentNode;
            nextNode->previous = nullptr;
            head = nextNode;
            --count;
            return begin();
        }
        else {
            Node<T>* previousNode = currentNode->previous;
            Node<T>* nextNode = currentNode->next;
            previousNode->next = nextNode;
            nextNode->previous = previousNode;
            delete currentNode;
            --count;
            return NodeIterator<T>(nextNode);
        }
    }

    T & front() {
        return head->data;
    }

    T & back() {
        return tail->data;
    }

    const T & front() const{
        return head->data;
    }

    const T & back() const{
        return tail->data;
    }

    int size() const {
        return count;
    }

    NodeIterator<T> begin() {
        return NodeIterator<T>(head);
    }

    NodeIterator<T> end() {
        return NodeIterator<T>(nullptr);
    }

    ConstNodeIterator<T> begin() const{
        return ConstNodeIterator<T>(head);
    }

    ConstNodeIterator<T> end() const{
        return ConstNodeIterator<T>(nullptr);
    }

    void reverse() {
        int i = 0;
        Node<T>* currentNode = head;
        while(i<count) {
            Node<T>* nextNode = currentNode->next;
            currentNode->next = currentNode->previous;
            currentNode->previous = nextNode;
            currentNode = nextNode;
            ++i;
        }
        std::swap(head, tail);
    }

};


// do not edit below this line

#endif
