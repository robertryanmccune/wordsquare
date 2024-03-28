/*
	Dictionary object header, Dict.hpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Reads in a wordlist and stores the words in a vector.
	Used in wordsquare generation when looking up words by their index in the matches matrix

	Dictionary may be a little misleading, wordlist would be more accurate, 
	but follows the convention laid out on Linux

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/


#ifndef DICT_HPP
#define DICT_HPP

class Dict {

	public:
		Dict();
		Dict(string);
		void read_dictfile(string);
		string get_word(int);
		int get_size();
	
	private:
		vector<string> dict;
		int size;
		string dictfile;
};

#endif
