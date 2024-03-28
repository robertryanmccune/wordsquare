/*
	Regex Object implementation, Regs.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Like a wordlist except the words are regular expressions
	with asterisks as a wildcard for a single letter
	Stores the list of regexs, as well as a map that maps
	a given regex to its index in the list

	This object is created from a file that stores
	all possible regexes that represent words in the wordlist.
	This regex  file is generated through preprocessing.

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include "wslib.hpp"

// Default Constructor
Regs::Regs(){}

// Initialize a Regs object with a 
Regs::Regs(string str) {
	regsfile = str;
	read_regsfile(regsfile);

}

/*
	Read in the preprocessed Regs file
	and create the list of regexes.
	Also create a map of regexes to their index
*/
void Regs::read_regsfile(string str) {
	cout << "loading regsfile: " << str << endl;
	ifstream instream;
	instream.open(str.c_str() );

	string header;
	getline( instream, header );
	size = atoi( header.c_str() );
	cout << "loading " << size << " regs" << endl;

	string line;
	for(int i=0; i<size; i++) {
		getline( instream, line );
		regs.push_back(line);
	}
	instream.close();
	cout << "regular expressions loaded" << endl;
	cout << "creating reg-to-index lookup map" << endl;
	string reg;
	for(int i=0; i<size; i++) {
		reg=regs[i];
		reg2index.insert( make_pair(reg,i) );
	}
	cout << "reg-to-index map created of size: " << reg2index.size() << endl;
}

/*
	Given a regex, return its index in the regex list
	Use the map reg2index
*/
int Regs::get_index(string reg) {

	if( reg2index.find(reg) != reg2index.end() ) {
		return reg2index[reg];
	} else {
		return -1;
	}
}

//  return the size of the regs list
int Regs::get_size() {
	return size;
}
