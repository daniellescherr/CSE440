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
	cout << "number of attributes: " << attributeCount << ", looking for: " << desired << " at " << desiredPos << endl;
	int present = 0, positive = 0;
	int totalData = data.size();
	// cout << "data size: " << totalData << endl;
	for (int i = 0; i < totalData; i++)
	{
		istringstream iss(data[i]);
		string attribute = data[i];
		string value = "";
		for (int j = 0; j <= attributeCount; j++)		//
		{
			// cout << "COUNT: " << j << " count: " << attributeCount << endl;
			getline(iss, value, ',');
			// cout << "value: " << value << " at " << j << endl;
			if (j == desiredPos)
			{
				// cout << "checking to match " << desired.c_str() << endl;
				if (strncmp (value.c_str(), desired.c_str(), 1) == 0)
				{
					if (j != attributeCount)
					{
						// cout << "not at end, but match, checking for positive";
						string boolean = "";
						while(getline(iss, boolean, ',')){};
						// cout << " boolean " << boolean << endl;
						if (strncmp (boolean.c_str(), "T",1) == 0) positive++;
					}
					// cout << "TRUE" << endl;
					present++;
				}
				break;
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
	for (int i = 0; i < totalData; i++)
	{
		istringstream iss(data[i]);
		string attribute = data[i];
		string value = "";
		// cout << "attribute " << attribute << endl;
		// cout << "looking for: " << desired << endl;
		//TODO for some reason "small" doesn't work
		int loc = attribute.find(desired);
		// cout << "found at: " << loc << endl;
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
	// cout << "frac1 " << frac1 << endl;
	// cout << "frac2 " << frac2 << endl;

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

float CalculateInformationGain(vector<string> data, int attributeCount, vector<string> attributes, int index)
{
	// cout << "info gain for: " << attribute << endl;
	// vector<string> vecAtt = attributeValues[attribute];
	float attributeGain = 0;

	for (int i = 0; i < attributes.size(); i++)
	{
		cout << "    calculating for: " << attributes[i] << endl;
		pair<int, int> attributeTrueCount = countInstances(data, attributeCount, attributes[i], index);
		cout << attributes[i] << " was present " << attributeTrueCount.first << " out of " << data.size() << " and positive " << attributeTrueCount.second << endl;
		float attributeFrac = (attributeTrueCount.first == 0 || attributeTrueCount.second == 0) ? 0 : (float)attributeTrueCount.first/float(data.size());
		// cout << "attributeFrac: " << attributeFrac << endl;
		float logFrac = (attributeTrueCount.first == 0 || attributeTrueCount.second == 0) ? 0 : (float)(attributeTrueCount.second/(float)attributeTrueCount.first);
		float logVal = CalculateLogValue(logFrac, 1-logFrac);
		// cout << "logVal: " << logVal << endl;
		attributeGain += attributeFrac * logVal;
		cout << "new gain " << attributeGain << endl;

	}
	cout << "total gain " << attributeGain << endl;


	return attributeGain;
}

void GenerateDecisionTree(int attributeCount, map<string, vector<string>> attributeValues, vector<string> data)
{
	string root = "";
	float entropy = CalculateEntropy(data, attributeCount);
	cout << "entropy: " << entropy << endl;


	// cout << "calculating info gain" << endl;

	float gainSize = CalculateInformationGain(data, attributeCount, attributeValues["size"], 0);
	cout << endl;
	float gainColor = CalculateInformationGain(data, attributeCount, attributeValues["color"], 1);
	cout << endl;
	float gainShape = CalculateInformationGain(data, attributeCount, attributeValues["shape"], 2);


	float gainSizeDiff = entropy - gainSize;
	float gainColorDiff = entropy - gainColor;
	float gainShapeDiff = entropy - gainShape;
	vector<float> vect = {gainSizeDiff, gainColorDiff, gainShapeDiff};
	sort(vect.begin(), vect.end());
	// cout << "MAX: " << vect[vect.size()-1];
	if (gainSizeDiff == vect[vect.size()-1]) root = "SIZE";
	else if (gainColorDiff == vect[vect.size()-1]) root = "COLOR";
	else if (gainShapeDiff == vect[vect.size()-1]) root = "SHAPE";

	cout << "ROOT: " << root << endl << endl << endl;

	//TODO loop through for iterations

	cout << "children " << endl;
	cout << "entropy for " << root << endl;
	// cout << "this: " << attributeValues["size"][0] << endl;
	vector<string> childData = createNewData(data, /*attributeValues["size"][i]*/ "BIG");
	for (auto elem : childData)
	{
		cout << "elem: " << elem << endl;
	}
	int childAttributeCount = attributeCount - 1;
	// cout << "child data size " << childData.size() << endl;
	float childEntropy = CalculateEntropy(childData, childAttributeCount);
	cout << "child entropy " << childEntropy << endl;



	cout << endl << "calculating next step" << endl;
	if (root == "SIZE")
	{
		string child = "";
		//TODO change index to match new data spots
		float childGainColor = CalculateInformationGain(childData, childAttributeCount, attributeValues["color"], 0);
		float childGainShape = CalculateInformationGain(childData, childAttributeCount, attributeValues["shape"], 1);


		float childGainColorDiff = childEntropy - childGainColor;
		float childGainShapeDiff = childEntropy - childGainShape;
		cout << "childGainColorDiff " << childGainColorDiff << endl;
		cout << "childGainShapeDiff " << childGainShapeDiff << endl;

		vector<float> childVect = {childGainColorDiff, childGainShapeDiff};
		sort(childVect.begin(), childVect.end());
		// cout << "MAX: " << vect[vect.size()-1];
		if (childGainColorDiff == childVect[childVect.size()-1]) child = "COLOR";
		else if (childGainShapeDiff == childVect[childVect.size()-1]) child = "SHAPE";

		cout << "CHILD: " << child << endl << endl << endl;


	}
	else if (root == "COLOR")
	{
		float childGainSize = CalculateInformationGain(childData, childAttributeCount, attributeValues["size"], 1);

		float gainSizeDiff = entropy - gainSize;

	}
	else if (root == "SHAPE")
	{
		;
	}

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
					// getline(infile, nextLineWord, ' ');
					getline(iss, nextWord, ' ');
					attributeName = nextWord;
					// cout << "    ATTRIBUTE name: " << attributeName << endl;
					// attributes.push_back(attributeName);
					categoryName = nextWord;
					// cout << "    CATEGORY NAME: " << categoryName << endl;
					// cout << "attributes " << endl;
					while (getline(iss, nextWord, ' '))
					{
						remove(nextWord.begin(), nextWord.end(), '{');
						remove(nextWord.begin(), nextWord.end(), '}');
						nextWord.erase(remove(nextWord.begin(), nextWord.end(), ','), nextWord.end());
						// cout << "        word:" << nextWord << endl;
						attributes.push_back(nextWord);

					}
					attributeValues.insert(std::make_pair(attributeName, attributes));
					attributes.clear();

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
	attributeValues.erase(categoryName);
	GenerateDecisionTree(attributeCount, attributeValues, instances);

	infile.close();
	return 0;

}
