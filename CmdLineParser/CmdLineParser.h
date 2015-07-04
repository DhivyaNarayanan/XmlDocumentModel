#ifndef CMDLINEPARSER_H
#define CMDLINEPARSER_H

//////////////////////////////////////////////////////////////////////////////////////
// CmdLineParser.h -  Parses Command line arguments                                 //
//				                                                                    //
// ver 1.0                                                                          //
// Language:       Visual C++ 2013                                                  //
// Application:    CSE687 - Project 2 (XML Document Model), Spring 2015             //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013               //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,                //
//				   Syracuse University,                                             //
//                 dnnaraya@syr.edu                                                 //
//////////////////////////////////////////////////////////////////////////////////////
/*
Package Operations
==================
This package provides a single class: CmdlineParser
-This class is intended to provide Support for parsing command line arguments and read the xml file and 
put the whole file into single xml string


Function Prologues - Public Interfaces
======================================
XmlString()   //In case of filename, opens the file , reads and put the whole into single string. 
src_Info()    //Displays the source info


Required Files:
================
CmdlineParser.h, CmdlineParser.cpp

Build Process:
===============
devenv Project2.sln /rebuild debug

Maintenance History:
=====================
Ver 1.0 - 18 Mar 2015
- first release
*/

#include<iostream>
#include<string>

class CmdLineParser
{
public:
	CmdLineParser(const std::string& src, bool isFile);     //Promotion constructor
	std::string XmlString();             //converts file into string
	void src_Info();					 //display source info includes source type and source string

private:
	std::string _xml;			//stores the xml string to be parsed
	bool _isFile;
	std::string _src;

};

#endif
