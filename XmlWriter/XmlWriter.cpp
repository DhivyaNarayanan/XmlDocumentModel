//////////////////////////////////////////////////////////////////////////////////////
// XmlWriter.cpp - Implemented to write the xml string into xml file                //
//				                                                                    //
// ver 1.0                                                                          //
// Language:       Visual C++ 2013                                                  //
// Application:    CSE687 - Project 2 (XML Document Model), Spring 2015             //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013               //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,                //
//				   Syracuse University,                                             //
//                 dnnaraya@syr.edu                                                 //
//////////////////////////////////////////////////////////////////////////////////////
#include"XmlWriter.h"
#include<fstream>
#include<string>
#include<vector>
using namespace std;
using namespace XmlProcessing;

//------------------< write the string into file >--------------------------------------------------------

void XmlWriter::create_newfile(std::shared_ptr<XmlProcessing::AbstractXmlElement>&pDoc)
{
	std::ofstream myfile;
	myfile.open("NewXmlFile.xml");
	myfile << pDoc->toString();
	myfile.close();
}

//------------------< test stub >--------------------------------------------------------

#ifdef TEST_XMLWRITER

int main()
{
	std::cout << "Testing XmlWriter Package";
	std::cout << "\n==========================\n";
	std::string Prologue = "<?xml version=\"1.0\"?><!-- top level comment -->";
	std::string test1 = Prologue;
	test1 += "<parent att1='val1' att2='val2'><child1 /><child2>child2 body<child1 /></child2></parent>";
	XmlDocument xdoc(test1);
	xdoc.buildTree();
	XmlWriter writer;
	writer.create_newfile(xdoc.getTree());
	std::cout << "The xml string is successfully written into another file";
	std::cout << "\n\n";
}
#endif