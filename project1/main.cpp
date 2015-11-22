//
//  main.cpp
//  proj1
//
//  Created by Danielle Scherr on 10/4/15.
//  Copyright © 2015 Danielle Scherr. All rights reserved.
//

#include <iostream>

#include <string>
#include <vector>
#include "tree.h"

using std::cout; using std::cin; using std::endl;
using std::string; using std::vector;
using std::pair; using std::stoi;

int main(int argc, char * argv[])
{
	if ((argc != 4) || (string(argv[1]) != "Astar") || (string(argv[2]) != "-h")) {
		std::cerr << "ERROR input should be in the format: Astar -h number" << endl;
		exit(1);
	}
	
	cout << "Please type in the initial state: ";
	
	char input[19];
	cin.getline(input,sizeof(input));
	
	vector<int> puzzleVector;
	puzzleVector.push_back(atoi(string(1,input[1]).c_str()));
	puzzleVector.push_back(atoi(string(1,input[3]).c_str()));
	puzzleVector.push_back(atoi(string(1,input[5]).c_str()));
	puzzleVector.push_back(atoi(string(1,input[7]).c_str()));
	puzzleVector.push_back(atoi(string(1,input[9]).c_str()));
	puzzleVector.push_back(atoi(string(1,input[11]).c_str()));
	puzzleVector.push_back(atoi(string(1,input[13]).c_str()));
	puzzleVector.push_back(atoi(string(1,input[15]).c_str()));
	puzzleVector.push_back(atoi(string(1,input[17]).c_str()));

	Tree puzzleTree(puzzleVector);

	puzzleTree.chooseMove(*puzzleTree.root, stoi(string(argv[3])));
	cout << "The total Number of Nodes Expanded: " << puzzleTree.numberOfExpandedNodes << endl;
	cout << "Initial State: ";
	puzzleTree.printVector(puzzleVector);
	for (int i = 0; i < puzzleTree.solutionSequence.size(); i++)
		cout << puzzleTree.solutionSequence[i] << endl;
	cout << "The total Number of Operators Applied: " << puzzleTree.numberOfOperators << endl;
	return 0;
	
}


