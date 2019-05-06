// Copyright 2019 Topala Andrei
#ifndef HASHMAP_H_
#define HASHMAP_H_
#include "LinkedList.cpp"

template<typename Tkey, typename Tvalue> struct elem_info {
    Tkey key;
    Tvalue value;
};

template<typename Tkey, typename Tvalue> class Hashtable {
 private:
    LinkedList<struct elem_info<Tkey, Tvalue> > *H;
    int HMAX;
    int (*hash)(Tkey);

 public:
    Hashtable(int hmax, int (*h)(Tkey));
    ~Hashtable();
    void put(Tkey key, Tvalue value);
    Tvalue get(Tkey key);
    bool hasKey(Tkey key);
};

#endif  // HASHMAP_H_
