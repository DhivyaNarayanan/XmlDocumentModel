#ifndef XMLREADER_H
#define XMLREADER_H
///////////////////////////////////////////////////////////////////////
// XmlReader.h - Parse XML strings                                    //
// ver 2.3                                                            //
// Application: CSE687 - Project 2 (XML Document Model), Spring 2015  //
// Platform:    HP G62, Windows 7 Home Premium, Visual Studio 2013    //
// Author:      Jim Fawcett, CST 4-187, 443-3948                      //
//              jfawcett@twcny.rr.com                                 //
//Modified By:  Dhivya Narayanan, Master's- Computer Engineering,     //
//			    Syracuse University,                                  //
//              dnnaraya@syr.edu                                      //
////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This package contains a single class, XmlReader.  It provides a
* simple interface to step from XML element to element and extract
* constituent parts, e.g., tag, attributes, and body.  The package
* does not support editing XML strings.  For that, you should use
* the XmlDocument package.
*
* Parsing honors element scope, e.g., if there are two elements with
* the same tag names at different levels of the document scope, the
* XmlReader will not get confused and prematurely close an element
* because it found a matching closing tag for a contained element.
*
* Exceptions will be thrown by element(), body(), and attributes()
* if malformed XML is detected.
*
* Required Files:
* ===============
* XmlReader.h, XmlReader.cpp, CmdLineParser.h, CmdLineParser.cpp
*
* Build Process:
* ==============
* devenv Project2.sln /debug rebuild
*
* Maintenance History:
* ====================
* ver 2.3 : 05 Jan 13
* - added #include <algorithm> to XmlReader.cpp due to change in library
*   layout for C++11.
* ver 2.2 : 15 Feb 12
* - fixed bug found by Yateen Nikharge in element(). Search for
*   element end was not selective enough.
* - previously added member functions position() and position(size_t p)
*   to enable measuring and changing internal string index - used in
*   creating Visual Studio project.
* ver 2.1 : 25 Jul 11
* - added comments to manual page, above, and implementations in
*   XmlReader.cpp
* ver 2.0 : 27 Jun 11
* - moved #include<stack> to XmlReader.cpp
* - moved #include<iostream> to test stub in XmlReader.cpp
* ver 1.0 : 26 Jun 11
* - first release
*/

#include <vector>
#include"../CmdLineParser/CmdLineParser.h"

class XmlReader
{
public:
	typedef std::vector<std::pair<std::string, std::string>> attribElems;
	typedef std::pair<std::vector<std::string>, std::vector<std::string>> text_childElems;
	XmlReader(const std::string& xmlstr);  //Promotion constructor
	std::string srcxml();
	bool next();
	std::string tag();	
	attribElems attributes();
	std::string body();
	std::string element();
	std::string comment(std::string&elem);
	text_childElems text_and_child();          //find text and child node of the element
	std::vector<std::string> child();
	std::string extractIdentifier(size_t& pos, std::string& xmlstr);
	void reset();
	size_t position();
	bool position(size_t p);
	std::string& xml();
private:	
	std::string src;		// source string
	std::string _xml;
	size_t _position;	
	std::string _tag;      // current element parts
	attribElems _attributes;
	std::vector<std::string> _children;
	std::vector<std::string> _text;
	size_t localposition;
};

inline void XmlReader::reset() { _position = 0; }

inline size_t XmlReader::position() { return _position; }

inline std::string& XmlReader::xml() { return _xml; }

#endif
