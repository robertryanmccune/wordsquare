/*
	Dictionary object implementation, Dict.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Stores words from a wordlist in a vector
	
	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include "wslib.hpp"

/* Default Constructor */
Dict::Dict(){}

/* Initialize object with the wordlist filename */
Dict::Dict(string str) {
	dictfile = str;
	read_dictfile(dictfile);

}

/*
	Read in a given wordlist and store it in a vector.
	first line of the wordlist lists the number of entries,
	followed by one word per line
*/
void Dict::read_dictfile(string str) {
	cout << "loading dictionary: " << str << endl;
	ifstream instream;
	instream.open(str.c_str() );

	string header;
	getline( instream, header );
	size = atoi( header.c_str() );
	cout << "loading " << size << " words" << endl;

	string line;
	for(int i=0; i<size; i++) {
		getline( instream, line );
		dict.push_back(line);
	}
	instream.close();
	cout << "dictionary loaded" << endl << endl;
}

/* 
	return a word at a given index
	used in wordsquare generation when finding the index of a word
	that matches a given regex		
*/
string Dict::get_word(int index) {
	return dict[index];
}

/* return number of words in wordlist */
int Dict::get_size() {
	return size;
}
