/*
	A WordSquare object header, Square.hpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	WordSquare "Square" object, represents one of many WordSquares

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#ifndef SQUARE_HPP
#define SQUARE_HPP

class Square {

	public:
		Square();

		void set_word(int, string);
		bool empty_at(int);
		bool test_layout();

		void assign(string, int);
		void unassign(int);
		int get_next_index();
		string get_constraint(int);

		void print_square();
		void print_words();
		
		void write_words(ofstream&);

	private:
		vector<string> words;
		vector<int> assigned;
};

#endif
