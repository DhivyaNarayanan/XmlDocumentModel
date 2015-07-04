#ifndef XMLWRITER_H
#define XMLWRITER_H

//////////////////////////////////////////////////////////////////////////////////////
// XmlWriter.h -  writes the xml string into xml file                               //
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
This package provides a single class: XmlWriter
-This class is intended to provide support for writing xml strings into the file


Function Prologues - Public Interfaces
======================================
create_newfile();   //open and write the xml string into the file


Required Files:
================
XmlWriter.h, XmlWriter.cpp, XmlDocument.h, XmlDocument.cpp

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
#include"../XmlReader/XmlReader.h"
#include<iostream>
#include<fstream>
#include<string>

class XmlWriter
{
public:
	XmlWriter(){}  //void constructor
	void create_newfile(std::shared_ptr<XmlProcessing::AbstractXmlElement>&pDoc);
};
#endif