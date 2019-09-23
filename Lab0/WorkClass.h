#pragma once

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<map>
using namespace std;
using std::string;

class WorkClass
{
public:
	void readAndCountWords(const string in);
	void writeWords(const string out);
	WorkClass();
	~WorkClass();
private:
	int allWords;
	map<string, int> wordFrequency;
};

