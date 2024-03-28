/*
	A WordSquare "Square" object implementation, Square.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	An object that represents a WordSquare, with functions
	that work with achieving completion

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include "wslib.hpp"

/* 
	Default Constructor, create a vector of 2*wordlen words
	and another vector that tracks whether an index to words is occupied
*/

Square::Square(){
	string init = "*****";
	for(int i=0; i<2*WORDLEN; i++) {
		words.push_back(init);
		assigned.push_back(0);
	}	
}

// set a given word to a given index
void Square::set_word(int index, string word) {
	words[index]=word;
}

// test if a given index position has been assigned a word
bool Square::empty_at(int index) {
	return !assigned[index];
}

// assign a given index a given word, mark as assigned
void Square::assign(string word, int index) {
	words[index] = word;
	assigned[index] = true;
}

// unassign a given index
void Square::unassign(int index) {
	string init = "*****";
	assigned[index] = false;
	words[index] = init;
}

/* 
	test if the wordsquare properties hold 
*/
bool Square::test_layout() {
	for(int i=0; i<2*WORDLEN; i++) {
		if( assigned[i] ) {		
			if(i<WORDLEN) {
				for(int j=0; j<WORDLEN; j++) {
					if( assigned[j+WORDLEN] ) {
						if( words[i][j] != words[j+WORDLEN][i] ) {
							return false;
						}
					}
				}
			} else {
				for(int j=0; j<WORDLEN; j++) {
					if( assigned[j] ) {
						if( words[i][j] != words[j][i-WORDLEN] ) {
							return false;
						}
					}
				}
			}	
		}
	}
	return true;
}

// return the index of the first unassigned word
int Square::get_next_index() {
	for(int i=0; i<2*WORDLEN; i++) {
		if( assigned[i]==false) {
			return i;
		}
	}
	return 2*WORDLEN;
}

/*
	return the regex of a given position
*/
string Square::get_constraint(int index) {
	string constraint(5,'*');
	if(index < WORDLEN ) {
		for(int i=0; i<WORDLEN; i++) {
			constraint[i] = words[i+WORDLEN][index];
		}
	} else {
		for(int i=0; i<WORDLEN; i++) {
			constraint[i] = words[i][index-WORDLEN];
		}
	}
	return constraint;
}

/*
	print just the square, half the words
*/
void Square::print_square() {
	
	for(int i=0; i<WORDLEN; i++) {
		for(int j=0; j<WORDLEN; j++) {
			cout << words[i][j] << " ";
		}
		cout << endl;
	}

	return;
}

/*
	print all the words, for completness
*/
void Square::print_words() {
	
	for(int i=0; i<2*WORDLEN; i++) {
		cout << i << ": " << words[i] << endl;
	}

	return;
}

/*
	write all the words to a given output stream
*/
void Square::write_words(ofstream& outstream) {

	for(int i=0; i<2*WORDLEN; i++) {
		outstream << i << ": " << words[i] << endl;
	}

}
