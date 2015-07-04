//////////////////////////////////////////////////////////////////////////////////////
// CmdLineParser.cpp - Implemented to Parse Command line arguments                  //
//				                                                                    //
// ver 1.0                                                                          //
// Language:       Visual C++ 2013                                                  //
// Application:    CSE687 - Project 2 (XML Document Model), Spring 2015             //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013               //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,                //
//				   Syracuse University,                                             //
//                 dnnaraya@syr.edu                                                 //
//////////////////////////////////////////////////////////////////////////////////////
#include"CmdLineParser.h"
#include<iostream>
#include<fstream>
#include<string>

//-------------< Promotion constructor >---------------------------------------

CmdLineParser::CmdLineParser(const std::string& src, bool isFile) : _src(src), _isFile(isFile)
{
}

//-------------< converts given xml file into xml string>---------------------------------------

std::string CmdLineParser:: XmlString()
{
	if (_isFile)
	{
		std::string str;
		std::ifstream infile;
		infile.open(_src);				//opens the file 
		while (infile.good())
		{
			getline(infile, str);		//read line by line and store it in str
			_xml += str;
		}
		infile.close();
	}
	else
		_xml = _src;

	return _xml;
}

//-------------< display source info includes source type and source string>---------------------------------------

void CmdLineParser::src_Info()
{
	std::cout << "\nSourceType  ::  ";
	if (_isFile)
	{
		std::cout << "File";
		std::cout << "\nFileName  ::  ";
		std::cout << _src;
	}
	else
	{
		std::cout << "String";
		std::cout << "XmlString  ::  ";
		std::cout << _src;
	}
		
}

//------------------< test stub >--------------------------------------------------------

#ifdef TEST_CMDLINEPARSER

int main(int argc, char * argv[])
{
	std::cout<< " Testing Command line parser " ;
	std::cout << "\n===========================\n";
	if (argc < 3)
	{
		std::cout
			<< "\n  please enter name of file to process on command line and it is file or not.( either true or false as second argument)\n\n";
		return 1;
	}
	CmdLineParser cmdlp(argv[1],argv[2]);
	std::cout<< cmdlp.XmlString();
	std::cout<<"\n\n";
}

#endif