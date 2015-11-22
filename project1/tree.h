
#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::cout;
using std::endl;
using std::map;
using std::make_pair;


struct Tree
{
public:
	Tree() {root = 0;}
	Tree(vector<int> puzzle_vector)
	{
		root = new Node(puzzle_vector);
		
		root->down_child = nullptr; root->up_child = nullptr;
		root->left_child = nullptr; root->right_child = nullptr;
		root->parent = nullptr;
	}
	~Tree() {delete root;}
	
	class Node
	{
	public:
		friend class Tree;
		
		Node(vector<int> &puzzle_vector)
		{			
			
			board = puzzle_vector;
			down_child = nullptr; up_child = nullptr;
			left_child = nullptr; right_child = nullptr;
			parent = nullptr;
			
		}
		~Node() {delete down_child;  delete up_child;
			delete left_child; delete right_child; delete parent;}
		
		vector<int> &Data() {return board;}
		
		
	private:
		Node    *down_child;
		Node    *up_child;
		Node    *left_child;
		Node    *right_child;
		Node	*parent;
		vector<int>  board;
		
	};
	
	void chooseMove (Node current, int h)
	{
		vector<pair<pair<int, string>, vector<int>>> moveDirection;
		vector<Node> nodeVector;
		
		int pos = 0;
		pair<int, string> minHeuresticDirection(50, "");
		
		nodeVector.push_back(current);
		allMoves.push_back(current);
		
		while (!nodeVector.empty())
		{
			if (current.board == goalStateNode.board)
			{
				goalStateNode.parent = current.parent;
				break;
			}
			
			int whiteSpacePosition = -1;
			int downHeurestic = 50;
			int prevDownHeurestic = 0;
			int upHeurestic = 50;
			int prevUpHeurestic = 0;
			int leftHeurestic = 50;
			int prevLeftHeurestic = 0;
			int rightHeurestic = 50;
			int prevRightHeurestic = 0;
			vector<Node> children;
			vector<pair<pair<int, string>, vector<int>>> moveDirection;
			string currentStep = "";
			
			for (int i = 0; i < current.board.size(); i++)
				if (current.board[i] == 0)
					whiteSpacePosition = i;
			
			
			if (whiteSpacePosition + 3 <= 8)
			{
				vector<int> childBoard = current.board;
				int downSwapPos = whiteSpacePosition + 3;
				childBoard[whiteSpacePosition] = current.board[downSwapPos];
				childBoard[downSwapPos] = 0;
				current.down_child = new Node(childBoard);
				downHeurestic = generateHeurestic(childBoard, h, prevDownHeurestic);
				children.push_back(childBoard);
				moveDirection.push_back(make_pair(make_pair(downHeurestic, DOWN), childBoard));
			}
			if (whiteSpacePosition - 3 >= 0)
			{
				vector<int> childBoard = current.board;
				int upSwapPos = whiteSpacePosition - 3;
				childBoard[whiteSpacePosition] = current.board[upSwapPos];
				childBoard[upSwapPos] = 0;
				current.up_child = new Node(childBoard);
				upHeurestic = generateHeurestic(childBoard, h, prevUpHeurestic);
				children.push_back(childBoard);
				moveDirection.push_back(make_pair(make_pair(upHeurestic, UP), childBoard));

			}
			if ((whiteSpacePosition % 3) != 0)
			{
				vector<int> childBoard = current.board;
				int leftSwapPos = whiteSpacePosition - 1;
				childBoard[whiteSpacePosition] = current.board[leftSwapPos];
				childBoard[leftSwapPos] = 0;
				current.left_child = new Node(childBoard);
				leftHeurestic = generateHeurestic(childBoard, h, prevLeftHeurestic);
				children.push_back(childBoard);
				moveDirection.push_back(make_pair(make_pair(leftHeurestic, LEFT), childBoard));
			}
			if (((whiteSpacePosition + 1) % 3) != 0)
			{
				vector<int> childBoard = current.board;
				int rightSwapPos = whiteSpacePosition + 1;
				childBoard[whiteSpacePosition] = current.board[rightSwapPos];
				childBoard[rightSwapPos] = 0;
				current.right_child = new Node(childBoard);
				rightHeurestic = generateHeurestic(childBoard, h, prevRightHeurestic);
				children.push_back(childBoard);
				moveDirection.push_back(make_pair(make_pair(rightHeurestic, RIGHT), childBoard));
			}
			
			for (int i = 0; i < moveDirection.size(); i++)
			{
				if (!(moveIsRepeat(moveDirection[i].second)) && (moveDirection[i].first.first < minHeuresticDirection.first))
				{
					minHeuresticDirection = moveDirection[i].first;
					pos = i;
				}
			}
			
			
			if (minHeuresticDirection.second == DOWN)
			{
				current = *current.down_child;
				allMoves.push_back(current);
				numberOfExpandedNodes++;
				prevDownHeurestic += downHeurestic;
			}
			else if (minHeuresticDirection.second == UP)
			{
				current = *current.up_child;
				allMoves.push_back(current);
				numberOfExpandedNodes++;
				prevUpHeurestic += upHeurestic;
			}
			else if (minHeuresticDirection.second == LEFT)
			{
				current = *current.left_child;
				allMoves.push_back(current);
				numberOfExpandedNodes++;
				prevLeftHeurestic += leftHeurestic;
			}
			else
			{
				current = *current.right_child;
				allMoves.push_back(current);
				numberOfExpandedNodes++;
				prevRightHeurestic += rightHeurestic;
			}
			numberOfOperators++;
			currentStep = minHeuresticDirection.second + ": ";
			for (int i = 0; i < current.board.size(); i++)
				currentStep.append(std::to_string(current.board[i]) + " ");
			solutionSequence.push_back(currentStep);
			minHeuresticDirection = std::make_pair(50, "");
		}
	}
	
	int generateHeurestic(vector<int> potentialBoard, int heurestic, int prevH)
	{
		int h = 0;
		
		if (heurestic == 1)
		{
			for (int i = 0; i < potentialBoard.size(); i++)
			{
				if (potentialBoard[i] == 0) continue;
				if (potentialBoard[i] != goalStateVector[i])
					h++;
			}
		}
		else if (heurestic == 2)
		{
			for (int i = 0; i < potentialBoard.size(); i++)
			{
				if (potentialBoard[i] == 0) continue;
				for (int n = 0; n < goalStateVector.size(); n++)
				{
					if (potentialBoard[i] == goalStateVector[n])
					{
						int iCoordX = getXCoord(i);
						int iCoordY = getYCoord(i);
						int nCoordX = getXCoord(n);
						int nCoordY = getYCoord(n);
						
						h+= abs(iCoordX- nCoordX) + abs(iCoordY - nCoordY);
					}
				}
			}
		}
		else if (heurestic == 3)
		{
			for (int i = 0; i < potentialBoard.size(); i++)
			{
				if (potentialBoard[i] == 0) continue;
				for (int n = 0; n < goalStateVector.size(); n++)
				{
					if (potentialBoard[i] == goalStateVector[n])
					{
						int iCoordX = getXCoord(i);
						int iCoordY = getYCoord(i);
						int nCoordX = getXCoord(n);
						int nCoordY = getYCoord(n);
						
						if (iCoordX != nCoordX) h++;
						if (iCoordY != nCoordY) h++;
					}
				}
			}
		}
		return h += prevH;
	}
	
	int getXCoord(int i)
	{
		return i % 3 + 1;
	}
	
	int getYCoord(int i)
	{
		int yCoord;
		if (i < 3) yCoord = 1;
		else if (i >= 3 && i < 6) yCoord = 2;
		else yCoord = 3;
		return yCoord;
	}
	
	bool moveIsRepeat (Node possibleNextMove)
	{
		for (int i = 0; i < allMoves.size(); i++)
		{
			if (allMoves[i].board == possibleNextMove.board)
				return true;
		}
		return false;
	}
	
	void printVector (vector<int> print)
	{
		cout << "(";
		for (int i = 0; i < print.size()-1; i++)
			cout << print[i] << " ";
		cout << print[print.size()-1];
		cout << ")";
		cout << endl;
	}
	
	//private:
	Node *root;
	
	vector<Node> allMoves;
	int numberOfExpandedNodes = 0;
	int numberOfOperators = 0;
	vector<int> goalStateVector = {1, 2, 3, 8, 0, 4, 7, 6, 5};
	Node goalStateNode = Node(goalStateVector);
	const string UP = "UP";
	const string DOWN = "DOWN";
	const string LEFT = "LEFT";
	const string RIGHT = "RIGHT";
	vector<string> solutionSequence;
};

#endif