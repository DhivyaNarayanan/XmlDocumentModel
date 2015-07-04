//////////////////////////////////////////////////////////////////////////////////////
// XmlDocument.cpp -  a container of XmlElement nodes                                 //
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

#include <iostream>
#include<stack>
#include<string>
#include "XmlDocument.h"

using namespace XmlProcessing; 
using sPtr = std::shared_ptr < AbstractXmlElement >;
using attribElems = std::vector<std::pair<std::string, std::string>>;
using textChildElems = std::pair<std::vector<std::string>, std::vector<std::string>>;


//-------------< Promotion constructor >---------------------------------------

XmlProcessing::XmlDocument::XmlDocument(const std::string& src, sourceType srcType) : srcxml(src)
{
}

//-------------< move constructor >---------------------------------------

XmlProcessing::XmlDocument::XmlDocument(XmlDocument&&xmldoc) : srcxml(xmldoc.srcxml), pDocElement_(xmldoc.pDocElement_)
{
	xmldoc.pDocElement_ = nullptr;
	found_ =std::move(xmldoc.found_);
}

//-------------< move assignment operator >---------------------------------------

XmlDocument& XmlProcessing::XmlDocument :: operator=(XmlDocument&&xmldoc)
{
	if (this == &xmldoc)
		return *this;
	srcxml = xmldoc.srcxml;
	pDocElement_ = xmldoc.pDocElement_;
	xmldoc.pDocElement_ = nullptr;
	found_ = std::move(xmldoc.found_);
	return *this;
}

//-------------< Build the tree representation of the xml string >---------------------------------------

XmlProcessing::XmlDocument::sPtr XmlDocument::buildTree()
{
	XmlReader reader(srcxml);
	std::stack<std::pair<std::string, std::shared_ptr<AbstractXmlElement>>> childStack;
	sPtr root;
	sPtr docroot = makeDocElement(root);
	while (reader.next())
	{
		std::string tag = reader.tag();
		std::string elem = reader.element();
		std::string comment = reader.comment(elem);
		std::vector<std::pair<std::string, std::string>> attrib = reader.attributes();
		std::pair<std::vector<std::string>, std::vector<std::string>> txt_child = reader.text_and_child();		
		if (tag == "?xml")		//XmlDeclaration element
			docroot->addChild(addDecl(tag, attrib));
		else if (tag[1] == '?')		//ProcInstruction
			docroot->addChild(addProc(tag, attrib));
		else if (tag == "!--")		//comment element
			docroot->addChild(makeCommentElement(comment));
		else if (childStack.empty())  // root tagged element
		{					
			std::pair<sPtr, std::vector<std::pair<std::string, sPtr>>> rootelem = addRootTag(tag, attrib, txt_child);;
			for (int i = rootelem.second.size() - 1; i >= 0; --i)
					childStack.push(rootelem.second[i]);				
			docroot->addChild(rootelem.first);
		}
		else  //child tag element
		{
			std::pair<std::string, sPtr> tptr = childStack.top();
			std::vector<std::pair<std::string, sPtr>> childvec;
			if (tag.compare(tptr.first) == 0)
			{
				childStack.pop();
				sPtr currenttag = tptr.second;
				childvec = addTag(currenttag, attrib, txt_child);
				for (int i = childvec.size() - 1; i >= 0; --i)
					childStack.push(childvec[i]);									
			} 
		}											
	} //end of while loop
	pDocElement_ = docroot;
	return pDocElement_;
}
//-------------< helper function to build tree - add declaration element>---------------------------------------

XmlProcessing::XmlDocument::sPtr XmlDocument::addDecl(std::string& tag,attribElems& attrib)
{
	sPtr dec_ele = makeXmlDeclarElement();

	for (int i = 0; i < attrib.size(); i++)
	{
		dec_ele->addAttrib(attrib[i].first, attrib[i].second);
	}
	return dec_ele;
}

//-------------< helper function to build tree - add ProcInstr element>---------------------------------------

XmlProcessing::XmlDocument::sPtr XmlDocument::addProc(std::string& tag,attribElems &attrib)
{
	sPtr proc_ele = makeProcInstrElement(tag);
	for (int i = 0; i < attrib.size(); i++)
	{
		proc_ele->addAttrib(attrib[i].first, attrib[i].second);
	}
	return proc_ele;
}

//-------------< helper function to build tree - add roottag element>---------------------------------------

std::pair<sPtr, std::vector<std::pair<std::string, sPtr>>> XmlDocument::addRootTag(std::string&tag, attribElems&attrib, textChildElems&txt_child)
{
	sPtr roottag = makeTaggedElement(tag);
	std::vector<std::pair<std::string, sPtr>> childvec;
	if (txt_child.first.size() != 0)
	{
		for (int i = 0; i < txt_child.first.size(); i++)
		{
			sPtr txt_ele = makeTextElement(txt_child.first[i]);
			roottag->addChild(txt_ele);
		}
	}
	if (txt_child.second.size() != 0)
	{
		for (int i = 0; i < txt_child.second.size(); i++)
		{
			sPtr childtag = makeTaggedElement(txt_child.second[i]);
			std::pair<std::string, sPtr> tag_ptr;
			tag_ptr.first = txt_child.second[i];
			tag_ptr.second = childtag;
			childvec.push_back(tag_ptr);
			roottag->addChild(childtag);
		}
	}
	if (attrib.size() != 0)
	{
		for (int i = 0; i < attrib.size(); i++)
		{
			roottag->addAttrib(attrib[i].first, attrib[i].second);
		}
	}
	std::pair<sPtr, std::vector<std::pair<std::string, sPtr>>> rootelem;
	rootelem.first = roottag;
	rootelem.second = childvec;
	return rootelem;
}

//-------------< helper function to build tree - add tag element>---------------------------------------

std::vector<std::pair<std::string, sPtr>> XmlDocument::addTag(sPtr &currenttag, attribElems&attrib, textChildElems &txt_child)
{
	std::vector<std::pair<std::string, sPtr>> childvec;
	if (txt_child.first.size() != 0)
	{
		for (int i = 0; i < txt_child.first.size(); i++)
		{
			sPtr txt_ele = makeTextElement(txt_child.first[i]);
			currenttag->addChild(txt_ele);
		}
	}
	if (txt_child.second.size() != 0)
	{		
		for (int i = 0; i < txt_child.second.size(); i++)
		{
			sPtr childtag = makeTaggedElement(txt_child.second[i]);
			std::pair<std::string, sPtr> tag_ptr;
			tag_ptr.first = txt_child.second[i];
			tag_ptr.second = childtag;
			childvec.push_back(tag_ptr);
			currenttag->addChild(childtag);
		}
	}
	if (attrib.size() != 0)
	{
		for (int i = 0; i < attrib.size(); i++)
		{
			currenttag->addAttrib(attrib[i].first, attrib[i].second);
		}
	}
	return childvec;
}

//-------------< found_[0] contains first element (DFS order) with tag - uses stack based DFS >---------------------------------------

XmlDocument& XmlDocument::element(const std::string& tag)
{
	sPtr doc = pDocElement_;
	found_.clear();
	std::stack<sPtr> nodestack;
	nodestack.push(doc);
	while (nodestack.size() > 0)
	{
		sPtr curPtr = nodestack.top();
		nodestack.pop();

		if (curPtr->value() == tag)
		{
			found_.push_back(curPtr);
			return *this;
		}
		size_t numChildren = curPtr->getChildren().size();
		for (size_t i = 0; i < numChildren; ++i)
			nodestack.push(curPtr->getChildren()[numChildren - i - 1]);
	}
	return *this;
}

//-------------< found_ contains all elements witch tag - uses stack based DFS>---------------------------------------

XmlDocument& XmlDocument::elements(const std::string& tag)
{
	sPtr doc = pDocElement_;
	found_.clear();
	std::stack<sPtr> nodestack;
	nodestack.push(doc);
	while (nodestack.size() > 0)
	{
		sPtr curPtr = nodestack.top();
		nodestack.pop();

		if (curPtr->value() == tag)
		{
			found_.push_back(curPtr);			
		}
		size_t numChildren = curPtr->getChildren().size();
		for (size_t i = 0; i < numChildren; ++i)
			nodestack.push(curPtr->getChildren()[numChildren - i - 1]);
	}
	return *this;
}

//-------------< found_ contains sPtrs to children of prior found_[0] - uses stack based DFS >---------------------------------------

XmlDocument& XmlDocument::children(const std::string& tag)
{
	
	sPtr pelem = found_[0];
	found_.clear();
	std::stack<sPtr> nodestack;
	nodestack.push(pelem);
	while (nodestack.size() > 0)
	{
		sPtr curPtr = nodestack.top();
		nodestack.pop();

		if (pelem == curPtr)
		{
			std::vector<sPtr> childvec = curPtr->getChildren();
			for (int i = 0; i < childvec.size(); i++)
			{
				if (tag == "")
				  found_.push_back(childvec[i]);
				if (tag == childvec[i]->value())
				{
					found_.push_back(childvec[i]);
					break;
				}
					
			}				
			break;
		}
		size_t numChildren = curPtr->getChildren().size();
		for (size_t i = 0; i < numChildren; ++i)
			nodestack.push(curPtr->getChildren()[numChildren - i - 1]);
	}

	return *this;
}

//-------------< found_ contains sPtrs to descendents of prior found_[0] - uses stack DFS >---------------------------------------

XmlDocument& XmlDocument:: descendents(const std::string& tag )
{
	sPtr pelem =found_[0];
	found_.clear();
	std::stack<sPtr> nodestack;
	nodestack.push(pelem);
	while (nodestack.size() > 0)
	{
		sPtr curPtr = nodestack.top();
		nodestack.pop();
		std::vector<sPtr> childvec = curPtr->getChildren();
		for (int i = 0; i < childvec.size(); i++)
		{
			if ( tag == "")
				found_.push_back(childvec[i]);
			if (tag == childvec[i]->value())
			{
				found_.push_back(childvec[i]);
				break;
			}
		}					
		size_t numChildren = curPtr->getChildren().size();
		for (size_t i = 0; i < numChildren; ++i)
			nodestack.push(curPtr->getChildren()[numChildren - i - 1]);
	}

	return *this;
}

//-------------<  return pointer to the elem which contains the given attribute pair >---------------------------------------

XmlProcessing::XmlDocument::sPtr XmlDocument::elemByAttrib(std::string&name, std::string&val)
{
	sPtr doc = pDocElement_;
	std::stack<sPtr> nodestack;
	nodestack.push(doc);
	while (nodestack.size() > 0)
	{
		sPtr curPtr = nodestack.top();
		nodestack.pop();
		TaggedElement* te = dynamic_cast<TaggedElement*>(curPtr.get());
		ProcInstrElement* pInc = dynamic_cast<ProcInstrElement*>(curPtr.get());
		XmlDeclarElement* pDecl = dynamic_cast<XmlDeclarElement*>(curPtr.get());
		if (te != nullptr || pInc != 0 || pDecl != 0)
		{
			std::vector<std::pair<std::string, std::string>>attribVec = curPtr->getAttrib();
			if (attribVec.size() > 0)
			{
				for (int i = 0; i < attribVec.size(); i++)
				{
					if (attribVec[i].first == name && attribVec[i].second == val)
						return curPtr;
				}
			}
		}
		size_t numChildren = curPtr->getChildren().size();
		for (size_t i = 0; i < numChildren; ++i)
			nodestack.push(curPtr->getChildren()[numChildren - i - 1]);		
	}
	return nullptr;
}

//-------------< return attributes of the given element >---------------------------------------

std::vector<std::pair<std::string, std::string>>XmlProcessing::XmlDocument::Attributes(std::shared_ptr<AbstractXmlElement> pelem)
{
	sPtr doc = pDocElement_;
	std::stack<sPtr> nodestack;
	std::vector<std::pair<std::string, std::string>>attribVec;
	nodestack.push(doc);
	while (nodestack.size() > 0)
	{
		sPtr curPtr = nodestack.top();
		nodestack.pop();
		
		if (pelem == curPtr)
		{
			attribVec = curPtr->getAttrib();
			return attribVec;
		}
		size_t numChildren = curPtr->getChildren().size();
		for (size_t i = 0; i < numChildren; ++i)
			nodestack.push(curPtr->getChildren()[numChildren - i - 1]);
	}
	return attribVec;
}

//-------------<  return reference to found_. >---------------------------------------

std::vector<XmlProcessing::XmlDocument::sPtr>& XmlProcessing::XmlDocument::select()
{
	return std::move(this->found_);
}

//-------------< return shared_ptr to the doc element(pDocElement_) >---------------------------------------

XmlProcessing::XmlDocument::sPtr XmlDocument::getTree()
{
	return pDocElement_;
}

//------------------< test stub >--------------------------------------------------------

#ifdef TEST_XMLDOCUMENT

int main(int argc, char*argv[])
{
	title("Testing XmlDocument class");
	CmdLineParser cmdlp(argv[1], argv[2]);
	std::string xmlstr = cmdlp.XmlString();
	XmlDocument doc(xmlstr);
	std::shared_ptr < AbstractXmlElement > root = doc.buildTree();
	std::string output = root->toString();
	std::cout << output;
	std::cout << "\n\nTesting search element by attribute method : \n";
	std::string name = "course";
	std::string val = "CSE681";
	std::shared_ptr < AbstractXmlElement > elem_containsattrib = doc.elemByAttrib(name, val);
	std::string tag = elem_containsattrib->value();
	std::cout << "tag of the Element contains the given attribute is :  "<<tag;
	
	std::cout << "\n\nTesting extraction of attributes of the given element : \n";
	std::cout << "Attributes are ::  ";
	std::vector<std::pair<std::string, std::string>> tmpatt = doc.Attributes(elem_containsattrib);
	for (int i = 0; i < tmpatt.size(); i++)
		std::cout << tmpatt[i].first << " , " << tmpatt[i].second << "\n";

	std::cout << "\n\nTesting return reference search elements :: \n";
	std::string ttag = "LectureNote";
	std::cout << "Given tag :: " << ttag;
	std::cout << "\nFound elements ::  ";
	
	std::vector<std::shared_ptr<AbstractXmlElement>> elem = doc.elements(ttag).select();
	for (int i = 0; i < elem.size(); i++)
		std::cout << elem[i]->value() << "\n\n";

	std::cout << "\n\nTesting return reference search children :: \n";
	std::cout << "Given tag element :: " << tag;
	std::cout << "\nFound child element ::  ";
	 elem = doc.element(tag).children().select();
	for (int i = 0; i < elem.size(); i++)
		std::cout << elem[i]->value() << "\n\n";

	std::cout << "\n\nTesting return reference search descendants:: \n";
	std::cout << "Given tag element :: " << ttag;
	
	std::cout << "\nFound child element ::  ";	
	 elem = doc.element(ttag).descendents().select();
	for (int i = 0; i < elem.size(); i++)
		std::cout << elem[i]->value() << "\n\n";

	std::cout << "\n\n";
}
#endif