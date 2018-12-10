#include "Album.h"



Album::Album()
{
}


Album::~Album()
{
	cout << "a new album object was made without args" << endl;
}


Album::Album(string name) {
	this->name = name;

	cout << "a new album object was made with args" << endl;
}