/*
	WordSquare Generator, main.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Given a few seedwords and a wordlist,
	generate complete wordsquares

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include "wslib.hpp"

using namespace std;

uint64 getTime();

int main(int argc, char* argv[]) {

	/* usage */
	if( argc != 6 ) {
		cout << "usage: ./squares  dict  regs  matches  seeds  outfile" << endl;
	}

	/* filenames as program input */
	string dictfile = argv[1];
	string regsfile = argv[2];
	string matchfile = argv[3];
	string seedfile = argv[4];
	string outfile = argv[5];

	uint64 start_total = getTime();

	/* load and assign wordlist and regex structures */
	cout << endl << "loading files..." << endl << endl;
	Dict dict(dictfile);
	Regs regs(regsfile);
	Matches matches(matchfile);
	Squares squares(seedfile);
	
	squares.set_dict(&dict);
	squares.set_regs(&regs);	
	squares.set_matches(&matches);
	squares.set_outfile_name(outfile);
	cout << "...all files loaded" << endl << endl;

	/* assign matches matrix dimensions */
	matches.set_numwords( dict.get_size() );
	matches.set_numregs( regs.get_size() );

	/* generate all possible seed square configurations */
	squares.generate_seedsquares();
	cout << "generated " << squares.get_numsquares() << " seedsquares" << endl;
	int sdsqrs = squares.get_numsquares();
	
	/* generate all possible wordsquares */
	uint64 start_ws_proc = getTime();
	squares.generate_wordsquares();
	int foundsquares = squares.get_numsquares() - sdsqrs;
	cout << "generated: " << foundsquares << " wordsquares" << endl;	
	
	/* write complete wordsquares to given output file */
	if(foundsquares==0) {
		cout << "no squares found" << endl;
	} else {
		cout << "writing wordsquares to: " << outfile << endl;
		squares.write_solved_squares();
		cout << "wordsquares written" << endl;
	}
	
	/* print runtime */	
	cout << "elapsed time calculating wordsqurare: " << (float)(getTime() - start_ws_proc)/1000 << " s" << endl;
	cout << "total elapsed time: " <<  (float)(getTime() - start_total)/1000 << " s" << endl;

	return 0;

}


/*
	get current time, for timing
*/
uint64 getTime()
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
