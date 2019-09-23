#include "WorkClass.h"

WorkClass::WorkClass()
{
	allWords = 0;
}

void WorkClass::readAndCountWords(const string in)
{
	ifstream fin(in);
	string currLine = "";
	string currWord = "";
	while (!fin.eof())
	{
		getline(fin, currLine);
		for (int i = 0; i < (int)currLine.length(); i++)
		{
			while (isalnum(currLine[i]))
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
	fin.close();
}
void WorkClass::writeWords(const string out)
{
	vector<pair<string, int> > copyMap(wordFrequency.begin(), wordFrequency.end());
	sort(copyMap.begin(), copyMap.end(), [](const pair<string, int> &a, const pair<string, int> &b){return a.second > b.second;});
	ofstream fout(out);
	for (const auto &pair : copyMap) {
		fout << pair.first + " " + to_string(pair.second) + " " + to_string((double)pair.second /(double) allWords) + '\n';
	}
	fout.close();
}
WorkClass::~WorkClass()
{
}


