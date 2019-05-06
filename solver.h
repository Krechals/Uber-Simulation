// Copyright 2019 Topala Andrei

#ifndef SOLVER_H_
#define SOLVER_H_
#include <vector>
#include <string>
#include "Hashmap.cpp"
#include "Sort.cpp"

#define INF (1<<30)
#define MARK -(1<<30)
#define VMAX 1097
#define MAXN 501

// Custom hash function
class solver {
 private:
	int N, M, nrDrivers;
	// Graph network with adjacency matrix
	// - used to fast search links between 2 nodes -
	int adjacency[MAXN][MAXN];
	// Distance between 2 nodes
	int computedDist[MAXN][MAXN];
	// Distance traveled by a driver when he goes to a neighbor node
	int neighborDist[MAXN][MAXN];
	// Destionation where a driver goes when a node is not accessible
	int newDestination[MAXN][MAXN];
	// Driver's position in the array after sortations
	int position[MAXN];

	// Intersections hashmap
	Hashtable<std::string, int> crossMap =
						Hashtable<std::string, int>(VMAX, customHash);
	// Drivers hashmap
	Hashtable<std::string, int> driverMap =
						Hashtable<std::string, int>(VMAX, customHash);
	// Graph network with list
	// - used to make DFS and BFS faster -
	std::vector<int> network[MAXN];
	// Intersections array
	std::vector<std::string> intersects;
	// Drivers array
	std::vector<status> drivers;

 public:
	int minimum(int A, int B);
	void deleteEdge(int node1, int node2);
	static int customHash(std::string key);
	int findDriver(int crossStartID, int &mini);
	void driverUpdate(int minDriver, int rateScore, int crossStartID,
						int crossEndID, int mini);
	void printDriverStatus(std::string name, int driverID,
						std::ofstream& fout);
	bool isPath(int id1, int id2);
	int distance(int id1, int id2);
	void preprocess(int id);
	std::vector<visits> visitNodes(int startID, int distance);
	void task1_solver(std::ifstream& fin, std::ofstream& fout);
	void task2_solver(std::ifstream& fin, std::ofstream& fout);
	void task3_solver(std::ifstream& fin, std::ofstream& fout);
	void task4_solver(std::ifstream& fin, std::ofstream& fout);
	void task5_solver(std::ifstream& fin, std::ofstream& fout);
};
#endif  // SOLVER_H_
