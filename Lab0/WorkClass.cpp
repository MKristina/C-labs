#include "WorkClass.h"

WorkClass::WorkClass(string in, string out)
{
	inputFile = in;
	outputFile = out;
	allWords = 0;
}

void WorkClass::readAndCountWords() 
{
	ifstream fin(inputFile);
	string currLine = "";
	string currWord = "";
	while (!fin.eof())
	{
		getline(fin, currLine);
		for (int i = 0; i < (int)currLine.length(); i++)
		{
			while ((currLine[i] >= 'a' && currLine[i] <= 'z') || (currLine[i] >= 'A' && currLine[i] <= 'Z') || isdigit(currLine[i]))
			{
				currWord += currLine[i];
				i++;
			}
			if (!currWord.empty()) {
				wordFrequency[currWord]++;
				allWords++;
				currWord = "";
			}
		}
	}
	
}
void WorkClass::writeWords() 
{
	vector<pair<string, int> > copyMap(wordFrequency.size());
	copy(wordFrequency.begin(), wordFrequency.end(), copyMap.begin());
	sort(copyMap.begin(), copyMap.end(), [](const pair<string, int> &a, const pair<string, int> &b){return a.second > b.second;});
	ofstream fout(outputFile);
	for (const auto &pair : copyMap) {
		fout << pair.first + " " + to_string(pair.second) + " " + to_string((double)pair.second /(double) allWords) + '\n';
	}
}
WorkClass::~WorkClass()
{
}


