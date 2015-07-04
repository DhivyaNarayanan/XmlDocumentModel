////////////////////////////////////////////////////////////////////////////
// Executive.cpp - The first package that gets called.                    //
//                  Oversees the control flow of the entire project       //
//                                                                        //
// ver 1.0                                                                //
// Language:       Visual C++ 2013                                        //
// Application:    CSE687 - Project 2 (XML Document model), Spring 2015   //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013     //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,      //
//				   Syracuse University,                                   //
//                 dnnaraya@syr.edu                                       //
////////////////////////////////////////////////////////////////////////////
/*
Package Operations:
===================
This package doesn't conatin any class.
This module is intended to oversee the program flow. This is the
entry point to the application. All the calls to the subsequent packages
will be routed from here.

Public Interfaces:
==================
None

Build Process:
==============

Required Files:
---------------
Executive.cpp, CmdLineParser.cpp, Display.cpp,XmlDocument.cpp, XmlReader.cpp, XmlWriter.cpp, XmlElement.cpp

Build Command:
--------------
devenv Project2.sln /rebuild debug

/Debug/Executive.exe LectureNote.xml true


Maintanence History:
====================
ver 1.0 - 18 Mar 2015
- first release
*/


/*
**************READ ME**************
XML Document Model Project
==========================
This project has the ability to read xml strings or text files, builds a document object
that can be queried for information about the xml contents, supports programmatic modification of the document,
and can write the revisions to another xml string or file.
- It uses stack based depth first search for tree traversal
Command Line Arguments:
=======================
Two divisions of command line arguments: Xml Filename or XmlString , bool isFile

•	First argument is considered as filename, if isFile(second argument)is true 
•	First argument is considered as xml string , if isFile (second argument) is false

Sample command line arguments:
=============================
- LectureNote.xml true
-"<?xml version="1.0" encoding="utf-8"?><!-- XML test case --><LectureNote course="CSE681"><title>XML Example #1</title></LectureNote>" false
*
*/

#include<iostream>
#include<fstream>
#include<string>
#include<vector>

#include"../CmdLineParser/CmdLineParser.h"
#include"../XmlDocument/XmlDocument.h"
#include"../XmlElement/XmlElement.h"
#include"../XmlReader/XmlReader.h"
#include"../XmlWriter/XmlWriter.h"
#include"../Display/Display.h"

using namespace XmlProcessing;

void titleprint(std::string str, char ch)
{
	std::cout << str << "\n";
	for (int i = 0; i < str.size() + 2; i++)
		std::cout << ch;
	std::cout << "\n";
}

#ifdef TEST_EXECUTIVE

int main(int argc, char*argv[])
{
	titleprint("           Demonstration of Project#2           ", '=');
	CmdLineParser cmdlp(argv[1], argv[2]);   //creating instances for CmdlineParser
	cmdlp.src_Info();
	std::string xmlstr = cmdlp.XmlString();  //Get the XmlString to parse
	XmlDocument doc(xmlstr);				 //creating instances for XmlDocument, XmlWriter,Display
	XmlWriter writer;
	std::shared_ptr < AbstractXmlElement > root = doc.buildTree();    //build an internal Parse tree
	Display dp(doc);
	dp.display_tree(root);					//displays the parse tree representation
	std::string ip_tag = "title";
	titleprint("Demonstrating Requirement 4 - move constructor and move assignment operator", '=');
	dp.demo_move(doc);
	titleprint("Demonstrating Requirement 5 -- General search element based on its Id attribute :: ", '=');
	dp.demo_searchElem(doc);										//search element demo in display package
	titleprint("Demonstrating Queries - Requirement 6 :: ", '=');
	dp.demo_queries(doc);											//query the xml parse tree	
	titleprint("Demonstrating Requirement 7 -- Add child element to the element found by tag'reference'::", '=');
	dp.demo_addremoveChild(doc);									//demonstrate add /remove child  in display package
	titleprint("Demonstrating Requirement 8 -- Find attributes of the given element (LectureNote) :: ", '=');
	dp.demo_findAttrib(doc);										//find attributes demo in display package
	titleprint("Demonstrating Requirement 9 -- Add attribute to the element with the given tag 'author'", '=');
	dp.demo_addremoveAttrib(doc);									//demonstrate add/remove attribute in display package

	std::cout << "\n<-------------End of Demonstration---------------->\n";

}
#endif
