/*
	A collection of WordSquare "Squares" object header
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Where the heavy lifting is done for computing WordSquares

	Contains a vector of squares where WordSquare solutions are written.
	Also contains pointers to preprocessed Dict, Regs, and Matches objects

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#ifndef SQUARES_HPP
#define SQUARES_HPP

class Squares{

	public:
		// initializers
		Squares();
		Squares(string);
		void read_seedfile();

		// public generator methods
		void generate_seedsquares();
		void generate_wordsquares();

		// set methods
		void set_dict(Dict*);
		void set_regs(Regs*);
		void set_matches(Matches*);

		// get and print methods
		int get_numsquares();
		void print_seedwords();
		void print_squares();
		
		// writing methods
		void set_outfile_name(string);
		void write_solved_squares();


	private:
		// private generator methods 
		void gen_ss(Square*, int);
		void gen_ws(Square*);

		// private wordsquare objects
		vector<Square> squares;
		string seedfile;
		vector<string> seedwords;
		int seedsize;

		int num_seedsquares;

		// preprocessed components
		Dict *dict;
		Regs *regs;
		Matches *matches;
		
		string outfile;

};
#endif
