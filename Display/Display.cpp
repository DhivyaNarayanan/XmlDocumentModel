//////////////////////////////////////////////////////////////////////////////////
// Display.cpp -   Implemented to Support console display part of Project       //
//				                                                                //
// ver 1.0                                                                      //
// Language:       Visual C++ 2013                                              //
// Application:    CSE687 - Project 2 (XML Document Model), Spring 2015         //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013           //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,            //
//				   Syracuse University,                                         //
//                 dnnaraya@syr.edu                                             //
//////////////////////////////////////////////////////////////////////////////////

#include"Display.h"
#include<string>
#include<vector>
#include<iostream>
#include<fstream>

//-------------< helper function  >---------------------------------------

void titledisplay(std::string str, char ch)
{
	std::cout << str << "\n";
	for (int i = 0; i < str.size() + 2; i++)
		std::cout << ch;
	std::cout << "\n";
}

//-------------< Promotion constructor >---------------------------------------

Display::Display(XmlProcessing::XmlDocument &doc) : xdoc(doc)
{
}

//-------------< displays the internal parse tree representation >---------------------------------------

void Display::display_tree(std::shared_ptr<XmlProcessing::AbstractXmlElement>&pDoc)
{
	std::cout << "\nXml Parse tree Representation of the given xml file or String :: \n";
	std::cout << "=================================================================\n";
	std::cout << pDoc->toString() << "\n\n "; 
	
}

//-------------< displays the output of the query result. Displays tag name and the element>---------------------------------------

void Display::query_result(std::vector<std::shared_ptr<AbstractXmlElement>>&query)
{
	std::cout << "Result :: \n";
	std::cout << "------------\n";
	for (int i = 0; i < query.size(); i++)
	{
		std::cout << "\n"<<i+1 << ")   ";
		std::cout << "Tag =  " << query[i]->value();
		std::cout << "\nElement : \n";
		std::cout << "----------\n\t";
		std::cout << query[i]->toString()<<"\n";
		
	}
	std::cout << "\n------------------------------------------------------\n\n";
}

//-------------< displays the output element of the search function >---------------------------------------

void Display::display_element(std::shared_ptr<AbstractXmlElement>&pelem)
{
	if (pelem != nullptr)
	{
		std::cout << "\nTag = " << pelem->value();
		std::cout << "\nElement = " << pelem->toString();
		std::cout << "\n";		
	}
	else
		std::cout << "Element is Empty (NULL )\n\n";
	std::cout << "------------------------------------------------------\n\n";
}

//-------------< displays attributes of the element >---------------------------------------

void Display::display_attrib(std::vector<std::pair<std::string, std::string>>&attrib)
{
	std::cout << " Attributes of the given element are :: \n";
	for (int i = 0; i < attrib.size(); i++)
	{
		std::cout << i+1 << ")  " << "Name : " << attrib[i].first << " , " << "Value : " << attrib[i].second;
		std::cout << "\n";
	}
	std::cout << "------------------------------------------------------\n\n";
}

//-------------< demonstrates queries to the xml parse tree >---------------------------------------

void Display::demo_queries(XmlDocument &doc)
{
	std::string ip_tag = "title";
	titledisplay("Query 1 :: doc.element(tag).select()  =>Find element with tag 'title' ", '-');
	query_result(doc.element(ip_tag).select());
	titledisplay("Query 2 :: doc.elements(tag).select() =>Find collection of elements with tag 'title'", '-');
	query_result(doc.elements(ip_tag).select());
	ip_tag = "LectureNote";
	titledisplay("Query 3 :: doc.element(tag).children().select() =>Find children of the element with tag 'LectureNote'", '-');
	query_result(doc.element(ip_tag).children().select());
	titledisplay("Query 4 :: doc.element(tag).descendents().select() =>Find descendants of the element with tag 'LectureNote'", '-');
	query_result(doc.element(ip_tag).descendents().select());
	titledisplay("Query 5 :: doc.element(tag).children(ctag).select() =>Find children with tag 'comment' of the element with tag 'LectureNote'", '-');
	std::string child_tag = "comment";
	query_result(doc.element(ip_tag).children(child_tag).select());
	titledisplay("Query 6 :: doc.element(tag).children(ctag).select() =>Find children with tag 'author' of the element with tag 'LectureNote'", '-');
	child_tag = "author";
	query_result(doc.element(ip_tag).children(child_tag).select());
	titledisplay("Query 7 :: doc.element(tag).descendents().select()  =>Find descendants with the tag 'author'of the element with tag 'LectureNote'", '-');
	query_result(doc.element(ip_tag).descendents(child_tag).select());
}

//-------------< demonstartes search element by attributes in the tree >---------------------------------------

void Display::demo_searchElem(XmlDocument&doc)
{
	std::string name = "Company";
	std::string val = "Wintellect";
	display_element(doc.elemByAttrib(name, val));     //search element by attribute and display the element
}

//-------------< demonstrates find attributes of the given element in the tree >---------------------------------------

void Display::demo_findAttrib(XmlDocument&doc)
{
	std::string ip_tag = "LectureNote";
	std::shared_ptr<AbstractXmlElement> elem = doc.element(ip_tag).select()[0];
	display_attrib(doc.Attributes(elem));			//display the attributes of the given element
}

//-------------< demonstrates add and/or remove child to/from the element in the xml tree >---------------------------------------

void Display::demo_addremoveChild(XmlDocument&doc)
{
	XmlWriter writer;
	std::shared_ptr<AbstractXmlElement> newelem = makeTaggedElement("Edition");
	newelem->addChild(makeTextElement("Ninth Edition"));					//add new child to the element
	std::cout << "New child to be added is  :  \n" << newelem->toString();
	bool isAdded = doc.element("reference").select()[0]->addChild(newelem);
	if (isAdded)
	{
		writer.create_newfile(doc.getTree());				//writes the modified xml string into file
		std::cout << "\nThe new child is added successfully and modified xml is written into the file.\n";
		display_tree(doc.getTree());
	}
	else
		std::cout << "Cannot add the child to the element..\n";

	titledisplay("Demonstrating Requirement 7 -- Removechild from the element found by tag'reference'::", '=');
	std::shared_ptr<AbstractXmlElement> oldelem = doc.element("page").select()[0];
	std::cout << "Element to be removed is  : \n" << oldelem->toString();
	bool isRemoved = doc.element("reference").select()[0]->removeChild(oldelem);	//remove child from the element
	if (isRemoved)
	{
		writer.create_newfile(doc.getTree());
		std::cout << "The given child(page) is removed successfully and modified xml is written into the file.\n";
		display_tree(doc.getTree());
	}
	else
		std::cout << "Cannot remove the child from the element..either parent element has no child or no child with the given element\n";

}

//-------------< demonstrates add and/or remove attributes to/from the element in xml tree >---------------------------------------

void Display::demo_addremoveAttrib(XmlDocument&doc)
{
	XmlWriter writer;
	std::string name = "SSN";
	std::string val = "S1234";
	bool isSuccess = doc.element("author").select()[0]->addAttrib(name, val);		// add attribute to the element
	std::cout << "New attribute to be added is  ::  Name : " << name << "   ,  Value  : " << val << "\n\n";
	if (isSuccess)
	{
		writer.create_newfile(doc.getTree());
		std::cout << "Attribute is added successfully and modified xml is written into the file.\n";		
		display_tree(doc.getTree());
	}
	else
		std::cout << "Cannot add the attribute to the element..\n";

	titledisplay("Demonstrating Requirement 9 -- Remove attribute to the element with the given tag 'note'", '=');
	name = "Company";
	isSuccess = doc.element("note").select()[0]->removeAttrib(name);		//remove attribute to the element
	std::cout << "Attribute to be removed ::  Name : " << name << "\n\n";
	if (isSuccess)
	{
		writer.create_newfile(doc.getTree());
		std::cout << "Attribute is removed successfully and modified xml is written into the file.\n";		
		display_tree(doc.getTree());
	}
	else
		std::cout << "Cannot remove the attribute to the element..\n";
}
std::shared_ptr<AbstractXmlElement> Display::test_func()
{
	std::shared_ptr<AbstractXmlElement> tmpPtr = makeTaggedElement("Parent");
	std::shared_ptr<AbstractXmlElement>txtptr = makeTextElement("Test Textnode");
	std::shared_ptr<AbstractXmlElement>childptr = makeTaggedElement("child1");
	tmpPtr->addChild(txtptr);
	tmpPtr->addChild(childptr);
	std::cout << "\n temp Tagged element to be moved (in local function - test_func()) \n";
	display_element(tmpPtr);
	return tmpPtr;
}

//-------------< demonstrates move constructor and move assignment operator >---------------------------------------

void Display::demo_move(XmlDocument &doc)
{
	std::shared_ptr<AbstractXmlElement> rootptr = makeTaggedElement("root");
	std::cout << " Testing move constructor -- example 1 -- gets the return value of the function \n";
	std::cout << "---------------------------------";
	std::cout << " \nRoot Tagged element ...before move ..\n";
	display_element(rootptr);
	rootptr = test_func();					//calls move constructor
	std::cout << " \nRoot Tagged element ...after move ..\n";
	display_element(rootptr);	
	std::cout << " Testing move constructor -- example 2 -- uses std::move  \n";
	std::cout << "---------------------------------";
	std::cout << "\n Element to be moved  is :: \n";
	display_element(rootptr);
	std::shared_ptr<AbstractXmlElement> newPtr = makeTaggedElement("newroot");
	std::cout << "\n new Tagged element...before move..\n";
	display_element(newPtr);
	newPtr = std::move(rootptr);			//calls move constructor
	std::cout << " \nnew Tagged element ...after move ..\n";
	display_element(newPtr);
	std::cout << "\n Element which is moved to new Tagged element ...after move..\n\n";
	display_element(rootptr);

}

//------------------< test stub >--------------------------------------------------------


#ifdef TEST_DISPLAY

int main()
{
	std::cout << "Testing Display Package\n";
	std::cout << "==========================";
	std::string Prologue = "<?xml version=\"1.0\"?><!-- top level comment -->";
	std::string test1 = Prologue;
	test1 += "<parent att1='val1' att2='val2'><child1 /><child2>child2 body<child1 /></child2></parent>";
	XmlDocument xdoc(test1);
	Display dp(xdoc);
	xdoc.buildTree();
	dp.display_tree(xdoc.getTree());
	dp.demo_queries(xdoc);
	dp.demo_searchElem(xdoc);
	dp.demo_findAttrib(xdoc);
	dp.demo_addremoveAttrib(xdoc);
	dp.demo_addremoveChild(xdoc);

	return 0;
}
#endif