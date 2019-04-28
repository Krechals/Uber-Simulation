// Copyright 2019 SD_Homework_Team

#ifndef SOLVER_H_
#define SOLVER_H_
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <queue>
//#include "Skiplist.h"
#define INF (1<<30)

struct status {
	std::string name;
	int idCross, position, usage, nrClients;
	int points;
	float rating;
	bool work;
};
class solver {
public:
	int N, M, nrDrivers = 0;
	int adjacency[600][600] = {0};
	int adjDistances[600][600] = {0};
	std::map<std::string, int> cross;
	std::map<std::string, int> driverMap;
	std::vector<int> network[1000];
	int log_2(int N) {
		int ans = 0;
		while ((1 << ans) <= N) {
			++ans;
		}
		return ans;
	}
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
	void task1_solver(std::ifstream& fin, std::ofstream& fout) {
		std::string crossRoad1, crossRoad2;
		int id1, id2, Q1;
		fin >> N >> M;
		for (int i = 1; i <= N; ++i) {
			fin >> crossRoad1;
			cross[crossRoad1] = i;
		}
		for (int i = 1; i <= M; ++i) {
			fin >> crossRoad1 >> crossRoad2;
			id1 = cross[crossRoad1];
			id2 = cross[crossRoad2];
			adjacency[id1][id2] = 1;
			network[id1].push_back(id2);
		}
		fin >> Q1;
		for (int i = 1; i <= Q1; ++i) {
			fin >> crossRoad1 >> crossRoad2;
			id1 = cross[crossRoad1];
			id2 = cross[crossRoad2];
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
			id1 = cross[crossRoad1];
			id2 = cross[crossRoad2];
			fout << distance(id1, id2) << '\n';
		}
	}

	void task3_solver(std::ifstream& fin, std::ofstream& fout) {
		std::string crossRoad1, crossRoad2, crossRoad3;
		int id1, id2, Q3;
		char op;
		int op2, size, ans;
		fin >> Q3;
		for (int i = 1; i <= Q3; ++i) {
			fin >> op;
			if (op == 'c') {
				fin >> crossRoad1 >> crossRoad2 >> op2;
				id1 = cross[crossRoad1];
				id2 = cross[crossRoad2];

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
				id1 = cross[crossRoad1];
				id2 = cross[crossRoad2];
				switch (op2) {
					case 0:
						if (isPath(id1, id2)) {
							fout << 'y' << '\n';
						}
						else {
							fout << 'n' << '\n';
						}
						break;
					case 1:
						fout << distance(id1, id2) << '\n';
						break;
					case 2:
						fin >> crossRoad3;
						int id3 = cross[crossRoad3];
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
	}

	void task4_solver(std::ifstream& fin, std::ofstream& fout) {
		std::string crossRoad1, crossRoad2, name, operation;
		std::string nameStart, nameEnd;
		int Q4, driverID, mini, rateScore, crossID;
		int minDriver, crossStartID, crossEndID;
		status tmp;
		std::map<std::string, int>::iterator it;
		std::vector<status> drivers;
		//Skiplist<int> rating(log_2(500));
		//Skiplist<int> dist(log_2(500));
		//Skiplist<int> rides(log_2(500));
		fin >> Q4;
		for (int i = 1; i <= Q4; ++i) {
			fin >> operation;
			if (operation[0] == 'd') {
				fin >> name >> crossRoad1;
				crossID = cross[crossRoad1];

				it = driverMap.find(name);
				if (it != driverMap.end()) {
					driverID = driverMap[name];
					drivers[driverID].work = true;
					drivers[driverID].idCross = crossID;
				} else {
					tmp.name = name;
					tmp.rating = 0;
					tmp.nrClients = 0;
					tmp.work = true;
					tmp.idCross = crossID;
					tmp.usage = 0;
					driverMap[name] = nrDrivers;
					drivers.push_back(tmp);
					++nrDrivers;
				}
			} else if (operation[0] == 'b') {
				fin >> name;
				driverID = driverMap[name];
				drivers[driverID].work = false;
			} else if (operation[0] == 'r') { // BFS pe un graf transpus la preprocesare
				fin >> nameStart >> nameEnd >> rateScore;
				crossStartID = cross[nameStart];
				crossEndID = cross[nameEnd];
				mini = INF;
				minDriver = 0;
				for (int j = 0; j < nrDrivers; ++j) {
					if (mini == adjDistances[drivers[j].idCross][crossStartID]) {
						if (drivers[j].rating > drivers[minDriver].rating) {
							minDriver = j;
						} else if (drivers[j].rating == drivers[minDriver].rating && drivers[j].name.compare(drivers[minDriver].name) < 0) {
							minDriver = j;
						}
					} else if (mini > adjDistances[drivers[j].idCross][crossStartID]) {
						mini = adjDistances[drivers[j].idCross][crossStartID];
						minDriver = j;
					}
				}
				drivers[minDriver].points += rateScore;
				drivers[minDriver].nrClients++;
				drivers[minDriver].rating = (float)(drivers[minDriver].points / drivers[minDriver].nrClients);
				drivers[minDriver].usage += mini + adjDistances[crossStartID][crossEndID];
				drivers[minDriver].idCross = crossEndID;
				// for (int i = 0; i < drivers.size(); ++i) {
				// 	std::cout << drivers[i].name << ' ' << drivers[i].usage << std::endl;
				// }
				// std::cout << std::endl;
			}
		}
	}

	void task5_solver(std::ifstream& fin, std::ofstream& fout) { }
};
#endif  // SOLVER_H_
