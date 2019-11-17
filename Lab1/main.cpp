#include <iostream>
#include <conio.h>
#include "Tritset.h"

using namespace trit_set;

int main() {
	//first simpe test
    TritSet A(3);
    TritSet B(2);
	A[0] = True;
    A[2] = True;
	A[3] = False;
    B = A;
    B[2] = False;
    B[3] = True;
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
    
	std::cout << "t: " << std::endl;
    for(TritSet::reference t : B)
    {
        std::cout << t << std::endl;
    }
	std::cout << "A: " << std::endl;
    TritSet::TritIterator tritIterator = A.begin();
    while(tritIterator != A.end()) {
      std::cout << tritIterator << std::endl;
       ++tritIterator;
   }
   _getch();
    return 0;
}