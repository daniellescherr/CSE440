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
	// cout << "number of attributes: " << attributeCount;
	// cout << "looking for: " << desired << endl;// << " at " << desiredPos << endl;
	int present = 0, positive = 0;
	int totalData = data.size();
	// cout << "data size: " << totalData << endl;
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
			// if (token == desired){ cout << "increasing present " << endl; present++; }
			if (strncmp (token.c_str(), desired.c_str(), desired.length()) == 0)
			{
				/*cout << "increasing present " << endl;*/
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
		// std::cout << "string after " << s << std::endl;
		char boolean = line.at(0);
		// cout << "bool " << boolean << endl;

		if (boolean == 'T')
		{
			// cout << "desired: " << desired << endl;
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

float CalculateInformationGain(vector<string> data, int attributeCount, string attribute, int index, map<string, vector<string>> attributeValues)
{
	float attributeGain = 0;
	// cout << "size " << attributes.size() << endl;
	// map<string, vector<string>> childAttributeData;
	//
	// cout << "looking for " << attribute << endl;
	for(auto attName : attributeValues) {
		if (attName.first != attribute) continue;
		for(auto vec : attName.second) {

		    // cout << "key: " << attName.first << " -> ";
		    // cout << vec << endl;
			// if (attribute.length() <= 0) return 0;
			// cout << "    calculating for: " << vec << endl;
			// for (auto d : data) cout << "        data " << d << endl;
			pair<int, int> attributeTrueCount = countInstances(data, attributeCount, vec, index);
			cout << vec << endl; cout << "        was present " << attributeTrueCount.first << " out of " << data.size() << " and positive " << attributeTrueCount.second << endl;
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

	// }
	cout << "total gain " << attributeGain << endl;


	return attributeGain;
}

void GenerateDecisionTree(int attributeCount, map<string, vector<string>> attributeValues, vector<string> data, vector<string> attributeNames)
{

	for(auto attName : attributeValues) {
		for(auto vec : attName.second) {

		    cout << "key: " << attName.first << " -> ";
		    cout << vec << endl;
		}
	}

	string root = "";
	float entropy = CalculateEntropy(data, attributeCount);
	cout << "entropy: " << entropy << endl << endl << endl;


	// cout << "calculating info gain" << endl;
	map<string, float> attrMap;

	// cout << "names size " << attributeNames.size() << endl;
	// for (auto a : attributeNames) cout << a << endl;
	for (int i = 0; i < attributeNames.size(); i++ )
	{
		cout << "calculating for attribute " << attributeNames[i] << endl;
		float gainAttr = CalculateInformationGain(data, attributeCount, attributeNames[i], 0, attributeValues);
		float gainAttrDiff = entropy - gainAttr;
		cout << "gain diff " << gainAttrDiff << endl;
		attrMap[attributeNames[i]] = gainAttrDiff;

	}
	for(auto attName : attrMap) {
		    cout << "THIS key: " << attName.first << " -> " << attName.second << endl;
	}
	//
	// float attrMax = 0;
	// for (auto it=attrMap.begin(); it!=attrMap.end(); ++it)
	// {
    // 	float num = it->first;
	// 	cout << "num: " << num << endl;
	// 	if (num >= attrMax) root = it->second;
	// }

	auto it=attrMap.begin();
	// cout << "it first " << it->first << endl;
	// cout << "it sec " << it->second << endl;
	if (attrMap["size"] == it->second) root = "size";
	else if (attrMap["act"] == it->second) root = "act";
	else if (attrMap["age"] == it->second) root = "age";
	else root = "color";


	cout << "ROOT: " << root << endl << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;


	





	//
	// vector<string> childData;
	// vector<float> childEntropyVector;
	// int childAttributeCount = 0;
	// float childEntropy = 0;
	// string nextChild = "";
	// // cout << "entropy for " << root << endl;
	//
	// map<string, vector<string>> childAttributeData;
	//
	// // for(auto attName : attributeValues) {
	// // 	for(auto vec : attName.second) {
	// //
	// // 	    cout << "key: " << attName.first << " -> ";
	// // 	    cout << vec << endl;
	// // 	}
	// // }
	//
	// cout << "remaining attributes " << endl;
	// 	for (auto att : attributeNames) cout << "    " << att << endl;
	//
	// // cout << "attribute size " << attributeNames.size() << endl;
	// // cout << "item " << attributeNames[0] << endl;
	//
	//
	// //TODO loop through for iterations
	// for (int i = 0; i < attributeNames.size()-1; i++)
	// {
	// 	// cout << "i " << i << endl;
	// 	map<string, vector<string>> attributeValuesCopy = attributeValues;
	// 	// if (attributeValuesCopy[attributeNames[0]][i] == "") break;
	//
	// 	// cout << attributeNames[0] << endl;
	// 	string newAttr = attributeValuesCopy[attributeNames[0]][i];
	//
	// 	// cout << endl << "this: " << newAttr << endl;
	// 	if (newAttr.length() <= 0) continue;
	// 	childData = createNewData(data, attributeValuesCopy[attributeNames[0]][i]);
	// 	cout << "New data set after iteration: " << endl;
	// 	for (auto elem : childData)
	// 	{
	// 		cout << "    " << elem << endl;
	// 	}
	// 	childAttributeData.insert(make_pair(attributeValuesCopy[attributeNames[0]][i], childData));
	// 	childAttributeCount = attributeCount - 1;
	// 	// cout << "child data size " << childData.size() << endl;
	// 	float childEntropyTemp = (childData.size() == 0) ? 0 : CalculateEntropy(childData, childAttributeCount);
	// 	childEntropyVector.push_back(childEntropyTemp);
	// 	childEntropy += childEntropyTemp;
	// 	cout << "accumulated child entropy " << childEntropy << endl;
	// 	sort(childEntropyVector.begin(), childEntropyVector.end());
	// 	float max = childEntropyVector[childEntropyVector.size()-1];
	// 	cout << "MAX entropy: " << max << endl;
	// 	if (childEntropyTemp == max) nextChild = attributeValuesCopy[attributeNames[0]][i];
	//
	//
	// 	// cout << "erasing" << attributeNames[i] << endl;
	// 	// attributeValuesCopy.erase(attributeNames[i]);
	// 	// for(auto attName : attributeValuesCopy) {
	// 	// 	for(auto vec : attName.second) {
	// 	// 	    cout << "key: " << attName.first << " -> ";
	// 	// 	    cout << vec << endl;
	// 	// 	}
	// 	// }
	// }
	//
	// cout << "GOING TO " << nextChild << endl;
	// cout << "***************************************************************************************" << endl;
	//
	//
	//
	// // cout << endl << endl << endl << "calculating next info gain, removing root " << root << endl;
	// vector<string>::iterator found = find(attributeNames.begin(), attributeNames.end(), root);
	// // cout << "found at " << *found << endl;
	// if (found != attributeNames.end())
	// 	attributeNames.erase(found);
	// attributeValues.erase(root);
	// // for(auto attName : attributeValues) {
	// // 	for(auto vec : attName.second) {
	// // 		cout << "key: " << attName.first << " -> ";
	// // 		cout << vec << endl;
	// // 	}
	// // }
	//
	//
	// // for (int i = 0; i < attributeNames.size()-1; i++)
	// // {
	// 	// cout << "in for                                         " << attributeNames[i] << endl;
	// 	// cout << "is this right " << attributeValues[attributeNames[i]][i] << endl;
	// 	//todo other ifs
	// 	if (root == "size")
	// 	{
	// 		cout << "in here att count " << attributeCount << endl;
	// 		for (int i = 0; i < attributeCount - 1; i++)
	// 		{
	// 			if (i == attributeCount - 2)
	// 			{
	// 				cout << "GOING TO " << attributeNames[i] << endl;
	// 				cout << "-----------------------------------------------------------------------------------" << endl;
	// 				break;
	// 			}
	// 			cout << "ATTRIBUTE: " << attributeNames[i] << endl;
	//
	// 			childData = childAttributeData[nextChild];
	// 			for (auto c : childData) cout << "child data " << c << endl;
	// 			string child = "";
	// 			//TODO change index to match new data spots when using her examples
	// 			// float childGainColor = CalculateInformationGain(childData, childAttributeCount, attributeValues[attributeNames[i]], 0);
	// 			// float childGainShape = CalculateInformationGain(childData, childAttributeCount, attributeValues[attributeNames[i]], 1);
	// 			float childGainColor = CalculateInformationGain(childData, childAttributeCount, attributeValues["color"], 0);
	// 			float childGainAct = CalculateInformationGain(childData, childAttributeCount, attributeValues["act"], 1);
	// 			float childGainAge = CalculateInformationGain(childData, childAttributeCount, attributeValues["age"], 2);
	//
	// 			cout << "child entropy " << childEntropy << endl;
	// 			// cout << "childGainColor " << childGainColor << endl;
	// 			// cout << "childGainShape " << childGainShape << endl;
	//
	// 			float childGainColorDiff = childEntropy - childGainColor;
	// 			float childGainAgeDiff = childEntropy - childGainAge;
	// 			float childGainActDiff = childEntropy - childGainAct;
	// 			cout << "childGainColorDiff " << childGainColorDiff << endl;
	// 			cout << "childGainAgeDiff " << childGainAgeDiff << endl;
	// 			cout << "childGainActDiff " << childGainActDiff << endl;
	//
	// 			vector<float> childVect = {childGainColorDiff, childGainAgeDiff, childGainActDiff};
	// 			sort(childVect.begin(), childVect.end());
	// 			cout << "MAX: " << childVect[childVect.size()-1] << endl;
	// 			if (childGainColorDiff == childVect[childVect.size()-1]) child = "color";
	// 			else if (childGainAgeDiff == childVect[childVect.size()-1]) child = "age";
	// 			else if (childGainActDiff == childVect[childVect.size()-1]) child = "act";
	//
	//
	// 			cout << "GOING TO " << child << endl;
	// 			cout << "-----------------------------------------------------------------------------------" << endl;
	//
	//
	// 		}
	// 	}
	// 	// else if (root == "color")
	// 	// {
	// 	// 	float childGainSize = CalculateInformationGain(childData, childAttributeCount, attributeValues["size"], 1);
	// 	//
	// 	// 	float gainSizeDiff = entropy - gainSize;
	// 	//
	// 	// }
	// 	// else if (root == "age")
	// 	// {
	// 	// 	;
	// 	// }
	// 	// else if (root == "act")
	// 	// {
	// 	// 	;
	// 	// }
	// // }

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
						if (nextWord.length() <= 0) continue;
						remove(nextWord.begin(), nextWord.end(), '{');
						remove(nextWord.begin(), nextWord.end(), '}');
						nextWord.erase(remove(nextWord.begin(), nextWord.end(), ','), nextWord.end());
						// cout << "        word:" << nextWord << endl;
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
	// cout << "categoryName" << categoryName << endl;
	attributeNames.erase(attributeNames.begin() + 4);
	attributeValues.erase(categoryName);
	GenerateDecisionTree(attributeCount, attributeValues, instances, attributeNames);

	infile.close();
	return 0;

}
