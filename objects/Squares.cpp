/*
	A collection of WordSquare "Squares" object implementation
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Implements the core logic for generating WordSquares.
		- Reads in input
		- Reads in auxiliary data structures
		- Generate seed squares from seed words
		- Generates solution squares from seed squares and wordlists
		- Writes results to output file

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include "wslib.hpp"

// Default Constructor
Squares::Squares() {}

// Constructor with an input seedfile
Squares::Squares(string str) {
	seedfile = str;
	read_seedfile();
}

/*
	read in a file that contains the seed words
	formatted with first line number of seed words,
	followed by each specific seed word.
	Read seed words into seed words vector.
*/
void Squares::read_seedfile() {

	cout << "reading seedfile: " << seedfile << endl;
	ifstream instream;
	instream.open(seedfile.c_str());
	seedsize=0;
	
	string line;
	
	while( getline(instream, line)) {
		seedsize++;
		if(seedsize>10) {
			cout << "ERROR: MORE THAN 10 SEEDWORDS"<<endl;
			cout << "exiting program" << endl;
			exit(-1);
		} 
		if(line.size() > WORDLEN) {
			cout << "ERROR: seedword " << line << " exceeds wordlength " << WORDLEN << endl;
			cout << "exiting program" << endl;
			exit(-1);
		} else if(line.size() < WORDLEN) {
			cout << "ERROR: seedword " << line << " less than wordlength " << WORDLEN << endl;
			cout << "try adding '-' characters for empty spaces" << endl;
			cout << "exiting program" << endl;
			exit(-1);
		}
		seedwords.push_back(line);
	}	
	if(seedsize<3) {
		cout << "ERROR: LESS THAN 3 SEEDWORDS"<<endl;
		cout << "exiting program" << endl;
		exit(-1);
	}
	
	cout << "loaded " << seedsize << " seedwords" << endl << endl;

	instream.close();

	return;
}

/*
	Public-facing generate all seedsquares function.
	Generate all possible layouts with the provided seed words.
	Initialize an empty square and pass it to the private gen_ss method.
	Number of words incorporated in the seedsquare is 0
*/
void Squares::generate_seedsquares() {

	Square seedsquare, *p_sqr = &seedsquare;
	
	gen_ss(p_sqr, 0);
	
	num_seedsquares=squares.size();
	
	return;
}

/*
	Public generate all wordsquare method
	For every seedsquare, call generate wordsquare
*/
void Squares::generate_wordsquares() {

	int numseeds = squares.size();
	Square sqr;
	for(int i=0; i<numseeds; i++) {
		sqr = squares[i];
		gen_ws(&sqr);
	}
	return;
}

/*
	Generate all possible seedsquares using the seed words.
	
	If all seedwords are in the square, push it to the vector of squares.
	Otherwise, get the next seedword.  
	Try placing the seedword in every available word position.
	Mark the position as assigned.
	If the position fits, recurse to the next seedword, then unassign
*/
void Squares::gen_ss(Square* sqr, int count) {
	
	if( count == seedsize ) {
		squares.push_back( *sqr );
		return;
	}

	string word = seedwords[count];

	for(int i=0; i<2*WORDLEN; i++) {
		if( count==0 && i>= WORDLEN ) break; // skip diagonal reflections
		if( sqr->empty_at(i) ) {
			sqr->assign(word, i);
			if( sqr->test_layout() ) {
				gen_ss( sqr, count+1 );
			}
			sqr->unassign(i);
		}
	}

	return;
}

/*
	Generate all possible wordsquares from the seedwords and the wordlist.
	
	First get the index of where the next word goes.  
	If it's equal to 2*WORDLEN, then push the square onto the solution square vector
	
	Otherwise, get the regex constraint on the given index.
	Use the regex to find it's index in the regex wordlist,
	then use the regex index to get the row numbers of all words that match the regex.
	Iterate over each row number, and use the dict to find the actual word.
	
	Assign each word to the square, then recurse, followed by an unassign
	
*/
void Squares::gen_ws(Square* p_sqr) {

	int index = p_sqr->get_next_index();

	if( index==2*WORDLEN) {
		squares.push_back(*p_sqr);
		return;
	}

	string reg = p_sqr->get_constraint(index);
	int regindex = regs->get_index(reg);
	if( regindex == -1 ) return;
	vector<int> regmatches = matches->get_matches(regindex);
	
	for(unsigned i=0; i<regmatches.size(); i++) {
		p_sqr->assign( dict->get_word(regmatches[i]), index );
		gen_ws(p_sqr);
		p_sqr->unassign(index);
	}
	
	return;
}

// return the number of squares in the squares vector
int Squares::get_numsquares() {
	return squares.size();
}

// set the wordlist object
void Squares::set_dict(Dict *d) {
	dict = d;
}

// set the precomputed regs object
void Squares::set_regs(Regs *r) {
	regs=r;
}

// set the precomputed match matrix
void Squares::set_matches(Matches *m) {
	matches=m;
}

// print all the seedwords
void Squares::print_seedwords() {
	cout << "printing seedwords..."<<endl;
	for(int i=0; i<seedsize; i++) {
		cout << i << ": " << seedwords[i] << endl;
	} cout << endl;
}

/*
	print all squares.
	Since completed squares are written to the end of a vector
	that also contains seedsquares, both seedsquares and complete squares
	will be printed, depending on when the method is called.
	not currently called in the program, but here for completeness.
*/
void Squares::print_squares() {
	cout << "printing squares..." << endl;
	for(unsigned i=0; i<squares.size(); i++) {
		cout << i << endl;
		(squares[i]).print_words();
		cout << endl;
	}
}

// set the output file name
void Squares::set_outfile_name(string outfilename)  {
	outfile=outfilename;
}

/*
	Write the completed WordSquares to the provided output file
*/
void Squares::write_solved_squares() {

	int num_solved_squares = squares.size()-num_seedsquares;
	
	ofstream outstream;
	outstream.open( outfile.c_str() );
	
	outstream << "found " << num_solved_squares << " wordsquares" << endl << endl;
	
	for(int i=num_seedsquares; i<squares.size(); i++) {
		outstream << i-num_seedsquares+1 << ": " << endl << endl;
		Square sqr = squares[i];
		sqr.write_words(outstream);
		if(i!=squares.size()-1) outstream << endl << endl;
	}
	
	outstream.close();

}

