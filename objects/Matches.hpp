/*
	A Matches object header, Matches.hpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>
	
	A 2D numwords*numregexes Matrix, represented by a 1D array

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#ifndef MATCHES_HPP
#define MATCHES_HPP

class Matches{

	public:
		Matches();
		Matches(string);
		~Matches();

		void read_matches();

		unsigned long get_numwords();
		void set_numwords(unsigned long);

		unsigned long get_numregs();
		void set_numregs(unsigned long);

		vector<int> get_matches(int);

	private:
		
		//Bits* bits;
		//int* csr1;
		//int* csr2;
		vector<int> csc1;
		vector<int> csc2;		
		//unsigned long size;
		string matchfile;
		int numwords;
		int numregs;

};

#endif
