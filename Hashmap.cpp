// Copyright 2019 Topala Andrei
#include "./Hashmap.h"

template<typename Tkey, typename Tvalue>
Hashtable<Tkey, Tvalue>::Hashtable(int hmax, int (*h)(Tkey)) {
	HMAX = hmax;
	hash = h;
	H = new LinkedList<struct elem_info<Tkey, Tvalue> > [HMAX];
}
template<typename Tkey, typename Tvalue>
Hashtable<Tkey, Tvalue>::~Hashtable() {
	for (int i = 0; i < HMAX; i++) {
		while (!H[i].isEmpty()) {
			H[i].removeFirst();
		}
	}
	delete[] H;
}
// Add an element to the hashmap
template<typename Tkey, typename Tvalue>
void Hashtable<Tkey, Tvalue>::put(Tkey key, Tvalue value) {
	struct list_elem<struct elem_info<Tkey, Tvalue> > *p;
	struct elem_info<Tkey, Tvalue> info;

	int hkey = hash(key);
	p = H[hkey].front();
	while (p != NULL) {
		if (p->info.key == key) {
			break;
		}
		p = p->next;
	}

	if (p != NULL) {
		p->info.value = value;
	} else {
		info.key = key;
		info.value = value;
		H[hkey].addLast(info);
	}
}
// Returns the value of the key in the hashmap
template<typename Tkey, typename Tvalue>
Tvalue Hashtable<Tkey, Tvalue>::get(Tkey key) {
	struct list_elem<struct elem_info<Tkey, Tvalue> > *p;

	int hkey = hash(key);
	p = H[hkey].front();

	while (p != NULL) {
		if (p->info.key == key) {
			break;
		}
		p = p->next;
	}

	if (p != NULL) {
		return p->info.value;
	} else {
		Tvalue x = 0;
		return x;
	}
}
// Check if a specific key exists in the hashmap
template<typename Tkey, typename Tvalue>
bool Hashtable<Tkey, Tvalue>::hasKey(Tkey key) {
	struct list_elem<struct elem_info<Tkey, Tvalue> > *p;

	int hkey = hash(key);
	p = H[hkey].front();

	while (p != NULL) {
		if (p->info.key == key) {
			break;
		}
		p = p->next;
	}

	if (p != NULL) {
		return true;
	} else {
		return false;
	}
}
