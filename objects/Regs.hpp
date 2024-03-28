/*
	Regex object header, regs.hpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Like a wordlist except the words are regular expressions
	with asterisks as a wildcard for a single letter
	
	Stores the list of regexs, as well as a map that maps
	a given regex to its index in the list

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#ifndef REGS_HPP
#define REGS_HPP

class Regs {

	public:
		Regs();
		Regs(string);
		void read_regsfile(string);
		int get_index(string);
		int get_size();
	
	private:
		vector<string> regs;
		map<string,int> reg2index;
		int size;
		string regsfile;
};

#endif
