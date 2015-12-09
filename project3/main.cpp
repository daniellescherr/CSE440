//
//  main.cpp
//  proj1
//
//  Created by Danielle Scherr on 11/24/15.
//  Copyright © 2015 Danielle Scherr. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>

using std::cout; using std::cin; using std::endl;
using std::string; using std::vector; using std::ifstream;
using std::istringstream; using std::stringstream;

int countInstances(vector<string> data, int attributeCount, string desired)
{
	cout << "data: " << attributeCount << ", " << desired << endl;
	int positive = 0;
	int totalData = data.size();
	for (int i = 0; i < totalData; i++)
	{
		istringstream iss(data[i]);
		string attribute = data[i];
		string value = "";
		for (int j = 0; j <= attributeCount; j++)		//
		{
			// cout << "COUNT: " << j << " count: " << attributeCount << endl;
			getline(iss, value, ',');
			// cout << "value: " << value << endl;
			if (j == attributeCount)		//T F indicator
			{
				if (strncmp (value.c_str(), desired.c_str(), 1) == 0)
				{
					// cout << "TRUE" << endl;
					positive++;
				}
				break;
			}
		}
	}
	cout << desired << " was present " << positive << " out of: " << totalData << endl;

	return positive;
}

float CalculateLogValue(float frac)
{
	return (-frac*log2(frac));
}

// float CalculateInformationGain()
// {
//
// }

void GenerateDecisionTree(vector<string> attributes, int attributeCount, vector<string> data)
{
	int trueCount = countInstances(data, attributeCount, "T");
	float trueFrac = (float)trueCount/float(data.size());
	cout << " frac: " << trueFrac << " log: " << CalculateLogValue(trueFrac) << endl;



	// for (int i = 0; i < attributes.size(); i++)
	// {
		//CalculateInformationGain
		//add to gain vector
	// }
	//max of information gain is root
}



int main(int argc, char * argv[])
{
	if ((argc != 3) || (string(argv[1]) != "DT")) {
		std::cerr << "ERROR input should be in the format: DT file.data" << endl;
		exit(1);
	}

	string line;
	string dataSetName = "", attributeName = "";
	int dataInstancesCount = 0;
	int attributeCount = 0;
	vector<string> instances;
	vector<string> attributes;

	ifstream infile;
	infile.open(string(argv[2]));

	if (infile.is_open())
	{
		while (getline(infile, line))
		{
			istringstream iss(line);
			vector<string> tokens;
			// cout << "LINE: " << line << endl;
			for (string word; getline(iss, word, ' '); tokens.push_back(word))
			{
				if (line.at(0) == '%') break;

				string nextWord = "";
				string categoryName = "";
				if (word == "@relation")
				{
					getline(iss, nextWord, ' ');
					dataSetName = nextWord;
					cout << "    RELATION name: " << dataSetName << endl;
				}
				else if (word == "@attribute")
				{
					// getline(infile, nextLineWord, ' ');
					getline(iss, nextWord, ' ');
					attributeName = nextWord;
					cout << "    ATTRIBUTE name: " << attributeName << endl;
					attributes.push_back(attributeName);
					categoryName = nextWord;
					cout << "    CATEGORY NAME: " << categoryName << endl;
					attributeCount++;
				}
				else if (strncmp (word.c_str(), "@data",5) == 0)
				{
					string dataLine = "";
					while (getline(infile, dataLine))
					{
						cout << "dataline: " << dataLine << endl;
						instances.push_back(dataLine);
						dataInstancesCount++;
					}
				}
			}
	   }
   }
	cout << "The number of training instances: " << dataInstancesCount << endl;
	attributeCount--;
	GenerateDecisionTree(attributes, attributeCount, instances);

	infile.close();
	return 0;

}
