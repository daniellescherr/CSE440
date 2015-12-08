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

using std::cout; using std::cin; using std::endl;
using std::string; using std::vector; using std::ifstream;
using std::istringstream; using std::stringstream;

int main(int argc, char * argv[])
{
	if ((argc != 3) || (string(argv[1]) != "DT")) {
		std::cerr << "ERROR input should be in the format: DT file.data" << endl;
		exit(1);
	}

	string line;
	string dataSetName = "", attributeName = "";

	ifstream infile;
	infile.open(string(argv[2]));

	if (infile.is_open())
	{
		while (getline(infile, line))
		{
			// string nextLine = "", nextLineWord = "";
			istringstream iss(line);
			vector<string> tokens;
			cout << "LINE: " << line << endl;
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
					// getline(infile, nextLine);
					// cout << "    nextLineWord: " << nextLineWord << endl;
					// cout << "    next line: " << nextLine << endl;
					// if (nextWord == "@attribute")
					// {
					// 	cout << "    next is also attribute, keep going" << endl;
					// }
					// else
					// {
					// 	categoryName = nextWord;
					// 	cout << "    ELSE CATEGORY NAME: " << categoryName << endl;
					// }
					categoryName = word;
					cout << "    CATEGORY NAME: " << categoryName << endl;

				}
				// else if (word == "@data")
				// {
				// 	cout << "YES" << endl;
				// 	string dataLine = "";
				// 	cout << "    DATA: " << word << endl;
				// 	while (getline(infile, dataLine))
				// 	{
				// 		cout << "dataline: " << dataLine << endl;
				// 		cout << "size: " << dataLine.length() << endl;
				// 		istringstream iss(dataLine);
				// 		if (dataLine.length() > 0)
				// 			cout << "    data: " << dataLine << endl;
				// 		else
				// 			break;
				// 	}
				// }
			}
	   }
   }
	// else
	// 	cout << "unable to open file";
	//

	infile.close();
	return 0;

}
