#include "WorkClass.h"
#include "iostream"
int main(int argc, char **argv) {
	if (argc != 3)
	{
		std::cout << "bad input";
	}
	else 
	{
		WorkClass workObject;
		workObject.readAndCountWords(argv[1]);
		workObject.writeWords(argv[2]);
	}
	return 0;
}