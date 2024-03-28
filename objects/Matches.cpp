/*
	Matches matrix object implementation, Matches.cpp 
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	A matrix object instantiated through preprocessing that 
	enables efficient retrieval of words that match a given regex		

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include "wslib.hpp"

// Default constructor
Matches::Matches(){}

// Initialize a Matches object with its filename
Matches::Matches(string str) {
	matchfile = str;
	read_matches();
}

/* 
	Default Destructor
	Make sure to deallocate array
*/
Matches::~Matches() {
	//delete [] bits;
}

/*
	Read in the binary representation of the precomputed matches matrix.
	First read in the top line to get the size of the array, then initialize.
	Then load the rest of the data into the array
*/
void Matches::read_matches() {
	cout << "reading matchfile: " << matchfile << endl;
	/*ifstream instream;
	instream.open(matchfile.c_str() );
	string header;
	getline( instream, header);
	size = atol(header.c_str());
	cout << "loading " << size << " bytes of matches" << endl;

	bits = new Bits[size];
	instream.read( reinterpret_cast< char* >(bits), size*sizeof(Bits) );
	*/
	ifstream instream;
	instream.open(matchfile);
	
	string line;
	getline(instream, line);
	int size1 = atoi( line.c_str() );
	csc1=vector<int>(size1);
	
	int val;
	for(int i=0; i<size1; i++) {
		getline(instream, line);
		val=atoi(line.c_str() );
		csc1[i]=val;
	}
	
	getline(instream, line);
	int size2=atoi(line.c_str());
	csc2=vector<int>(size2);
	
	for(int i=0; i<size2; i++) {
		getline( instream, line);
		val=atoi(line.c_str());
		csc2[i]=val;
	}
	
	cout << "matches loaded" << endl << endl;

	return;
}

/*
	Find the rows of all words that matches a regular expression at index regindex,
	and return them in a vector.
	Iterate through the column regindex and put all rows with 1 entries into a vector.
	This will eventually lead to a vector of words that match the regex
*/
vector<int> Matches::get_matches(int regindex) {

	vector<int> colmatches;
	/*unsigned long index, rem;
	Bits bit;
	for(unsigned long i=0; i<(unsigned long)numwords; i++) {
		index = (unsigned long)regindex+(unsigned long)(i*numregs);
		rem = index%8;
		index/=8;
		bit=bits[index];
		if( rem==0 && bit.bit0==1 ) colmatches.push_back(i);
		else if( rem==1 && bit.bit1 == 1) colmatches.push_back(i);
		else if( rem==2 && bit.bit2 == 1) colmatches.push_back(i);
		else if( rem==3 && bit.bit3 == 1) colmatches.push_back(i);
		else if( rem==4 && bit.bit4 == 1) colmatches.push_back(i);
		else if( rem==5 && bit.bit5 == 1) colmatches.push_back(i);
		else if( rem==6 && bit.bit6 == 1) colmatches.push_back(i);
		else if( rem==7 && bit.bit7 == 1) colmatches.push_back(i);
	}*/
	int start = csc1[regindex];
	int end = csc1[regindex+1];
	int num_matches=end-start;
	for(int i=0; i<num_matches; i++) {
		colmatches.push_back(csc2[start+i]);
	}
		
	return colmatches;
	

}

// get numwords method
unsigned long Matches::get_numwords() {
	return  numwords;
}

// set numwords
void Matches::set_numwords(unsigned long nw) {
	cout << "setting numwords to " << nw << endl;
	numwords = nw;
}

// get num_regs
unsigned long Matches::get_numregs() {
	return numregs;
}

// set num regs
void Matches::set_numregs(unsigned long nr) {
	cout << "setting numregs to " << nr << endl;
	numregs = nr;
}
