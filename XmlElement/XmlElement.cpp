////////////////////////////////////////////////////////////////////////
// XmlElement.cpp - define XML Element types                            //
// ver 1.5                                                            //
// Application: CSE687 - Project 2 (XML Document Model), Spring 2015  //
// Platform:    HP G62, Windows 7 Home Premium, Visual Studio 2013    //
// Author:      Jim Fawcett, CST 4-187, 443-3948                      //
//              jfawcett@twcny.rr.com                                 //
//Modified By:  Dhivya Narayanan, Master's- Computer Engineering,     //
//			    Syracuse University,                                  //
//              dnnaraya@syr.edu                                      //
////////////////////////////////////////////////////////////////////////

#include "XmlElement.h"
#include <iostream>

using namespace XmlProcessing;

size_t AbstractXmlElement::count = 0;
size_t AbstractXmlElement::tabSize = 2;

//----< factory for doc elements >----------------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeDocElement(std::shared_ptr<AbstractXmlElement> pRoot)
{
	std::shared_ptr<AbstractXmlElement> ptr(new DocElement(pRoot));
	return ptr;
}

//----< factory for tagged elements >----------------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeTaggedElement(const std::string& tag)
{
	std::shared_ptr<AbstractXmlElement> ptr(new TaggedElement(tag));
	return ptr;
}
//----< factory for text elements >------------------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeTextElement(const std::string& text)
{
	std::shared_ptr<AbstractXmlElement> ptr(new TextElement(text));
	return ptr;
}
//----< factory for comment elements >---------------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeCommentElement(const std::string& text)
{
	std::shared_ptr<AbstractXmlElement> ptr(new CommentElement(text));
	return ptr;
}
//----< factory for processing instruction elements >------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeProcInstrElement(const std::string& text)
{
	std::shared_ptr<AbstractXmlElement> ptr(new ProcInstrElement(text));
	return ptr;
}
//----< factory for XML Declaration elements >-------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeXmlDeclarElement()
{
	std::shared_ptr<AbstractXmlElement> ptr(new XmlDeclarElement());
	return ptr;
}

//----< return empty vector - in AbstractXmlElement>-------------------------------

std::vector<std::shared_ptr<AbstractXmlElement>> AbstractXmlElement::getChildren()
{
	std::vector<std::shared_ptr<AbstractXmlElement>> tmp;
	return tmp;
}

//----< return empty vector - in AbstractXmlElement>-------------------------------

std::vector<std::pair<std::string, std::string>> AbstractXmlElement::getAttrib()
{
	std::vector<std::pair<std::string, std::string>> tmp;
	return tmp;
}
//----< doc element constructor and destructor >-----------------------------

DocElement::DocElement(std::shared_ptr<AbstractXmlElement> pRoot)
{
	if (pRoot != nullptr)
		children_.push_back(pRoot);
}

//----< doc element move constructor  >-----------------------------

DocElement::DocElement(DocElement && doc) : children_(std::move(doc.children_))
{
}

//----< doc element move assignment operator  >-----------------------------

DocElement& DocElement :: operator=(DocElement&& doc)
{
	if (this == &doc)
		return *this;
	delete[] &children_;
	children_ = std::move(doc.children_);
	return *this;
}


//----< private helper for DocElement::addChild(...) >-----------------------

bool DocElement::hasXmlRoot()
{
	for (auto pElement : children_)
	{
		if (dynamic_cast<TaggedElement*>(pElement.get()) != nullptr)
			return true;
	}
	return false;
}

//----< return vector contains children >-------------------------------

std::vector<std::shared_ptr<AbstractXmlElement>> DocElement:: getChildren()
{
	return children_;
}
//----< add only one child to doc element using pointer to child >-----------

bool DocElement::addChild(std::shared_ptr<AbstractXmlElement> pChild)
{
	// don't add TextElements
	TextElement* pTxEl = dynamic_cast<TextElement*>(pChild.get());
	if (pTxEl != 0)  // don't add text elements to DocElement
		return false;

	// don't add another DocElement
	DocElement* pDcEl = dynamic_cast<DocElement*>(pChild.get());
	if (pDcEl != 0)  // don't add text elements to DocElement
		return false;

	XmlDeclarElement* pDecl = dynamic_cast<XmlDeclarElement*>(pChild.get());
	if (pDecl != 0)
	{
		children_.push_back(pChild);
		return true;
	}
	
	CommentElement*pCmt = dynamic_cast<CommentElement*>(pChild.get());
	if (pCmt != 0)
	{
		children_.push_back(pChild);
		return true;
	}
	ProcInstrElement* pInc = dynamic_cast<ProcInstrElement*>(pChild.get());
	if (pInc != 0)
	{
		children_.push_back(pChild);
		return true;
	}
	TaggedElement* te = dynamic_cast<TaggedElement*>(pChild.get());
	if (te != nullptr)  //is tagged element
	{
		if (!hasXmlRoot())
		{
			children_.push_back(pChild);
			return true;
		}
	}
	
	return false;
}
//----< remove child from doc element using pointer to child >---------------

bool DocElement::removeChild(std::shared_ptr<AbstractXmlElement> pChild)
{
	auto iter = std::find(begin(children_), end(children_), pChild);
	if (iter != end(children_))
	{
		children_.erase(iter);
		return true;
	}
	return false;
}
//----< return value = tag from doc element >-----------------------------

std::string DocElement::value() { return std::string(""); }

//----< generate xml string for tagged element >-----------------------------

std::string DocElement::toString()
{
	std::string rtn;
	for (auto elem : children_)
		rtn += elem->toString();
	return rtn;
}
//----< Tagged element move constructor >-----------------

TaggedElement::TaggedElement(TaggedElement&& te) : tag_(te.tag_), children_(std::move(te.children_)), attribs_(std::move(te.attribs_))
{
}

//----< Tagged element move assignment operator >----------------

TaggedElement & TaggedElement :: operator=(TaggedElement&& te)
{
	if (this == &te)
		return *this;
	tag_ = te.tag_;
	delete[] & children_;
	delete[] & attribs_;
	children_ = std::move(te.children_);
	attribs_ = std::move(te.attribs_);
	return *this;
}

//----< add child to tagged element using pointer to child >-----------------

bool TaggedElement::addChild(std::shared_ptr<AbstractXmlElement> pChild)
{
	children_.push_back(pChild);
	return true;
}
//----< remove child from tagged element using pointer to child >------------

bool TaggedElement::removeChild(std::shared_ptr<AbstractXmlElement> pChild)
{
	auto iter = std::find(begin(children_), end(children_), pChild);
	if (iter != end(children_))
	{
		children_.erase(iter);
		return true;
	}
	return false;
}
//----< remove child from tagged element using child's value >---------------

bool TaggedElement::removeChild(const std::string& value)
{
	for (auto iter = begin(children_); iter != end(children_); ++iter)
	{
		if ((*iter)->value() == value)
		{
			children_.erase(iter);
			return true;
		}
	}
	return false;
}
//----< add attribute to tagged element >------------------------------------

bool TaggedElement::addAttrib(const std::string& name, const std::string& value)
{
	std::pair<std::string, std::string> atPair(name, value);
	attribs_.push_back(atPair);
	return true;
}
//----< remove attribute from tagged element >-------------------------------

bool TaggedElement::removeAttrib(const std::string& name)
{
	for (auto iter = begin(attribs_); iter != end(attribs_); ++iter)
	{
		if (iter->first == name)
		{
			attribs_.erase(iter);
			return true;
		}
	}
	return false;
}

std::vector<std::shared_ptr<AbstractXmlElement>> TaggedElement::getChildren()
{
	return children_;
}

std::vector<std::pair<std::string, std::string>> TaggedElement::getAttrib()
{
	return attribs_;
}
//----< return value = tag from tagged element >-----------------------------

std::string TaggedElement::value() { return tag_; }

//----< generate xml string for tagged element >-----------------------------

std::string TaggedElement::toString()
{
	std::string spacer(tabSize*++count, ' ');
	std::string xml = "\n" + spacer + "<" + tag_;
	for (auto at : attribs_)
	{
		xml += " ";
		xml += at.first;
		xml += "=\"";
		xml += at.second;
		xml += "\"";
	}
	xml += ">";
	for (auto pChild : children_)
		xml += pChild->toString();
	xml += "\n" + spacer + "</" + tag_ + ">";
	--count;
	return xml;
}

//----< TextElement move constructor>-------------------------------

TextElement::TextElement(TextElement&& text) :text_(std::move(text.text_))
{
}

//----< TextElement move assignment operator>-------------------------------

TextElement &TextElement::operator=(TextElement && text)
{
	if (this == &text)
		return *this;
	text_ = std::move(text.text_);
	return *this;
}

//----< generate xml string for text element >-------------------------------

std::string TextElement::toString()
{
	std::string spacer(tabSize * ++count, ' ');
	std::string xml = "\n" + spacer + text_;
	--count;
	return xml;
}

//----< ProcInstrElememt move constructor>-------------------------------

ProcInstrElement::ProcInstrElement(ProcInstrElement && pe) : text(pe.text), attribs_(std::move(pe.attribs_))
{
}

//----< ProcInstrElememt move assignment operator >-------------------------------

ProcInstrElement& ProcInstrElement ::operator=(ProcInstrElement&&pe)
{
	if (this == &pe)
		return *this;
	text = std::move(pe.text);
	delete[] & attribs_;
	attribs_ = std::move(pe.attribs_);
	return *this;
}

//----< add attribute to ProcInstElement >-----------------------------------

bool ProcInstrElement::addAttrib(const std::string& name, const std::string& value)
{
	std::pair<std::string, std::string> atPair(name, value);
	attribs_.push_back(atPair);
	return true;
}
//----< remove attribute from ProcInstElement >------------------------------

bool ProcInstrElement::removeAttrib(const std::string& name)
{
	for (auto iter = begin(attribs_); iter != end(attribs_); ++iter)
	{
		if (iter->first == name)
		{
			attribs_.erase(iter);
			return true;
		}
	}
	return false;
}
//----< return attribute from ProcInstElement >------------------------------

std::vector<std::pair<std::string, std::string>> ProcInstrElement::getAttrib()
{
	return attribs_;
}
//----< generate xml string for text element >-------------------------------

std::string ProcInstrElement::toString()
{
	std::string spacer(tabSize * ++count, ' ');
	std::string xml = "\n" + spacer + "<?" + type_;
	for (auto at : attribs_)
	{
		xml += " ";
		xml += at.first;
		xml += "=\"";
		xml += at.second;
		xml += "\"";
	}
	xml += "?>";
	--count;
	return xml;
}

//----< XmlDeclarElement move constructor >-------------------------------

XmlDeclarElement::XmlDeclarElement(XmlDeclarElement&&decl) : attribs_(std::move(decl.attribs_))
{
}

//----< XmlDeclarElement move assignment operator >-------------------------------

XmlDeclarElement& XmlDeclarElement ::operator=(XmlDeclarElement&&decl)
{
	if (this == &decl)
		return *this;
	delete[] & attribs_;
	attribs_ = std::move(decl.attribs_);
	return *this;
}

//----< generate xml string for Xml declaration element >-------------------------------

std::string XmlDeclarElement::toString()
{
	std::string spacer(tabSize * ++count, ' ');
	std::string xml = "\n" + spacer + "<?xml";
	for (auto at : attribs_)
	{
		xml += " ";
		xml += at.first;
		xml += "=\"";
		xml += at.second;
		xml += "\"";
	}
	xml += "?>";
	--count;
	return xml;
}
//----< add attribute to XmlDeclarElement >-----------------------------------

bool XmlDeclarElement::addAttrib(const std::string& name, const std::string& value)
{
	std::pair<std::string, std::string> atPair(name, value);
	attribs_.push_back(atPair);
	return true;
}
//----< remove attribute from XmlDeclarElement >------------------------------

bool XmlDeclarElement::removeAttrib(const std::string& name)
{
	for (auto iter = begin(attribs_); iter != end(attribs_); ++iter)
	{
		if (iter->first == name)
		{
			attribs_.erase(iter);
			return true;
		}
	}
	return false;
}

//----< return attribute from XmlDeclarElement >------------------------------

std::vector<std::pair<std::string, std::string>> XmlDeclarElement::getAttrib()
{
	return attribs_;
}

//----< comment element move constructor >-------------------------------

CommentElement::CommentElement(CommentElement && ce) :commentText_(std::move(ce.commentText_))
{
}

//----< comment element move assignment operator >-------------------------------
CommentElement &CommentElement:: operator=(CommentElement && ce)
{
	if (this == &ce)
		return *this;
	commentText_ = std::move(ce.commentText_);
	return *this;
}


//----< generate xml string for comment element >-------------------------------

std::string CommentElement::toString()
{
	std::string spacer(tabSize * ++count, ' ');
	std::string xml = "\n" + spacer + "<!--";
	xml += " ";
	xml += commentText_;
	xml += " ";
	
	xml += "-->";
	--count;
	return xml;
}
//----< helper function displays titles >------------------------------------

void XmlProcessing::title(const std::string& title, char underlineChar)
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, underlineChar);
}

//----< test stub >----------------------------------------------------------

#ifdef TEST_XMLELEMENT

int main()
{
	title("Testing XmlElement Package", '=');
	std::cout << "\n";

	using sPtr = std::shared_ptr < AbstractXmlElement >;

	sPtr root = makeTaggedElement("root");
	root->addChild(makeTextElement("this is a test"));

	sPtr child = makeTaggedElement("child");
	child->addChild(makeTextElement("this is another test"));
	child->addAttrib("first", "test");
	root->addChild(child);

	sPtr docEl = makeDocElement(root);
	std::cout << "  " << docEl->toString();
	std::cout << "\n\n";
}

#endif
