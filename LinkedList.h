// Copyright 2019 Topala Andrei

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
#include <stdio.h>
#include <stdlib.h>

template<typename T> struct list_elem {
    T info;
    struct list_elem<T> *next, *prev;
};

template <typename T> class LinkedList {
 private:
    struct list_elem<T> *pfirst, *plast;
 public:
    list_elem<T> * front();
    LinkedList();
    void addFirst(T x);
    void addLast(T x);
    void removeFirst();
    bool isEmpty();
};

#endif  // LINKEDLIST_H_
