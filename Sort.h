// Copyright 2019 Topala Andrei
#ifndef SORT_H_
#define SORT_H_
#include <string>
#include <vector>

// Driver status
struct status {
	std::string name;
	int crossID, id;
	int points, usage, nrClients;
	float rating;
	bool work;

	// Default constructor
	status() {
		usage = nrClients = points = 0;
		rating = 0;
		work = true;
	}
	// Constructor
	status(std::string name, int id, int crossID) {
		this->name = name;
		this->id = id;
		this->crossID = crossID;
		usage = nrClients = points = 0;
		rating = 0;
		work = true;
	}
};
// Marked destinations features
struct visits{
	std::string name;
	int gasFree;
	visits() {}
	visits(std::string name, int gasFree) {
		this->name = name;
		this->gasFree = gasFree;
	}
};

inline bool cmpDist(status A, status B);
inline bool cmpRate(status A, status B);
inline bool cmpRides(status A, status B);
inline bool cmpCross(visits A, visits B);
template <typename Tvalue>
void quicksort(std::vector<Tvalue> &array, int first, int last,
				bool (*cmp)(Tvalue, Tvalue));

#endif  // SORT_H_
