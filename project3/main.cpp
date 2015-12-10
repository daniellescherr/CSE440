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
#include <map>

using std::cout; using std::cin; using std::endl;
using std::string; using std::vector; using std::ifstream;
using std::istringstream; using std::stringstream;
using std::map; using std::remove;

int countInstances(vector<string> data, int attributeCount, string desired, int desiredPos)
{
	cout << "number of attributes: " << attributeCount << ", looking for: " << desired << " at " << desiredPos << endl;
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
			cout << "value: " << value << " at " << j << endl;
			if (j == desiredPos)
			{
				cout << "checking for match between " << value.c_str() << endl; cout << "    and " << desired.c_str() << endl;
				if (strncmp (value.c_str(), desired.c_str(), 1) == 0)
				{
					cout << "TRUE" << endl;
					positive++;
				}
				break;
			}
		}
	}
	cout << desired << " was present " << positive << " out of " << totalData << endl;

	return positive;
}

float CalculateLogValue(float frac1, float frac2)
{
	return (-frac1*log2(frac1) + -frac2*log2(frac2));
}

float CalculateEntropy(vector<string> data, int attributeCount)
{
	int trueCount = countInstances(data, attributeCount, "T", attributeCount);
	float trueFrac = (float)trueCount/float(data.size());
	// cout << " frac: " << trueFrac << " log: " << CalculateLogValue(trueFrac, 1-trueFrac) << endl;
	return CalculateLogValue(trueFrac, 1-trueFrac);
}

float CalculateInformationGain(vector<string> data, int attributeCount, string attribute)
{
	//TODO this isn't entirely right- the calculation is off :(
	cout << "info gain for: " << attribute << endl;
	int attributeTrueCount = countInstances(data, attributeCount, "SMALL", 0);
	float attributeFrac = (float)attributeTrueCount/float(data.size());
	return CalculateLogValue(attributeFrac, 1-attributeFrac);
}

void GenerateDecisionTree(vector<string> attributes, int attributeCount, map<string, vector<string>> attributeValues, vector<string> data)
{
	float entropy = CalculateEntropy(data, attributeCount);
	cout << "entropy: " << entropy << endl;

	//TODO for all attributes associated with size, etc
	float gain = CalculateInformationGain(data, attributeCount, attributes[0]);
	cout << "gain for: " << attributes[0] << " (not really) is " << gain << endl;

	// for (int i = 0; i < attributes.size(); i++)
	// {
		//attributeValues[attributes[i]] returns vector
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
	map<string, vector<string>> attributeValues;

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
					// cout << "    RELATION name: " << dataSetName << endl;
				}
				else if (word == "@attribute")
				{
					// getline(infile, nextLineWord, ' ');
					getline(iss, nextWord, ' ');
					attributeName = nextWord;
					cout << "    ATTRIBUTE name: " << attributeName << endl;
					attributes.push_back(attributeName);
					categoryName = nextWord;
					// cout << "    CATEGORY NAME: " << categoryName << endl;
					// cout << "attributes " << endl;
					while (getline(iss, nextWord, ' '))
					{
						remove(nextWord.begin(), nextWord.end(), '{');
						remove(nextWord.begin(), nextWord.end(), '}');
						remove(nextWord.begin(), nextWord.end(), ',');
						cout << "        word:" << nextWord << endl;
						attributeValues.insert(std::make_pair(attributeName, attributes));

					}
					attributeCount++;
				}
				else if (strncmp (word.c_str(), "@data",5) == 0)
				{
					string dataLine = "";
					while (getline(infile, dataLine))
					{
						// cout << "dataline: " << dataLine << endl;
						instances.push_back(dataLine);
						dataInstancesCount++;
					}
				}
			}
	   }
   }
	cout << "The number of training instances: " << dataInstancesCount << endl;
	attributeCount--;
	GenerateDecisionTree(attributes, attributeCount, attributeValues, instances);

	infile.close();
	return 0;

}
