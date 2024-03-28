/*
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>
#include "sys/time.h"

using namespace std;

typedef unsigned long long uint64;

#define WORDLEN 5

/*
struct Bits{
	bool bit0 : 1;
	bool bit1 : 1;
	bool bit2 : 1;
	bool bit3 : 1;
	bool bit4 : 1;
	bool bit5 : 1;
	bool bit6 : 1;
	bool bit7 : 1;
};
*/

#include "Dict.hpp"
#include "Regs.hpp"
#include "Matches.hpp"
#include "Square.hpp"
#include "Squares.hpp"