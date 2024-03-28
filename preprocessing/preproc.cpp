/*
	Dictionary (wordlist) preprocessing for wordsquares, preprocessing.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	This program computes 3 data structures utilized by the efficient processing
	of wordsquare generations.  These 3 data structures are:
		- A wordlist of all words equal to the width of the wordsquare
		- A list of all possible regexes that can represent a word in the wordlist
		- A matrix of size wordlist*number_of_regexes, 
		  where an entry in the matrix is 1 if a word can be represented by the regex
	
	For more detail on how the 3 data structures are used, and further elaboration
	on the merits of preprocessing, see the README
	
	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

typedef unsigned long long uint64;

/* 1-byte struct to allow for bit level access/storage */
/*struct Bits{
	bool bit0 : 1;
	bool bit1 : 1;
	bool bit2 : 1;
	bool bit3 : 1;
	bool bit4 : 1;
	bool bit5 : 1;
	bool bit6 : 1;
	bool bit7 : 1;
};*/

/* read in a wordlist */
void read_dict( set<string>&, string, int);
string sanitize(string);

/* create a vector that stores all possible regexes from the wordlist */
vector<set<int> > gencombos(int);
void gensets(set<int>, set<int>, set<int>, vector<set<int> >&);
void populate_regexes(vector<string>&, vector<set<int> >&, char*, int, int, int);
void populate_regset(set<string>&, char*, int, int, int);
vector<string> set2vec(set<string>&);

/* generate the matches matrix */
//void populate_matches( Bits*, vector<string>&, vector<string>&, int wordlen );
void populate_matches( vector<int>&, vector<int>&, vector<string>&, vector<string>&, int);
void write_matches_csc(vector<int>&, vector<int>&, string);


bool match(string, string, int);

void write_output(vector<int>&, vector<int>&, string );

/* time */
uint64 getTimeMs64();

int main(int argc, char* argv[]) {

	if(argc!=5) {
		cout << "usage: ./preproc  dict_infile  dict_outfile  reg_outfile  matches_outfile" << endl;
		return -1;
	}

	/* initialize file strings */
	string dictfile = argv[1];
	string dictout = argv[2];
	string regout = argv[3];
	string matchesout = argv[4];
	
	/* key variables */
	/* 
	  if you're going to change wordlen be aware the read_dict() function only
	  supports words smaller than wordlen when wordlen is 5.
	  Without changes, changing wordlen will only allow wordsquares 
	  with all words equal to the wordlen
	*/
	int wordlen = 5;
	int numwords, numcombos, numregs;

	uint64 total_start = getTimeMs64();

	/* 
		Load a dictionary/wordlist with 
		words only of length "wordlen" (default 5)
	*/	
	cout << "loading dictionary" << endl;
	set<string> dictset;
	vector<string> dict;
	read_dict( dictset, dictfile, wordlen );
	dict = set2vec(dictset);
	numwords = dict.size();	
	cout << "loaded " << numwords << " words" << endl << endl;

	// write all 5-letter words to a new wordlist
	cout << "writing 5-letter word file" << endl;
	uint64 start = getTimeMs64();
	ofstream outstream;
	outstream.open( dictout.c_str() );
	outstream << numwords << endl;
	for(int i=0; i<numwords; i++) {
		outstream << dict[i] << endl;
	}
	outstream.close();
	cout << "wrote 5-letter word file in: " << (float)(getTimeMs64() - start)/1000 << " s" << endl << endl;

	// Generate all possible combinations of indices from a word of length: wordlen
	cout << "generating all regular expression combos from a word of length: " << wordlen << endl;
	vector<set<int> > combos = gencombos(wordlen);
	numcombos = combos.size();
	cout << "generated " << numcombos << " combinations" << endl << endl;
	

	/* 
		populate array of regexes formed from words in dictionary, duplicates included.
		use the collection of sets generated from gencombos() 
	*/
	cout << "creating and populating array with all possible regular expressions from dictionary: " << dictfile << endl;
	char* regexes;
	regexes = new char[numcombos*wordlen*numwords];
	for(int i=0; i<numcombos*wordlen*numwords; i++) regexes[i] = '*';
	populate_regexes(dict, combos, regexes, numwords, numcombos, wordlen);
	cout << "populated regex array with: " << numcombos * numwords << " entries,";
	cout << "with " << numcombos*numwords*wordlen << " total characters" << endl << endl;


	/*
		Create and populate a set with all regular expressions to eliminate duplicates
		Then convert the set to a vector for easier access
	*/	
	cout << "populating collection of regular expressions" << endl;	
	set<string> regset;
	vector<string> regs;
	populate_regset( regset, regexes, numwords, numcombos, wordlen);
	regs = set2vec(regset);
	numregs = regs.size();
	delete [] regexes; //save mem, delete now
	cout << "regex vector populated with " << numregs << " expressions" << endl << endl;

	/* write all the regexes to a file */
	cout << "writing regex file" << endl;
	start = getTimeMs64();
	outstream.open( regout.c_str() );
	outstream << numregs<< endl;
	for(int i=0; i<numregs; i++) {
		outstream << regs[i] << endl;
	}
	outstream.close();
	cout << "wrote regex word file in: " << (float)(getTimeMs64() - start)/1000 << " s" << endl << endl;


	/*
		Create and initialize matrix for word/regex matches
		the matches array is a 2d matrix with words as rows and regexes as columns
		the 2d matrix "matches" is stored as a single 1d array of length numwords*numregs.
		To conserve space, entries in the matrix as represented by bits.
		If a given entry is 1, then it means a given word can be represented by the regex,
		e.g.  the entry at (word:route, regex: ro***) would be 1	
		Initialize the matrix with zeroes.	
	*/		
	//cout << "initializing matrix array" << endl;
	//Bits* matches;
	
	/*unsigned long numentries = (long)numwords*numregs;
	unsigned long matchsize;
	matchsize = numentries/8;	
	matches = new Bits[ matchsize ];
	for(long i=0; i<matchsize; i++) {
		matches[i].bit0=0;
		matches[i].bit1=0;
		matches[i].bit2=0;
		matches[i].bit3=0;
		matches[i].bit4=0;
		matches[i].bit5=0;
		matches[i].bit6=0;
		matches[i].bit7=0;
	}*/
	long num_entries = numwords*numcombos; //  27068*31
	vector<int> csc1(numregs+1);
	vector<int> csc2(num_entries);
	
	cout << "initialized csc matrix" << endl;
	//cout << "intiailzed: " << numentries << " in array of size: " << matchsize << endl << endl; 


	// Populate matches array by finding word/regex matches
	cout << "populating array by finding all word/regex matches" << endl;
	start = getTimeMs64();
	//populate_matches( matches, dict, regs, wordlen );
	populate_matches( csc1, csc2, dict, regs, wordlen);
	cout << "matches array populated in " << (float)(getTimeMs64() - start)/1000 << " s" << endl << endl;

	/* 
		write the matches matrix to a file
		first line is the size of the matches matrix in bytes
		the rest is the binary representation of the matrix
	*/
	cout << "writing matches file" << endl;
	start = getTimeMs64();
	write_matches_csc(csc1, csc2, matchesout);
	//outstream.open(matchesout.c_str());
	//outstream << matchsize << endl;
	//outstream.write(  reinterpret_cast< const char* >(matches), matchsize*sizeof(Bits)  );
	//outstream.close();
	cout << "matches csc file written in " << (float)(getTimeMs64() - start)/1000 << " s" << endl << endl;

	cout << "preprocessing complete" << endl;
	cout << "total elapsed time: " << (float)(getTimeMs64()-total_start)/1000 <<  " s" << endl;

	//delete [] matches;
	

	return 0;

}

// read only 5 letter words into dictionary/wordlist
void read_dict( set<string>& dict, string dictstr, int wordlen) {

	string line;
	int len;
	ifstream instream;
	instream.open( dictstr.c_str() );
	
	while( getline(instream, line) ) {
		line = sanitize(line);
		len = line.size();
		if ( len == wordlen ) {
			dict.insert(line);
		} 
		/* hardcoded below to allow for 3 and 4 letter words in 5x5 wordsquares */
		else if( wordlen==5 && len == 4) {
			string str1, str2;
			str1 = "-"+line;
			str2 = line+"-";
			dict.insert(str1);
			dict.insert(str2);
		} else if ( wordlen==5 && len == 3) {
			string str1, str2, str3;
			str1 = "--"+line;
			str2 = "-"+line+"-";
			str3 = line + "--";
			dict.insert(str1);
			dict.insert(str2);
			dict.insert(str3);
		}
	}

	return;
}


// all lower case, letters only
string sanitize(string line) {

	string line2 = "";
	//int counter=0;
	char c;
	for(int i=0; i<line.size(); i++) {
		c = line[i];
		if( isalpha(c) ) {
			if( isupper(c) ) {
				line[i] = tolower(c);
			}
			line2.append(line,i,1);
		}
	}

	return line2;
}

/*
	Initialize a set P with all indices for a word of given length,
	e.g. for words of length 5, create a set P with {1,2,3,4,5},
	then pass the set to gensets() to generate all possible subsets with those indices
*/
vector<set<int> > gencombos(int wordlen) {

	vector<set<int> > combos;
	set<int> R, P, X;
	for(int i=0; i<wordlen; i++) {
		P.insert(i);
	}
	gensets( R, P, X, combos);

	return combos;
}

/*
	Generate all possible subsets from a given input set,
	e.g. for a set with {1,2,3,4,5}, compute all possible subsets,
	like {1}, {1,2}, {1, 4}, {2,3}, etc.
*/
void gensets(set<int> R, set<int> P, set<int> X, vector<set<int> > &combos) {

	if( P.empty() )	return;

	int x;
	set<int> P2, X2;
	set<int>::iterator itr;
	for(itr=P.begin(); itr!=P.end(); ++itr) {
		x = *itr;
		if( X.find(x) == X.end() ) {
			R.insert(x);
			combos.push_back(R);
			P2 = P;
			P2.erase(x);
			X.insert(x);
			gensets(R, P2, X, combos);
			R.erase(x);
		}
	}
	return;
}

/*
	populate the character array called regexes with all possible regex combinations from the wordlist.
	e.g. for a word like "route", generate "r****", "ro***", "**u**", "ro*te," etc.  
	do this for every word, and store the result in the long regexes array
	use the combos array that holds all possible combinations of indices of words of length wordlen.
	duplicates are possible
	
	nw = number of words
	nc = number of combos
	wl = length of words
*/
void populate_regexes(vector<string>& dict, vector<set<int> > &combos, char* regexes, int nw, int nc, int wl) {

	int pos, elem;
	string word;
	set<int> S;
	set<int>::iterator itr;
	for(int i=0; i<nw; i++) {
		word = dict[i];
		for(int j=0; j<nc; j++) {
			S=combos[j];
			itr=S.begin();
			pos=0;
			while( itr != S.end() ) {
				elem = *itr;
				if( elem == pos ) {				
					regexes[(i*nc*wl)+(j*wl)+pos] = word[elem];					
					pos++;
					itr++;
				} else {
					pos++;
				} 
			}
		}
	}

	return;
}

/*
	Using the regex array created in populate_regexes()
	remove all duplicates by adding each regex to a set
*/
void populate_regset(set<string>& rhash, char* regexes, int nw, int nc, int wl) {

	string word(wl, '*');
	for(int i=0; i<nw; i++) {
		for(int j=0; j<nc; j++) {
			for(int k=0; k<wl; k++) {
				word[k] = regexes[(i*nc*wl) + (j*wl) + k];
			}
			if( rhash.find(word) == rhash.end() ) {
				rhash.insert(word);
			}
		}
	}

	return;
}

/*
	Using the set created by populate_regset(), 
	convert it to a vector for easier access
*/
vector<string> set2vec(set<string>& regset) {
	vector<string> regvec;
	set<string>::iterator itr;
	for(itr = regset.begin(); itr != regset.end(); ++itr) {
		regvec.push_back( *itr );
	}
	regset.clear();
	return regvec;
}

/*
	Populate the matches matrix
	Iterate through every word in the dictionary wordlist
	Then for each word, iterate through every possible regex.
	If the given word fits the given regex (e.g. "route" fits "r**t*"),
	then set the corresponding bit in matches to 1
*/
//void populate_matches(Bits* matches, vector<string>& dict, vector<string>& regs, int wordlen ) {
void populate_matches(vector<int>& csc1, vector<int>& csc2, vector<string>& dict, vector<string>& regs, int wordlen ) {

	int numwords = dict.size();
	int numregs = regs.size();

	string word, reg;
	//long index, rem;
	
	/*
	for(long i=0; i<numwords; i++) {
		if(i%1000==0) cout << "matching " << i << "th word" << endl;
		word=dict[i];
		for(long j=0; j<numregs; j++) {
			reg=regs[j];
			if( match(word, reg, wordlen) ) {
				index = i*numregs + j;
				rem = index%8;
				index /= 8;

				switch(rem) {
					case 0: matches[index].bit0=1; break;
					case 1: matches[index].bit1=1; break;
					case 2: matches[index].bit2=1; break;
					case 3: matches[index].bit3=1; break;
					case 4: matches[index].bit4=1; break;
					case 5: matches[index].bit5=1; break;
					case 6: matches[index].bit6=1; break;
					case 7: matches[index].bit7=1; break;
				}
			}
		}
	}*/
	csc1[0]=0;
	int count=0;
	for(int i=0; i<numregs; i++) {
		if(i%10000==0) cout << "matching " << i << "th regex" << endl;
		reg=regs[i];
		for(int j=0; j<numwords; j++) {
			word=dict[j];
			if( match(word, reg, wordlen) ) {
				csc2[count]=j;
				count++;
			}
		}
		csc1[i+1]=count;
	}

	return;
}

/* 
	Test if a given word matches the given regex
	return true if so, else false
 */
bool match(string word, string regex, int wordlen) {

	for(int i=0; i<wordlen; i++) {
		if( regex[i] != '*' && word[i] != regex[i] ) {
			return false;
		}
	}

	return true;
}

/* write the compressed spares column data to a file */
void write_matches_csc(vector<int>& csc1, vector<int>& csc2, string outfile) {
	int c1_size = csc1.size();
	int c2_size = csc2.size();
	
	ofstream outstream;
	outstream.open( outfile );
	
	outstream << c1_size << endl;
	for(int i=0; i<c1_size; i++) {
		outstream << csc1[i] << endl;
	}
	
	outstream << c2_size << endl;
	for(int i=0; i<c2_size; i++) {
		outstream << csc2[i] << endl;
	}
	
	outstream.close();
}

/*
	get current time, for timing
*/
uint64 getTimeMs64()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  uint64 ret = tv.tv_usec;
  // convert micro secs (10^-6) to millisecs (10^-3)
  ret /= 1000;
  
  // add seconds (10^0) after converting to millisecs (10^-3)
  ret += (tv.tv_sec*1000);
  return ret;
}

void write_output(vector<int>& csc1, vector<int>& csc2, string filename) {


}
