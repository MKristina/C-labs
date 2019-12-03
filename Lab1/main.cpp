


#include <iostream>
#include <conio.h>
#include "Tritset.h"

using namespace trit_set;

int main() {
	//first simpe test
	TritSet A(5);
	TritSet B(5);
	A[2] = True;
	A[3] = False;
	
	Trit ttttt = A[3];
	std::cout << "ttt " << ttttt << std::endl;
	B = A;
	
	B[2] = False;
	B[3] = True;
	A[3] = B[3];
	A[100] = A[2];
	std::cout << "A: " << A << std::endl;
	std::cout << "B: " << B << std::endl;
	std::cout << "A length: " << A.length() << std::endl;
	std::cout << "Number of  'True' in A: " << A.cardinality(True) << std::endl;
	std::cout << "~A: " << ~A << std::endl;
	std::cout << "A | B: " << (A | B) << std::endl;
	std::cout << "A & B: " << (A & B) << std::endl;
	A.trim(2);
	std::cout << "A trim: " << A << std::endl;
	B.shrink();
	std::cout << "B shrink: " << B << std::endl;
	
	_getch();
	return 0;
}