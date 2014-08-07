#pragma once


#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include <cctype>
#include <cassert>
#include "FileNode.h"
using namespace std;

class RjbReader {
private: 
	fstream *fin;
	static const int MAX_LEN = 5000000;
	char *line;

	vector<char> buffer;
	int bufferPtr;

	void skip() {
		while(bufferPtr < (int)buffer.size() && isspace(buffer[bufferPtr])) bufferPtr++;
	}

	/*
	void skip(int exception) {
		while(bufferPtr < (int)buffer.size() & buffer[bufferPtr] != exception && isspace(buffer[bufferPtr])) bufferPtr++;
	}
	*/

	char peek() {
		for (int i=bufferPtr; i<(int)buffer.size(); i++)
			if (!isspace(buffer[i])) return buffer[i];
		return -1;
	}

	char get() { skip(); return buffer[bufferPtr++]; }

	// assume next is a string
	string nextString() {
		skip();
		int ptr = 1; line[0] = '\"'; bufferPtr++;
		do {
			line[ptr++] = buffer[bufferPtr++];
		} while(buffer[bufferPtr - 1] !=	'\"');

		line[ptr] = '\0';
		return line;
	}

	bool isSymbol(char ch) {
		return ch == ':' || ch == ',' || ch == '{' || ch == '}';
	}

	string next() {
		skip();
		int ptr = 0; line[0] = '\0';
		for (; bufferPtr < (int)buffer.size(); bufferPtr++) {
			char ch = buffer[bufferPtr];
			if (isspace(ch) || isSymbol(ch)) break;
			line[ptr++] = buffer[bufferPtr];
		}
		line[ptr] = '\0';
		return line;
	}

	string nextPrimitive() {
		skip();
		if (buffer[bufferPtr] == '\"') return nextString();
		else return next();
	}

	FileNode* read() {
		skip();
		FileNode *root = NULL;

		if (peek() != -1) {
			root = new FileNode();
			string pName = next();
			if (pName.back() == ':') {
				bufferPtr--;
				pName.pop_back();
			}
			root->_property_name = pName;

			assert(peek() == ':');
			get(); // eat the ':'

			if (peek() != '{') { // have some primitive_data
				// read one primitive first
				root->_primitive_data.push_back(nextPrimitive());
				while(peek() == ',') {
					get(); // eat that ','
					root->_primitive_data.push_back(nextPrimitive());
				}
			}

			if (peek() == '{') { // have children
				get(); // eat that '{'
				while(peek() != '}') root->_sub_property.push_back(read());
				get(); // eat the '}'
			}
		}

		return root;
	}

public:
	RjbReader(const char* filename) {
		fin = new fstream(filename, ios::in);

		bufferPtr = 0;
		line = new char[MAX_LEN + 1];
		while(fin->good()) {
			fin->getline(line, MAX_LEN);

			char *pos = strstr(line, ";");
			if (pos) *pos = '\0';

			for (int i=0; line[i]; i++) 
				buffer.push_back(line[i]);
				buffer.push_back('\n');
		}
	}

	FileNode* readFile() {
		FileNode *root = new FileNode(), *tmp;
		while(tmp = read(), tmp != NULL) root->_sub_property.push_back(tmp);
		return root;
	}
};
