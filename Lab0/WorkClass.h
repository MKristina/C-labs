#pragma once

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<map>

using namespace std;
using std::pair;
using std::string;

class WorkClass
{
public:
	string inputFile;
	string outputFile;
	void readAndCountWords();
	void writeWords();
	WorkClass(string in, string out);
	~WorkClass();
private:
	int allWords;
	map<string, int> wordFrequency;
};

