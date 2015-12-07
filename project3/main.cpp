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
	string dataSetName = "";

	ifstream infile;
	infile.open(string(argv[2]));



	if (infile.is_open())
	{
		while (getline(infile, line))
		{
		   istringstream iss(line);
		   vector<string> tokens;
		   for (string word; getline(iss, word, ' '); tokens.push_back(word))
		   {
			   cout << "word: " << word << endl;
		   }


			// string firstElem = string(line[0]);
			// if (line[0] == '%') continue;
			// if (line[0] == "@relation")
			// {
			// 	cout << "RELATION" << endl;
			// 	dataSetName = line[1];
			// }

			// else if (line[0] == "@attribute")
			// {
			// 	getline(infile, nextLine);
			// 	if (nextLine[0] == "@attribute")
			// 	{
			// 		cout << line << " " << line2 << "keep going" << endl;
			// 	}
			// }

			// std::istringstream iss(line);
		    // string first_on_line;
		    // if (iss >> first_on_line)
			// {
			// 	cout << "first: " << first_on_line << endl;
			// }
			//do some stuff

		}
	}
	// else
	// 	cout << "unable to open file";
	//

	infile.close();
	return 0;

}
