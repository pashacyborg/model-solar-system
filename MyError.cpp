#include <iostream>
#include "MyError.h"
#include <windows.h>

using namespace std;

void MyError::print()
{
	cerr << "Не могут значения быть отрицательными или быть равными 0!";
}