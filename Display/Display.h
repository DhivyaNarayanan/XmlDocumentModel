#ifndef DISPLAY_H
#define DISPLAY_H

//////////////////////////////////////////////////////////////////////////////////
// Display.h -     Supports console display part of Project                     //
//				                                                                //
// ver 1.0                                                                      //
// Language:       Visual C++ 2013                                              //
// Application:    CSE687 - Project 2 (XML Document Model), Spring 2015         //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013           //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,            //
//				   Syracuse University,                                         //
//                 dnnaraya@syr.edu                                             //
//////////////////////////////////////////////////////////////////////////////////
/*
Package Operations
==================
This package provides a single class: Display
-Provides support for displaying xml parse tree, and search and query result


Function Prologues - Public Interfaces
======================================
display_tree();			//displays the internal parse tree representation
query_result();			//displays the output of the query result. Displays tag name and the element
display_element();	    //displays the output element of the search function
display_attrib();       //displays attributes of the element
demo_queries();			//demonstrates queries to the xml parse tree
demo_searchElem();      //demonstartes search element by attributes in the tree
demo_findAttrib();		//demonstrates find attributes of the given element in the tree
demo_addremoveChild();	//demonstrates add and/or remove child to/from the element in the xml tree
demo_addremoveAttrib(); //demonstrates add and/or remove attributes to/from the element in xml tree

Required Files:
================
Display.h, Display.cpp

Build Process:
===============
devenv Project2.sln /rebuild debug

Maintenance History:
=====================
Ver 1.0 - 18 Mar 2015
- first release
*/


#include"../XmlDocument/XmlDocument.h"
#include"../XmlElement/XmlElement.h"
#include"../XmlWriter/XmlWriter.h"
#include<iostream>
#include<string>

using namespace XmlProcessing;

class Display
{
public:
	Display(XmlDocument& xdoc);		//constructor
	void display_tree(std::shared_ptr<AbstractXmlElement> &pDoc);
	void query_result(std::vector<std::shared_ptr<AbstractXmlElement>>&query);
	void display_element(std::shared_ptr<AbstractXmlElement>&pelem);
	void display_attrib(std::vector<std::pair<std::string, std::string>>&attrib);
	void demo_queries(XmlDocument&doc);
	void demo_searchElem(XmlDocument&doc);
	void demo_findAttrib(XmlDocument&doc);
	void demo_addremoveChild(XmlDocument&doc);
	void demo_addremoveAttrib(XmlDocument&doc);
	void demo_move(XmlDocument&doc);
	std::shared_ptr<AbstractXmlElement>test_func();

private:
	XmlDocument & xdoc;				//reference to the class XmlDocument

};
#endif