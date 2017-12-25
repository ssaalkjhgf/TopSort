// TopSort.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <cstddef>

using namespace std;

#define INPUT_FILE_PATH "course_inf.txt"
#define BUFFER_SIZE 1000

template <typename E>
E max(E a, E b) {
	return a > b ? a : b;
}

class Vertex {
public:
	int ID;
	string name;
	int length;
	int term;
	vector<int> neighbour;
	vector<int> prequisite;
};

class Graph {
private:
	vector<bool> visited;
	void topHelp(Vertex v, int i);
public:
	vector<Vertex> vertex;
	vector<int> depth;
	vector<int> deadline;
	void TopSort();
};

int main() {
	fstream input(INPUT_FILE_PATH, ios::in);
	Graph G;
	char record[BUFFER_SIZE] = { '\0' };
	string temp;
	vector<Vertex> sequence;
	int termCount[8];
	for (;;) {
		input.getline(record, BUFFER_SIZE);
		//cout << record << endl;
		if (strlen(record) < 2)
			continue;
		if (record[0] == '/' && record[1] == '/')
			continue;
		//cout << record << endl;
		stringstream term(record);
		for (int i = 0; i<8; i++) {
			term >> termCount[i];
		}
		break;
	}
	
	vector<Vertex> result[8];
	for (;;) {
		input.getline(record, BUFFER_SIZE);
		//cout << record << endl;
		Vertex v;
		if (strlen(record) < 2)
			continue;
		if (record[0] == '/' && record[1] == '/')
			continue;
		stringstream ss(record);
		ss >> temp;
		v.ID = (temp[1] - '0') * 10 + (temp[2] - '0');
		ss >> temp;
		v.name = temp;
		ss >> temp;
		v.length = temp[0] - '0';
		ss >> temp;
		v.term = temp[0] - '0';
		for (;;) {
			if (ss.eof())
				break;
			ss >> temp;
			if (temp == "0")
				break;
			int k = (temp[1] - '0') * 10 + (temp[2] - '0');
			v.prequisite.push_back(k-1);
			G.vertex[k - 1].neighbour.push_back(v.ID-1);
		}
		if (v.term == 0)
			G.vertex.push_back(v);
		else {
			result[v.term - 1].push_back(v);
		}
		if (input.eof())
			break;
	}
	for (int i = 0; i < G.vertex.size(); i++) {
		G.deadline.push_back(0);
		G.depth.push_back(0);
	}
	G.TopSort();
	int maxDeadline = 0;
	int maxDepth = 0;
	for (int i = 0; i < G.deadline.size(); i++) {
		if (G.deadline[i] > maxDeadline)
			maxDeadline = G.deadline[i];
	}
	for (int i = 0; i < G.depth.size(); i++) {
		if (G.depth[i] > maxDepth)
			maxDepth = G.depth[i];
	}
	vector<Vertex>* layer = new vector<Vertex>[maxDepth+1];
	int* currLayerPos = new int[maxDepth + 1];
	for (int i = 0; i <= maxDepth; i++) {
		for (int j = maxDeadline; j >= 0; j--) {
			for (int k = 0; k < G.deadline.size(); k++) {
				if (G.deadline[k] == j && G.depth[k] == i) {
					layer[i].push_back(G.vertex[k]);
				}
			}
		}
		currLayerPos[i] = 0;
	}
	for (int i = 0; i < layer[0].size(); i++) {
		if (result[0].size() == termCount[0])
			break;
		result[0].push_back(layer[0][i]);
		currLayerPos[0]++;
	}
	vector<Vertex> freeCourse;
	for (int i = currLayerPos[0]; i < layer[0].size(); i++) {
		freeCourse.push_back(layer[0][i]);
	}
	int curr = 0;
	int k = 1;
	for (int i=1; i<=maxDepth; i++) {
		for (int j = 0; j < layer[i].size(); j++) {
			result[k].push_back(layer[i][j]);
			if (result[k].size() >= termCount[k]) 
				k++;
		}
		if (freeCourse.size() > curr) {
			while (result[k].size() < termCount[k]) {
				result[k].push_back(freeCourse[curr]);
				curr++;
			}
		}
		k++;
	}
	input.close();
	for (int i = 0; i < 8; i++) {
		cout << "Term " << i << endl;
		for (vector<Vertex>::iterator j = result[i].begin(); j != result[i].end(); j++) {
			cout << '\t' << j->name << endl;
		}
	}
	cin.get();
	return 0;
}

void Graph::TopSort() {
	visited.clear();
	for (int i = 0; i<vertex.size(); i++) {
		visited.push_back(false);
	}
	for (int i = 0; i<vertex.size(); i++) {
		if (!visited[i])
			topHelp(vertex[i], i);
	}
}

void Graph::topHelp(Vertex v, int i) {
	visited[i] = true;
	for (vector<int>::iterator j = v.neighbour.begin(); j != v.neighbour.end(); j++) {
		depth[*j] = max(depth[*j], depth[i] + 1);
		if (!visited[*j])
			topHelp(vertex[*j], *j);
		deadline[i] = max(deadline[*j] + 1, deadline[i]);
	}
}

