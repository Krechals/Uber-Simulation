// Copyright 2019 Topala Andrei
#include "./solver.h"
#include <fstream>
#include <iomanip>
#include <stack>
#include <string>
#include <vector>
#include <queue>


// Returns minimum of 2 integers
int solver::minimum(int A, int B) {
	if (A <= B) {
		return A;
	}
	return B;
}
// Deletes an edge from one node to another
void solver::deleteEdge(int node1, int node2) {
	unsigned int size = network[node1].size();
	for (unsigned int i = 0; i < size; ++i) {
		if (network[node1][i] == node2) {
			network[node1].erase(network[node1].begin() + i);
			break;
		}
	}
}
// Custom hash function
int solver::customHash(std::string key) {
    int hkey = 0;
    for (unsigned int i = 0; i < key.length(); i++)
        hkey = (hkey * 67 + key[i]) % VMAX;
    return hkey;
}
/**
 * Return the closest driver to the starting
 * node and it's distance
 *
 * @par crossStartID Index of the starting node
 * @par mini Minimum distance to the starting node
 */
int solver::findDriver(int crossStartID, int &mini) {
	int minDriver = 0;
	mini = INF;
	// Get the most appropiate driver
	for (int j = 0; j < nrDrivers; ++j) {
		// Check if the driver is offline
		if (drivers[j].work == false) {
			continue;
		}
		// Find the best driver according to the task
		if (mini == computedDist[drivers[j].crossID][crossStartID]) {
			if (drivers[j].rating > drivers[minDriver].rating) {
				minDriver = j;
			} else if (drivers[j].rating == drivers[minDriver].rating
				&& drivers[j].name < drivers[minDriver].name) {
				minDriver = j;
			}
		} else if (mini > computedDist[drivers[j].crossID][crossStartID]) {
			mini = computedDist[drivers[j].crossID][crossStartID];
			minDriver = j;
		}
	}
	return minDriver;
}
/**
 * Change status of the driver which had the ride
 *
 * @par minDriver Index of driver which is updated
 * @par rateScore Rating of the last ride
 * @par crossStartID Index of start of the ride
 * @par crossEndID Index of new current location
 * @par distance Distance from last location to start of the ride
 */
void solver::driverUpdate(int minDriver, int rateScore, int crossStartID,
							int crossEndID, int distance) {
	drivers[minDriver].points += rateScore;
	drivers[minDriver].nrClients++;
	drivers[minDriver].rating = 1.0 * drivers[minDriver].points
								/ drivers[minDriver].nrClients;
	drivers[minDriver].crossID = crossEndID;
	// There are accessible neighbor nodes
	if (computedDist[crossStartID][crossEndID] == INF) {
		drivers[minDriver].usage +=
				distance + neighborDist[crossStartID][crossEndID];
		drivers[minDriver].crossID =
				newDestination[crossStartID][crossEndID];
	} else {
		drivers[minDriver].usage +=
				distance + computedDist[crossStartID][crossEndID];
	}
}
// Prints status of a specific driver
void solver::printDriverStatus(std::string name, int driverID,
								std::ofstream& fout) {
	fout << name << ": ";
	fout << intersects[drivers[position[driverID]].crossID - 1];
	fout << ' ' << drivers[position[driverID]].rating << ' ';
	fout << drivers[position[driverID]].nrClients << ' ';
	fout << drivers[position[driverID]].usage << ' ';
	if (drivers[position[driverID]].work == true) {
		fout << "online";
	} else {
		fout << "offline";
	}
	fout << '\n';
}
/**
 * Verifies if 2 graph nodes are connected
 * using DFS algorithm
 *
 * @par id1 Index of the starting node
 * @par id2 Index of the ending node
 */
bool solver::isPath(int id1, int id2) {
		bool visit[MAXN] = {0};
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
				if (!visit[next]) {
					S.push(next);
					visit[next] = true;
				}
			}
		}
		return false;
	}
/**
 * Returns distance of 2 graph nodes
 * using BFS algorithm
 *
 * @par id1 Index of the starting node
 * @par id2 Index of the ending node
 */
int solver::distance(int id1, int id2) {
	bool visit[MAXN] = {0};
	int distance[MAXN] = {0};
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
			if (!visit[next]) {
				Q.push(next);
				distance[next] = distance[current] + 1;
				visit[next] = true;
			}
		}
	}
	// There is NO path between the 2 nodes
	return -1;
}
/**
 * Computes  distances between specific node and
 * the all rest nodes using BFS algorithm
 *
 * @par id Index of the node in which the computation is applied
 */
void solver::preprocess(int id) {
	bool visit[MAXN] = {0};
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
			if (!visit[next]) {
				Q.push(next);
				computedDist[id][next] = computedDist[id][current] + 1;
				visit[next] = true;
			}
		}
	}
	// If a node is inaccessible, we mark it with infinit distance
	for (int i = 1; i <= N; ++i) {
		if (i != id && computedDist[id][i] == 0) {
			computedDist[id][i] = INF;
		}
	}
}
/**
 * Returns all MARKED nodes of a graph that are close to
 * startNode by a specific distance using BFS algorithm
 *
 * @par startID Index of the starting node
 * @par distance 
 * @var gass[i] Ramaining distance of a node till maximum distance is achived
 */
std::vector<visits> solver::visitNodes(int startID, int distance) {
	int gass[MAXN] = {0};
	bool visit[MAXN] = {0};
	int current, next, size;
	visits tmp;
	std::vector<visits> ans;
	std::queue<int> Q;
	Q.push(startID);
	visit[startID] = true;
	gass[startID] = distance;

	// Verify if the startNode is Marked
	if (crossMap.get(intersects[startID - 1]) == MARK) {
		tmp = visits(intersects[startID - 1], distance);
		ans.push_back(tmp);
	}
	while (!Q.empty()) {
		current = Q.front();
		Q.pop();
		size = network[current].size();
		for (int i = 0; i < size; ++i) {
			next = network[current][i];
			if (gass[current] > 0 && !visit[next]) {
				Q.push(next);
				gass[next] = gass[current] - 1;
				visit[next] = true;
				// Add the node, if it's MARKED
				if (crossMap.get(intersects[next - 1]) == MARK) {
				 	tmp = visits(intersects[next - 1], gass[next]);
				 	ans.push_back(tmp);
				}
			}
		}
	}
	return ans;
}

void solver::task1_solver(std::ifstream& fin, std::ofstream& fout) {
	std::string crossRoad1, crossRoad2;
	int id1, id2, Q1;
	fin >> N >> M;
	for (int i = 1; i <= N; ++i) {
		fin >> crossRoad1;
		intersects.push_back(crossRoad1);
		// Add the intersection to hashmap
		crossMap.put(crossRoad1, i);
	}
	for (int i = 1; i <= M; ++i) {
		fin >> crossRoad1 >> crossRoad2;
		// Get IDs of intersection names
		id1 = crossMap.get(crossRoad1);
		id2 = crossMap.get(crossRoad2);
		// Make links between nodes
		adjacency[id1][id2] = 1;
		network[id1].push_back(id2);
	}
	// Answer to queries
	fin >> Q1;
	for (int i = 1; i <= Q1; ++i) {
		fin >> crossRoad1 >> crossRoad2;
		id1 = crossMap.get(crossRoad1);
		id2 = crossMap.get(crossRoad2);
		if (isPath(id1, id2)) {
			fout << 'y' << '\n';
		} else {
			fout << 'n' << '\n';
		}
	}
}
void solver::task2_solver(std::ifstream& fin, std::ofstream& fout) {
	std::string crossRoad1, crossRoad2;
	int id1, id2, Q2;
	// Answer to queries
	fin >> Q2;
	for (int i = 1; i <= Q2; ++i) {
		fin >> crossRoad1 >> crossRoad2;
		// Get IDs of intersection names
		id1 = crossMap.get(crossRoad1);
		id2 = crossMap.get(crossRoad2);
		fout << distance(id1, id2) << '\n';
	}
}
void solver::task3_solver(std::ifstream& fin, std::ofstream& fout) {
	std::string crossRoad1, crossRoad2, crossRoad3;
	int id1, id2, Q3, op2;
	char op;
	// Answer to queries
	fin >> Q3;
	for (int i = 1; i <= Q3; ++i) {
		fin >> op;
		if (op == 'c') {
			fin >> crossRoad1 >> crossRoad2 >> op2;
			// Get IDs of intersection names
			id1 = crossMap.get(crossRoad1);
			id2 = crossMap.get(crossRoad2);

			switch (op2) {
				case 0:
					// Create link between 2 specific nodes
					adjacency[id1][id2] = 1;
					network[id1].push_back(id2);
					break;
				case 1:
					// Delete all links between 2 specific nodes
					adjacency[id1][id2] = 0;
					adjacency[id2][id1] = 0;
					deleteEdge(id1, id2);
					deleteEdge(id2, id1);
					break;
				case 2:
					// Create bidirectional edge between 2 nodes
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
					// Swap the link between 2 intersection
					// Delete first to second link and create second to first link
					if (adjacency[id1][id2] && !adjacency[id2][id1]) {
						adjacency[id1][id2] = 0;
						adjacency[id2][id1] = 1;
						deleteEdge(id1, id2);
						network[id2].push_back(id1);
					// Delete second to first link and create first to second link
					} else if (!adjacency[id1][id2] && adjacency[id2][id1]) {
						adjacency[id1][id2] = 1;
						adjacency[id2][id1] = 0;
						deleteEdge(id2, id1);
						network[id1].push_back(id2);
					}
					break;
			}
		} else {
			// Answer to queries
			fin >> crossRoad1 >> crossRoad2 >> op2;
			id1 = crossMap.get(crossRoad1);
			id2 = crossMap.get(crossRoad2);
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
					// Calculate distance from 2 nodes, passing through another
					fin >> crossRoad3;
					int id3 = crossMap.get(crossRoad3);
					int dist1 = distance(id1, id3), dist2 = distance(id3, id2);
					if (dist1 != -1 && dist2 != -1) {
						fout << dist1 + dist2 << '\n';
					} else {
						// There is NO such path
						fout << -1 << '\n';
					}
					break;
			}
		}
	}
	// Computation of all distances in graph
	for (int i = 1; i <= N; ++i) {
		preprocess(i);
	}
	// Set neighbor destination if we can't get to a specific node
	for (int i = 1; i <= N; ++i) {
		for (int j = 1; j <= N; ++j) {
			newDestination[i][j] = -1;
			for (unsigned int k = 0; k < network[j].size(); ++k) {
				if (computedDist[i][j] == INF &&
					computedDist[i][network[j][k]] < INF) {
					// Set the distances of the specific node
					neighborDist[i][j] = computedDist[i][network[j][k]];
					// Set another destination
					newDestination[i][j] = network[j][k];
				}
			}
		}
	}
}
void solver::task4_solver(std::ifstream& fin, std::ofstream& fout) {
	std::string crossRoad1, crossRoad2, name, operation;
	std::string nameStart, nameEnd;
	int Q4, driverID, mini, rateScore, crossID;
	int minDriver, crossStartID, crossEndID;
	unsigned int nr;

	// Answer to queries
	fin >> Q4;
	for (int i = 1; i <= Q4; ++i) {
		fin >> operation;
		if (operation == "d") {
			fin >> name >> crossRoad1;
			// Get ID of the intersection
			crossID = crossMap.get(crossRoad1);
			// Driver has it's first appearance
			if (driverMap.hasKey(name) == 1) {
				// Change status of the driver
				driverID = driverMap.get(name);
				drivers[position[driverID]].work = true;
				drivers[position[driverID]].crossID = crossID;
			} else {
				// Add driver to the hashmap
				status tmp = status(name, nrDrivers, crossID);
				position[nrDrivers] = nrDrivers;
				driverMap.put(name, nrDrivers);
				drivers.push_back(tmp);
				++nrDrivers;
			}
		} else if (operation == "b") {
			// Set specific driver offline
			fin >> name;
			driverID = driverMap.get(name);
			drivers[position[driverID]].work = false;
		} else if (operation == "r") {
			// Send a driver for a ride from a startNode to a finishNode
			fin >> nameStart >> nameEnd >> rateScore;
			// Get ID of start and finish nodes
			crossStartID = crossMap.get(nameStart);
			crossEndID = crossMap.get(nameEnd);
			minDriver = findDriver(crossStartID, mini);
			// None of the drivers can get to the starting node
			if (mini == INF) {
				fout << "Soferi indisponibili" << '\n';
				continue;
			}
			// There is no path between start node and finish node
			// and there are no neighbour nodes
			if (computedDist[crossStartID][crossEndID] == INF
				&& newDestination[crossStartID][crossEndID] == -1) {
				fout << "Destinatie inaccesibila" << '\n';
				continue;
			}
			driverUpdate(minDriver, rateScore, crossStartID, crossEndID, mini);
		} else if (operation == "top_dist") {
			// Print top drivers according to their traveled distance
			fin >> nr;
			// Sort top drivers
			quicksort(drivers, 0, nrDrivers - 1, cmpDist);
			nr = minimum(nr, nrDrivers);
			// Print
			for (unsigned int j = 0; j < nr; ++j) {
				position[drivers[j].id] = j;
				fout <<  drivers[j].name << ':' << drivers[j].usage << ' ';
			}
			// Update their position in the array
			for (unsigned int j = nr; j < drivers.size(); ++j) {
				position[drivers[j].id] = j;
			}
			fout << '\n';
		} else if (operation == "top_rating") {
			// Print top drivers according to their rating
			fin >> nr;
			// Sort top drivers
			quicksort(drivers, 0, nrDrivers - 1, cmpRate);
			nr = minimum(nr, nrDrivers);
			// Print
			for (unsigned int j = 0; j < nr; ++j) {
				position[drivers[j].id] = j;
				fout << std::fixed;
				fout << std::setprecision(3) << drivers[j].name << ':';
				fout << drivers[j].rating << ' ';
			}
			// Update their position in the array
			for (unsigned int j = nr; j < drivers.size(); ++j) {
				position[drivers[j].id] = j;
			}
			fout << '\n';
		} else if (operation == "info") {
			// Get info of a specific driver
			fin >> name;
			driverID = driverMap.get(name);
			printDriverStatus(name, driverID, fout);
		} else if (operation == "top_rides") {
			// Print top drivers according to their rating
			fin >> nr;
			// Sort drivers
			quicksort(drivers, 0, nrDrivers - 1, cmpRides);
			nr = minimum(nr, nrDrivers);
			// Print
			for (unsigned int j = 0; j < nr; ++j) {
				position[drivers[j].id] = j;
				fout <<  drivers[j].name << ':';
				fout << drivers[j].nrClients << ' ';
			}
			// Update their position in the array
			for (unsigned int j = nr; j < drivers.size(); ++j) {
				position[drivers[j].id] = j;
			}
			fout << '\n';
		}
	}
}
void solver::task5_solver(std::ifstream& fin, std::ofstream& fout) {
	// Get all specific destination from which a driver can get
	int gass, nrCross, driverID;
	std::string name, crossRoad;
	std::vector<visits> canVisit;
	fin >> gass >> name;
	fin >> nrCross;
	// Get driver ID
	driverID = driverMap.get(name);
	for (int i = 1; i <= nrCross; ++i) {
		fin >> crossRoad;
		// Mark specific intersections
		crossMap.put(crossRoad, MARK);
	}
	// Get all destinations
	canVisit = visitNodes(drivers[position[driverID]].crossID, gass);
	// Sort destinations according to the task
	quicksort(canVisit, 0, canVisit.size() - 1, cmpCross);
	// Print
	for (unsigned int i = 0; i < canVisit.size(); ++i) {
		fout << canVisit[i].name << ' ';
	}
}
