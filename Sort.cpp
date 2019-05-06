// Copyright 2019 Topala Andrei
#include "./Sort.h"
#include <vector>

// Distance comparison function
bool cmpDist(status A, status B) {
	if (A.usage == B.usage) {
		return A.name < B.name;
	}
	return A.usage > B.usage;
}
// Rating comparison function
inline bool cmpRate(status A, status B) {
	if (A.rating == B.rating) {
		return A.name < B.name;
	}
	return A.rating > B.rating;
}
// Ride number comparison function
inline bool cmpRides(status A, status B) {
	if (A.nrClients == B.nrClients) {
		return A.name < B.name;
	}
	return A.nrClients > B.nrClients;
}
// Intersection names comparison
inline bool cmpCross(visits A, visits B) {
	if (A.gasFree == B.gasFree) {
		return A.name < B.name;
	}
	return A.gasFree > B.gasFree;
}
// Quicksort with median pivot
// - has the worst-case O(n*logn) -
template <typename Tvalue>
void quicksort(std::vector<Tvalue> &array, int first, int last,
				bool (*cmp)(Tvalue, Tvalue)) {
	int k, mini, maxi;
	if (first > last) {
		return;
	}
	Tvalue med, aux;
	k = (first + last) / 2;
	med = array[k];
	mini = first;
	maxi = last;
	while (mini <= maxi) {
		while (cmp(array[mini], med)) {
			mini++;
		}
		while (cmp(med, array[maxi])) {
			maxi--;
		}
		if (mini <= maxi) {
			aux = array[mini];
			array[mini] = array[maxi];
			array[maxi] = aux;

			mini++;
			maxi--;
		}
	}
	if (first < maxi) {
		quicksort(array, first, maxi, cmp);
	}
	if (last > mini) {
		quicksort(array, mini, last, cmp);
	}
}
