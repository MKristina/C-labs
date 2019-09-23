#include "WorkClass.h"
#include "iostream"
int main(int argc, char **argv) {
	if (argc != 3)
	{
		std::cout << "bad input";
	}
	else 
	{
		WorkClass workObject(argv[1], argv[2]);
		workObject.readAndCountWords();
		workObject.writeWords();
	}
	return 0;
}