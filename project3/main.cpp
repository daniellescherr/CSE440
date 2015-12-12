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
using std::map; using std::remove; using std::make_pair;
using std::pair; using std::sort;



pair<int, int> countInstances(vector<string> data, int attributeCount, string desired, int desiredPos)
{
	int present = 0, positive = 0;
	int totalData = data.size();
	for (int i = 0; i < totalData; i++)
	{
		string line = data[i];
		string comma = ",";

		size_t pos = 0;
		string token;
		// std::cout << "line " << line << std::endl;
		while ((pos = line.find(comma)) != std::string::npos) {
		    token = line.substr(0, pos);
		    // std::cout << "token" << token << std::endl;
			if (strncmp (token.c_str(), desired.c_str(), desired.length()) == 0)
			{
				present++;
				string boolean = line.substr(line.length() - 2, line.length() - 1);
				// cout << "bool " << boolean << endl;
				if (strncmp(boolean.c_str(), "T", 1) == 0)
				{
					positive++;
				}
			}

		    line.erase(0, pos + comma.length());
		}
		char boolean = line.at(0);

		if (boolean == 'T')
		{
			if (desired == "T")
			{
				present++;
				positive++;
			}
		}

	}
	// cout << desired << " was present " << present << " out of " << totalData << " and positive " << positive << endl;
	return make_pair(present, positive);
}

vector<string> createNewData(vector<string> data, string desired)
{
	// cout << "looking for: " << desired << endl;
	int present = 0, positive = 0;
	int totalData = data.size();
	vector<string> newData;
	// cout << "total data" << totalData << endl;
	for (int i = 0; i < totalData; i++)
	{
		istringstream iss(data[i]);
		string attribute = data[i];
		string value = "";
		// cout << "attribute " << attribute << endl;
		// cout << "looking for: " << desired << endl;
		int loc = attribute.find(desired);
		if (loc == 0)
		{
			attribute.erase(0, desired.length() + 1);
			// cout << "new string: " << attribute << endl;
			newData.push_back(attribute);
		}
		else if (loc > 0)
		{
			attribute.erase(loc, desired.length() + 1);
			// cout << "new string: " << attribute << endl;
			newData.push_back(attribute);
		}
	}
	return newData;
}

float CalculateLogValue(float frac1, float frac2)
{
	return ((frac1 == 0) ? 0 : -frac1*log2(frac1) + ((frac2 == 0) ? 0 : -frac2*log2(frac2)));
}

float CalculateEntropy(vector<string> data, int attributeCount)
{
	// cout << "data num: " << data.size() << endl;
	pair<int, int> trueCount = countInstances(data, attributeCount, "T", attributeCount);
	float trueFrac = (float)trueCount.first/float(data.size());
	// cout << " frac: " << trueFrac << " log: " << CalculateLogValue(trueFrac, 1-trueFrac) << endl;
	return CalculateLogValue(trueFrac, 1-trueFrac);
}

float CalculateInformationGain(vector<string> data, int attributeCount, string attribute, int index, map<string, vector<string>> attributeValues)
{
	float attributeGain = 0;

	// cout << "looking for " << attribute << endl;
	for(auto attName : attributeValues) {
		if (attName.first != attribute) continue;
		for(auto vec : attName.second) {

			pair<int, int> attributeTrueCount = countInstances(data, attributeCount, vec, index);
			// cout << vec << endl; cout << "        was present " << attributeTrueCount.first << " out of " << data.size() << " and positive " << attributeTrueCount.second << endl;
			float attributeFrac = (attributeTrueCount.first == 0 || attributeTrueCount.second == 0) ? 0 : (float)attributeTrueCount.first/float(data.size());
			// cout << "attributeFrac: " << attributeFrac << endl;
			float logFrac = (attributeTrueCount.first == 0 || attributeTrueCount.second == 0) ? 0 : (float)(attributeTrueCount.second/(float)attributeTrueCount.first);
			float logVal = CalculateLogValue(logFrac, 1-logFrac);
			// cout << "logVal: " << logVal << endl;
			attributeGain += attributeFrac * logVal;
			index++;
		}
	}
		// cout << "additional gain " << attributeFrac * logVal << endl << endl;
	cout << "    total gain: " << attributeGain << endl;
	return attributeGain;
}

void GenerateDecisionTree(int attributeCount, map<string, vector<string>> attributeValues, vector<string> data, vector<string> attributeNames)
{
	string root = "";
	float entropy = CalculateEntropy(data, attributeCount);
	cout << "entropy: " << entropy << endl << endl;

	map<string, float> attrMap;

	// for (auto a : attributeNames) cout << a << endl;
	for (int i = 0; i < attributeNames.size(); i++ )
	{
		cout << "calculating for attribute " << attributeNames[i] << endl;
		float gainAttr = CalculateInformationGain(data, attributeCount, attributeNames[i], 0, attributeValues);
		float gainAttrDiff = entropy - gainAttr;
		cout << "    entropy - gain = " << gainAttrDiff << endl;
		attrMap[attributeNames[i]] = gainAttrDiff;

	}
	cout << "Map of attributes to information gain. " << endl;
	for(auto attName : attrMap) {
		    cout << attName.first << " -> " << attName.second << endl;
	}

	auto it=attrMap.begin();
	// cout << "it first " << it->first << endl;
	// cout << "it sec " << it->second << endl;
	if (attrMap["size"] == it->second) root = "size";
	else if (attrMap["act"] == it->second) root = "act";
	else if (attrMap["age"] == it->second) root = "age";
	else root = "color";

	cout << "ROOT: " << root << endl << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

	cout << "next, calculate infromation gain for root's children " << endl;
	vector<string> children = attributeValues[root];
	for (auto c : children) cout << "    " << c << endl;

}



int main(int argc, char * argv[])
{
	if ((argc != 3) || (string(argv[1]) != "DT")) {
		std::cerr << "ERROR input should be in the format: DT file.data" << endl;
		exit(1);
	}

	string line;
	string dataSetName = "", attributeName = "", categoryName = "";
	int dataInstancesCount = 0;
	int attributeCount = 0;
	vector<string> instances;
	vector<string> attributes;
	vector<string> attributeNames;
	map<string, vector<string>> attributeValues;
	map<string, vector<string>> newData;

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
				if (word == "@relation")
				{
					getline(iss, nextWord, ' ');
					dataSetName = nextWord;
					// cout << "    RELATION name: " << dataSetName << endl;
				}
				else if (word == "@attribute")
				{
					getline(iss, nextWord, ' ');
					attributeName = nextWord;
					categoryName = nextWord;
					while (getline(iss, nextWord, ' '))
					{
						if (nextWord.length() <= 0) continue;
						remove(nextWord.begin(), nextWord.end(), '{');
						remove(nextWord.begin(), nextWord.end(), '}');
						nextWord.erase(remove(nextWord.begin(), nextWord.end(), ','), nextWord.end());
						attributes.push_back(nextWord);

					}
					attributeValues.insert(std::make_pair(attributeName, attributes));
					attributeNames.push_back(attributeName);
					attributes.clear();

					attributeCount++;
				}
				else if (strncmp (word.c_str(), "@data",5) == 0)
				{
					string dataLine = "";
					while (getline(infile, dataLine))
					{
						instances.push_back(dataLine);
						dataInstancesCount++;
					}
				}
			}
	   }
   }
	cout << "The number of training instances: " << dataInstancesCount << endl;
	attributeCount--;
	attributeNames.erase(attributeNames.begin() + 4);
	attributeValues.erase(categoryName);
	GenerateDecisionTree(attributeCount, attributeValues, instances, attributeNames);

	infile.close();
	return 0;

}
