#include <iostream>
#include "Date.h"


using namespace std;
using namespace::minirisk;

void test1()
{
}

void test2()
{
}

void test3()
{
}

int main()
{

	Date test;
	try {

		test.init(1889, 5, 7);
	}
	catch(exception e){
		cout<< "eception" << endl;
	}

	cout << "test serial number is "<< test.serial() << endl;
    test1();
    test2();
    test3();
    return 0;
}

