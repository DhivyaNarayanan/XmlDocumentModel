#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
//////////////////////////////////////////////////////////////////////////////////////
// XmlDocument.h -  a container of XmlElement nodes                                 //
//				                                                                    //
// ver 1.0                                                                          //
// Language:       Visual C++ 2013                                                  //
// Application:    CSE687 - Project 2 (XML Document Model), Spring 2015             //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013               //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,                //
//				   Syracuse University,                                             //
//                 dnnaraya@syr.edu                                                 //
//Reference:       Project2HelpS15 by Jim Fawcett                                   //
//////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* It uses C++11 constructs, most noteably std::shared_ptr.  The XML Document Model is essentially
* a program-friendly wrapper around an Abstract Syntax Tree (AST) used to
* contain the results of parsing XML markup.
*
* Abstract Syntax Trees, defined in this package, are unordered trees with
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes
*
* Function Prologues
* ====================
  buildTree();      //Build the tree representation of the xml string
  element();        // found_[0] contains first element (DFS order) with tag
  elements();       // found_ contains all elements witch tag
  children();       // found_ contains sPtrs to children of prior found_[0]
  descendents();    // found_ contains sPtrs to descendents of prior found_[0]
  select();         // return reference to found_.  Use std::move(found_) to clear found_
  getTree()         // return shared_ptr to the doc element(pDocElement_)
  elemByAttrib();   // return pointer to the elem which contains the given attribute pair
  Attributes();     //return attributes of the given element


* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp, XmlElement.h, XmlElement.cpp
*     XmlReader.h , XmlReader.cpp, CmdLineParser.h,CmdLineParser.cpp
*
* Build Process:
* --------------
*   devenv Project2.sln /debug rebuild
*
* Maintenance History:
* --------------------
*
* Ver 1.0 : 12 Mar 15
* - first release
*
*/

#include <memory>
#include <string>
#include "../XmlElement/XmlElement.h"
#include "../XmlReader/XmlReader.h"
#include"../CmdLineParser/CmdLineParser.h"

namespace XmlProcessing
{
	class XmlDocument
	{
	public:
		using sPtr = std::shared_ptr < AbstractXmlElement >;
		using attribElems = std::vector<std::pair<std::string, std::string>>; 
		using textChildElems = std::pair<std::vector<std::string>, std::vector<std::string>>;
		enum sourceType { string, filename };
		XmlDocument(const std::string& src, sourceType srcType = string);		//Promotion constructor
		XmlDocument(XmlDocument&& xmldoc);										//move constructor
		XmlDocument& operator=(XmlDocument&& xmldoc);							//move assignment operator
		sPtr buildTree();      

		//helper functions to build tree
		sPtr addDecl(std::string&tag,attribElems& attrib);
		sPtr addProc(std::string&tag,attribElems &attrib);
		std::pair<sPtr, std::vector<std::pair<std::string, sPtr>>> addRootTag(std::string&tag, attribElems&attrib, textChildElems &txt_child);
		std::vector<std::pair<std::string, sPtr>> addTag(sPtr&tag, attribElems&attrib, textChildElems &txt_child);

		// queries return XmlDocument references so they can be chained, e.g., doc.element("foobar").descendents();
		XmlDocument& element(const std::string& tag);           
		XmlDocument& elements(const std::string& tag);         
		XmlDocument& children(const std::string& tag = "");     
		XmlDocument& descendents(const std::string& tag = "");  
		std::vector<sPtr>& select();                            
		sPtr getTree();											
		sPtr elemByAttrib(std::string&name, std::string&val);   
		std::vector<std::pair<std::string, std::string>> Attributes(std::shared_ptr<AbstractXmlElement> pelem);  
				

	private:
		std::string srcxml;		
		sPtr pDocElement_;         // holds Xml Declaration,procInstr, comments, XML root, and more comments
		std::vector<sPtr> found_;  // query results
		
	};
}
#endif
