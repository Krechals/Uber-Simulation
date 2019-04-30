// Copyright 2019 Topala Andrei

#ifndef SOLVER_H_
#define SOLVER_H_
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
#include "Hashmap.h"

#define INF (1<<30)
#define MARK -(1<<30)
#define VMAX 1009

struct status {
	std::string name;
	int idCross, id, usage, nrClients;
	int points = 0;
	float rating;
	bool work;
};
struct visits{
	std::string name;
	int gasFree = 0;
};
bool cmpDist(status A, status B) {
	if (A.usage == B.usage) {
		return A.name < B.name;
	}
	return A.usage > B.usage;
}
bool cmpRate(status A, status B) {
	if (A.rating == B.rating) {
		return A.name < B.name;
	}
	return A.rating > B.rating;
}
bool cmpRides(status A, status B) {
	if (A.nrClients == B.nrClients) {
		return A.name < B.name;
	}
	return A.nrClients > B.nrClients;
}
bool cmpCross(visits A, visits B) {
	if (A.gasFree == B.gasFree) {
		return A.name < B.name;
	}
	return A.gasFree > B.gasFree;
}
int customHash(std::string key) {
    int hkey = 0;
    for (unsigned int i = 0; i < key.length(); i++)
        hkey = (hkey * 31 + key[i]) % VMAX;
    return hkey;
}
void quicksort1(std::vector<status> &array, int first, int last) {
	int k, mini, maxi;
	if (first > last) {
		return;
	}
	status med, aux;
	k = (first + last) / 2;
	med = array[k];
	mini = first;
	maxi = last;
	while (mini <= maxi) {
		while (cmpDist(array[mini], med)) {
			mini++;
		}
		while (cmpDist(med, array[maxi])) {
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
		quicksort1(array, first, maxi);
	}
	if (last > mini) {
		quicksort1(array, mini, last);
	}
}
void quicksort2(std::vector<status> &array, int first, int last) {
	int k, mini, maxi;
	if (first > last) {
		return;
	}
	status med, aux;
	k = (first + last) / 2;
	med = array[k];
	mini = first;
	maxi = last;
	while (mini <= maxi) {
		while (cmpRate(array[mini], med)) {
			mini++;
		}
		while (cmpRate(med, array[maxi])) {
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
		quicksort2(array, first, maxi);
	}
	if (last > mini) {
		quicksort2(array, mini, last);
	}
}
void quicksort3(std::vector<status> &array, int first, int last) {
	int k, mini, maxi;
	if (first > last) {
		return;
	}
	status med, aux;
	k = (first + last) / 2;
	med = array[k];
	mini = first;
	maxi = last;
	while (mini <= maxi) {
		while (cmpRides(array[mini], med)) {
			mini++;
		}
		while (cmpRides(med, array[maxi])) {
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
		quicksort3(array, first, maxi);
	}
	if (last > mini) {
		quicksort3(array, mini, last);
	}
}
void quicksort4(std::vector<visits> &array, int first, int last) {
	int k, mini, maxi;
	if (first > last) {
		return;
	}
	visits med, aux;
	k = (first + last) / 2;
	med = array[k];
	mini = first;
	maxi = last;
	while (mini <= maxi) {
		while (cmpCross(array[mini], med)) {
			mini++;
		}
		while (cmpCross(med, array[maxi])) {
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
		quicksort4(array, first, maxi);
	}
	if (last > mini) {
		quicksort4(array, mini, last);
	}
}

class solver {
 public:
	int N, M, nrDrivers = 0;
	int adjacency[600][600] = {{0}};
	int adjDistances[600][600] = {{0}};
	int adjDistances2[600][600] = {{0}};
	int newDestination[600][600] = {{0}};

	Hashtable<std::string, int> cross =
						Hashtable<std::string, int>(VMAX, customHash);
	Hashtable<std::string, int> driverMap =
						Hashtable<std::string, int>(VMAX, customHash);
	// std::map<std::string, int> cross;
	// std::map<std::string, int> driverMap;
	std::vector<int> network[1000];
	std::vector<std::string> intersects;
	std::vector<status> drivers;
	int position[600];

	bool isPath(int id1, int id2) {
		bool visit[1000] = {0};
		int current, next, size;
		std::stack<int> S;
		S.push(id1);
		visit[id1] = true;
		while (!S.empty()) {
			current = S.top();
			S.pop();
			if (current == id2) {
				return true;
			}
			size = network[current].size();
			for (int i = 0; i < size; ++i) {
				next = network[current][i];
				if (next > 0 && !visit[next]) {
					S.push(next);
					visit[next] = true;
				}
			}
		}
		return false;
	}
	int distance(int id1, int id2) {
		bool visit[1000] = {0};
		int distance[1000] = {0};
		int current, next, size;
		std::queue<int> Q;
		Q.push(id1);
		visit[id1] = true;
		while (!Q.empty()) {
			current = Q.front();
			Q.pop();
			if (current == id2) {
				return distance[id2];
			}
			size = network[current].size();
			for (int i = 0; i < size; ++i) {
				next = network[current][i];
				if (next > 0 && !visit[next]) {
					Q.push(next);
					distance[next] = distance[current] + 1;
					visit[next] = true;
				}
			}
		}
		return -1;
	}
	void preprocess(int id) {
		bool visit[1000] = {0};
		int current, next, size;
		std::queue<int> Q;
		Q.push(id);
		visit[id] = true;
		while (!Q.empty()) {
			current = Q.front();
			Q.pop();
			size = network[current].size();
			for (int i = 0; i < size; ++i) {
				next = network[current][i];
				if (next > 0 && !visit[next]) {
					Q.push(next);
					adjDistances[id][next] = adjDistances[id][current] + 1;
					visit[next] = true;
				}
			}
		}
		for (int i = 1; i <= N; ++i) {
			if (i != id && adjDistances[id][i] == 0) {
				adjDistances[id][i] = INF;
			}
		}
	}
	std::vector<visits> visitNodes(int startID, int distance) {
		int gass[1000] = {0};
		bool visit[1000] = {0};
		int current, next, size;
		visits tmp;
		std::vector<visits> ans;
		std::queue<int> Q;
		Q.push(startID);
		visit[startID] = true;
		gass[startID] = distance;
		if (cross.get(intersects[startID - 1]) == MARK) {
			tmp.name = intersects[startID - 1];
			tmp.gasFree = distance;
			ans.push_back(tmp);
		}
		while (!Q.empty()) {
			current = Q.front();
			Q.pop();
			size = network[current].size();
			for (int i = 0; i < size; ++i) {
				next = network[current][i];
				if (next > 0 && gass[current] > 0 && !visit[next]) {
					Q.push(next);
					gass[next] = gass[current] - 1;
					visit[next] = true;
					if (cross.get(intersects[next - 1]) == MARK) {
					 	tmp.name = intersects[next - 1];
					 	tmp.gasFree = gass[next];
					 	ans.push_back(tmp);
					}
				}
			}
		}
		return ans;
	}
	void task1_solver(std::ifstream& fin, std::ofstream& fout) {
		std::string crossRoad1, crossRoad2;
		int id1, id2, Q1;
		fin >> N >> M;
		for (int i = 1; i <= N; ++i) {
			fin >> crossRoad1;
			intersects.push_back(crossRoad1);
			cross.put(crossRoad1, i);
		}
		for (int i = 1; i <= M; ++i) {
			fin >> crossRoad1 >> crossRoad2;
			id1 = cross.get(crossRoad1);
			id2 = cross.get(crossRoad2);
			adjacency[id1][id2] = 1;
			network[id1].push_back(id2);
		}
		fin >> Q1;
		for (int i = 1; i <= Q1; ++i) {
			fin >> crossRoad1 >> crossRoad2;
			id1 = cross.get(crossRoad1);
			id2 = cross.get(crossRoad2);
			if (isPath(id1, id2)) {
				fout << 'y' << '\n';
			} else {
				fout << 'n' << '\n';
			}
		}
	}

	void task2_solver(std::ifstream& fin, std::ofstream& fout) {
		std::string crossRoad1, crossRoad2;
		int id1, id2, Q2;
		fin >> Q2;
		for (int i = 1; i <= Q2; ++i) {
			fin >> crossRoad1 >> crossRoad2;
			id1 = cross.get(crossRoad1);
			id2 = cross.get(crossRoad2);
			fout << distance(id1, id2) << '\n';
		}
	}

	void task3_solver(std::ifstream& fin, std::ofstream& fout) {
		std::string crossRoad1, crossRoad2, crossRoad3;
		int id1, id2, Q3;
		char op;
		int op2, size;
		fin >> Q3;
		for (int i = 1; i <= Q3; ++i) {
			fin >> op;
			if (op == 'c') {
				fin >> crossRoad1 >> crossRoad2 >> op2;
				id1 = cross.get(crossRoad1);
				id2 = cross.get(crossRoad2);

				switch (op2) {
					case 0:
						adjacency[id1][id2] = 1;
						network[id1].push_back(id2);
						break;
					case 1:
						adjacency[id1][id2] = 0;
						adjacency[id2][id1] = 0;
						size = network[id1].size();
						for (int i = 0; i < size; ++i) {
							if (network[id1][i] == id2) {
								network[id1][i] = -1;
								break;
							}
						}
						size = network[id2].size();
						for (int i = 0; i < size; ++i) {
							if (network[id2][i] == id1) {
								network[id2][i] = -1;
								break;
							}
						}
						break;
					case 2:
						if (!adjacency[id1][id2]) {
							adjacency[id1][id2] = 1;
							network[id1].push_back(id2);
						}
						if (!adjacency[id2][id1]) {
							adjacency[id2][id1] = 1;
							network[id2].push_back(id1);
						}
						break;
					case 3:
						if (adjacency[id1][id2] && !adjacency[id2][id1]) {
							adjacency[id1][id2] = 0;
							adjacency[id2][id1] = 1;
							size = network[id1].size();
							for (int i = 0; i < size; ++i) {
								if (network[id1][i] == id2) {
									network[id1][i] = -1;
									break;
								}
							}
							network[id2].push_back(id1);
						} else if (!adjacency[id1][id2] && adjacency[id2][id1]) {
							adjacency[id1][id2] = 1;
							adjacency[id2][id1] = 0;
							int size = network[id2].size();
							for (int i = 0; i < size; ++i) {
								if (network[id2][i] == id1) {
									network[id2][i] = -1;
									break;
								}
							}
							network[id1].push_back(id2);
						}
						break;
				}
			} else {
				fin >> crossRoad1 >> crossRoad2 >> op2;
				id1 = cross.get(crossRoad1);
				id2 = cross.get(crossRoad2);
				switch (op2) {
					case 0:
						if (isPath(id1, id2)) {
							fout << 'y' << '\n';
						} else {
							fout << 'n' << '\n';
						}
						break;
					case 1:
						fout << distance(id1, id2) << '\n';
						break;
					case 2:
						fin >> crossRoad3;
						int id3 = cross.get(crossRoad3);
						int dist1 = distance(id1, id3), dist2 = distance(id3, id2);
						if (dist1 != -1 && dist2 != -1) {
							fout << dist1 + dist2 << '\n';
						} else {
							fout << -1 << '\n';
						}
						break;
				}
			}
		}
		for (int i = 1; i <= N; ++i) {
			preprocess(i);
		}
		for (int i = 1; i <= N; ++i) {
			for (int j = 1; j <= N; ++j) {
				newDestination[i][j] = -1;
				for (unsigned int k = 0; k < network[j].size(); ++k) {
					if (adjDistances[i][j] == INF &&
						adjDistances[i][network[j][k]] < INF) {
						adjDistances2[i][j] = adjDistances[i][network[j][k]];
						newDestination[i][j] = network[j][k];
					}
				}
			}
		}
	}

	void task4_solver(std::ifstream& fin, std::ofstream& fout) {
		std::string crossRoad1, crossRoad2, name, operation;
		std::string nameStart, nameEnd;
		int Q4, driverID, mini, rateScore, crossID;
		int minDriver, crossStartID, crossEndID;
		unsigned int nr;
		status tmp;
		std::map<std::string, int>::iterator it;
		std::vector<std::string> canVisit;

		fin >> Q4;
		for (int i = 1; i <= Q4; ++i) {
			fin >> operation;
			if (operation[0] == 'd') {
				fin >> name >> crossRoad1;
				crossID = cross.get(crossRoad1);

				// it = driverMap.find(name);
				if (driverMap.hasKey(name) == 1) {
					driverID = driverMap.get(name);
					drivers[position[driverID]].work = true;
					drivers[position[driverID]].idCross = crossID;
				} else {
					tmp.name = name;
					tmp.rating = 0;
					tmp.id = nrDrivers;
					position[nrDrivers] = nrDrivers;
					tmp.nrClients = 0;
					tmp.work = true;
					tmp.idCross = crossID;
					tmp.usage = 0;
					driverMap.put(name, nrDrivers);
					drivers.push_back(tmp);
					++nrDrivers;
				}
			} else if (operation[0] == 'b') {
				fin >> name;
				driverID = driverMap.get(name);
				drivers[position[driverID]].work = false;
				// BFS pe un graf transpus la preprocesare
			} else if (operation[0] == 'r') {
				fin >> nameStart >> nameEnd >> rateScore;
				crossStartID = cross.get(nameStart);
				crossEndID = cross.get(nameEnd);
				mini = INF;
				minDriver = 0;
				for (int j = 0; j < nrDrivers; ++j) {
					if (drivers[j].work == false) {
						continue;
					}
					if (mini == adjDistances[drivers[j].idCross][crossStartID]) {
						if (drivers[j].rating > drivers[minDriver].rating) {
							minDriver = j;
						} else if (drivers[j].rating == drivers[minDriver].rating
							&& drivers[j].name < drivers[minDriver].name) {
							minDriver = j;
						}
					} else if (mini > adjDistances[drivers[j].idCross][crossStartID]) {
						mini = adjDistances[drivers[j].idCross][crossStartID];
						minDriver = j;
					}
				}
				if (mini == INF) {
					fout << "Soferi indisponibili" << '\n';
					continue;
				}
				if (adjDistances[crossStartID][crossEndID] == INF
					&& newDestination[crossStartID][crossEndID] == -1) {
					fout << "Destinatie inaccesibila" << '\n';
					continue;
				}
				drivers[minDriver].points += rateScore;
				drivers[minDriver].nrClients++;
				drivers[minDriver].rating = 1.0 * drivers[minDriver].points
											/ drivers[minDriver].nrClients;
				drivers[minDriver].idCross = crossEndID;
				if (adjDistances[crossStartID][crossEndID] == INF) {
					drivers[minDriver].usage +=
							mini + adjDistances2[crossStartID][crossEndID];
					drivers[minDriver].idCross =
							newDestination[crossStartID][crossEndID];
				} else {
					drivers[minDriver].usage +=
							mini + adjDistances[crossStartID][crossEndID];
				}
				// top_dist
			} else if (operation[0] == 't' && operation[6] == 's') {
				fin >> nr;
				// std::sort(drivers.begin(), drivers.end(), cmpDist);
				quicksort1(drivers, 0, nrDrivers - 1);
				nr = std::min(nr, (unsigned int)nrDrivers);
				for (unsigned int j = 0; j < nr; ++j) {
					position[drivers[j].id] = j;
					fout <<  drivers[j].name << ':' << drivers[j].usage << ' ';
				}
				for (unsigned int j = nr; j < drivers.size(); ++j) {
					position[drivers[j].id] = j;
				}
				fout << '\n';
				// top_rating
			} else if (operation[0] == 't' && operation[6] == 't') {
				fin >> nr;
				// std::sort(drivers.begin(), drivers.end(), cmpRate);
				quicksort2(drivers, 0, nrDrivers - 1);
				nr = std::min(nr, (unsigned int)nrDrivers);
				for (unsigned int j = 0; j < nr; ++j) {
					position[drivers[j].id] = j;
					fout << std::fixed;
					fout << std::setprecision(3) << drivers[j].name << ':';
					fout << drivers[j].rating << ' ';
				}
				for (unsigned int j = nr; j < drivers.size(); ++j) {
					position[drivers[j].id] = j;
				}
				fout << '\n';
			} else if (operation[0] == 'i') {
				fin >> name;
				driverID = driverMap.get(name);
				fout << name << ": ";
				fout << intersects[drivers[position[driverID]].idCross - 1];
				fout << ' ' << drivers[position[driverID]].rating << ' ';
				fout << drivers[position[driverID]].nrClients << ' ';
				fout << drivers[position[driverID]].usage << ' ';
				if (drivers[position[driverID]].work == true) {
					fout << "online";
				} else {
					fout << "offline";
				}
				fout << '\n';
				// top_riders
			} else if (operation[0] == 't' && operation[6] == 'd') {
				fin >> nr;
				// std::sort(drivers.begin(), drivers.end(), cmpRides);
				quicksort3(drivers, 0, nrDrivers - 1);
				nr = std::min(nr, (unsigned int)nrDrivers);
				for (unsigned int j = 0; j < nr; ++j) {
					position[drivers[j].id] = j;
					fout <<  drivers[j].name << ':';
					fout << drivers[j].nrClients << ' ';
				}
				for (unsigned int j = nr; j < drivers.size(); ++j) {
					position[drivers[j].id] = j;
				}
				fout << '\n';
			}
		}
	}

	void task5_solver(std::ifstream& fin, std::ofstream& fout) {
		int gas, nrCross, driverID;
		std::string name, crossRoad;
		std::vector<visits> canVisit;
		fin >> gas >> name;
		fin >> nrCross;
		driverID = driverMap.get(name);
		for (int i = 1; i <= nrCross; ++i) {
			fin >> crossRoad;
			cross.put(crossRoad, MARK);
		}
		canVisit = visitNodes(drivers[position[driverID]].idCross, gas);
		quicksort4(canVisit, 0, canVisit.size() - 1);
		for (unsigned int i = 0; i < canVisit.size(); ++i) {
			fout << canVisit[i].name << ' ';
		}
	}
};
#endif  // SOLVER_H_
