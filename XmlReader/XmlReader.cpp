///////////////////////////////////////////////////////////////////////
// XmlReader.cpp - Parse XML strings                                    //
// ver 2.3                                                            //
// Application: CSE687 - Project 2 (XML Document Model), Spring 2015  //
// Platform:    HP G62, Windows 7 Home Premium, Visual Studio 2013    //
// Author:      Jim Fawcett, CST 4-187, 443-3948                      //
//              jfawcett@twcny.rr.com                                 //
//Modified By:  Dhivya Narayanan, Master's- Computer Engineering,     //
//			    Syracuse University,                                  //
//              dnnaraya@syr.edu                                      //
////////////////////////////////////////////////////////////////////////

#include "XmlReader.h"
#include <ctype.h>
#include <stack>
#include <algorithm>
#include<iostream>
#include<fstream>
#include<string>

using namespace std;

//-------------< Promotion constructor >---------------------------------------

XmlReader::XmlReader(const std::string& xmlstr): _xml(xmlstr), _position(0), localposition(0)
{
}
 
std::string XmlReader::srcxml()
{
	return _xml;
}
//----< helper identifies markup chars >-----------------------------

bool specialChar(char ch)
{
	bool test = isspace(ch) || ch == '/' || ch == '>' || ch == '<' || ch == '=';
	test = test || ch == '\'' || ch == '"';
	return test;
}
//----< helper finds identifiers >-----------------------------------

std::string XmlReader::extractIdentifier(size_t& pos, std::string& xmlstr)
{
	std::string ident;
	while (true)
	{
		if (pos == xmlstr.size())
			return ident;
		char ch = xmlstr[pos];
		if (specialChar(ch))
			++pos;
		else
			break;
	}
	while (true)
	{
		char ch = xmlstr[pos];
		if (specialChar(ch))
			break;
		ident += ch;
		++pos;
	}
	return ident;
}
//----< return current element string >------------------------------

std::string XmlReader::element()
{
	localposition = _position;
	_tag = extractIdentifier(localposition,_xml);	
	std::string decTag = "?xml";	// is declaration? or is procInstruction?
	if (_tag.compare("?xml") == 0 || _tag[0]=='?')
	{
		size_t locpos = _xml.find("?>");
		return _xml.substr(_position - 1, locpos - _position + 3);
	}
	if (_tag.compare("!--") == 0)		// is comment?
	{
		size_t locpos = _xml.find("-->");
		return _xml.substr(_position - 1, locpos - _position + 4);
	}
	size_t locpos1 = localposition;
	std::stack<std::string> tagStack;
	tagStack.push(_tag);
	while (true)
	{
		locpos1 = _xml.find(_tag, locpos1);
		if (locpos1 >= _xml.size())
			break;
		if (_xml[locpos1 - 1] == '/' && _xml[locpos1 + _tag.size()] == '>')  
			tagStack.pop();
		else if (_xml[locpos1 - 1] == '<')
			tagStack.push(_tag);
		if (tagStack.size() == 0)
			break;
		++locpos1;
	}
	size_t locpos2 = _xml.find(">", localposition);
	if (_xml[locpos2 - 1] != '/')
		locpos2 = _xml.size();
	localposition = std::min(locpos1, locpos2);
	if (localposition >= _xml.size())
		throw std::exception("malformed XML");
	if (localposition == locpos1)
	{
		localposition = _xml.find('>', localposition);
		return _xml.substr(_position - 1, localposition - _position + 2);
	}
	return _xml.substr(_position - 1, localposition - _position + 2);
}
//----< return body string >-----------------------------------------

std::string XmlReader::body()
{
	if (_tag.compare("?xml") == 0 || _tag.compare("!--") == 0 || _tag[0]=='?')
	{
		return "";
	}
	std::string elem = element();
	size_t locpos1 = elem.find('>');
	if (locpos1 >= elem.size())
		throw std::exception("malformed XML");
	if (elem[locpos1 - 1] == '/')
		return "";
	std::string localtag = tag();
	size_t locpos2 = elem.find_last_of("</");
	if (locpos2 < elem.size())
		return elem.substr(locpos1 + 1, locpos2 - locpos1 - 2);
	return "";
}

//----< return text and child string >-----------------------------------------

XmlReader::text_childElems XmlReader::text_and_child()
{
	_children.clear();
	_text.clear();
	if (_tag.compare("?xml") == 0 || _tag.compare("!--") == 0 || _tag[0] == '?')  //is declaration or comment or procInstr?
	{
		XmlReader::text_childElems textchild;
		textchild.first = _text;
		textchild.second = _children;
		return textchild;
	}
	std::string tag_body = body();	
	size_t str_pos = 0;
	size_t pos = tag_body.find('<', str_pos);
	if (pos > tag_body.size() && tag_body.size() > 0)
		_text.push_back(tag_body);
	std::stack<std::string> childStack;
	while (str_pos < tag_body.size())      //tagged element
	{
		size_t pos = tag_body.find('<', str_pos);
		char ch = tag_body[str_pos];
		while (isspace(tag_body[str_pos]))
			str_pos++;
		char cht = tag_body[str_pos];		
		if (pos >= tag_body.size())
			break;
		if (pos == str_pos)					//is child tagged element?
		{
			++pos;
			size_t tmp_pos = pos;
			std::string child = extractIdentifier(tmp_pos,tag_body);
			size_t tpos = str_pos;
			size_t lastpos = tag_body.find('>', pos);
			
			if (tag_body[lastpos - 1] == '/')
			{		
				if (childStack.empty())
				{
					_children.push_back(child);
				}		
				str_pos = pos+ child.size();
				str_pos = str_pos + 3;
			}
			else if (tag_body[pos] == '/')
			{
				std::string tmp_child = childStack.top();
				if (tmp_child.compare(child) == 0)
				{
					childStack.pop();
					if (childStack.empty())
					  _children.push_back(tmp_child);
					
				}			
				str_pos = str_pos + child.size()+3;
			}
			else if (tpos + child.size() < lastpos || isspace(tag_body[tpos + child.size()]))
			{
				childStack.push(child);
				str_pos = str_pos + child.size() + 2;
				str_pos = lastpos+1;
			}
			else
			{
				childStack.push(child);
				str_pos =str_pos+ child.size()+2;
			}			
		}
		else if (pos > str_pos)       //is Child text element?
		{
			std::string temp_text ;			
			while (str_pos < pos)
			{
				std::string tmp = extractIdentifier(str_pos, tag_body);
				temp_text += tmp;
				temp_text += " ";
				while (isspace(tag_body[str_pos]))
					str_pos++;
			}
			if (childStack.empty())
				_text.push_back(temp_text);
		}
		char chtt = tag_body[str_pos];
	}
	std::pair<std::vector<std::string>,std::vector<std::string>> textchildpair;
	textchildpair.first = _text;
	textchildpair.second = _children;
	return textchildpair;

}
//----< move to next XML tag >---------------------------------------

bool XmlReader::next()
{
	while (true)
	{
		_position = _xml.find('<', _position);
		if (_position >= _xml.size())
			return false;
		++_position;
		if (_xml[_position] != '/')
			break;
	}
	return true;
}
//----< return tag string >------------------------------------------

std::string XmlReader::tag()
{
	localposition = _position;
	return extractIdentifier(localposition,_xml);
}

std::string XmlReader::comment(std::string&elem)
{
	std::string cmt;
	size_t pos = elem.find('<');
	if (elem[1] == '!'&&elem[2] == '-'&&elem[3] == '-')
	{
		pos = pos + 4;
		while (isspace(elem[pos]))
			pos++;
		while (pos < elem.size() - 3)
		{
			std::string tmpstr = extractIdentifier(pos, elem);
			cmt += tmpstr;
			cmt += " ";
			while (isspace(elem[pos]))
				pos++;
		}
	}
	return cmt;
}
//----< return vector of attributes >--------------------------------

XmlReader::attribElems XmlReader::attributes()
{
	_attributes.clear();
	if (_tag.compare("!--") == 0)
		return _attributes;
	if (_tag.compare("?xml") == 0 || _tag[0]=='?')
	{
		localposition = _position;
		extractIdentifier(localposition,_xml);
		size_t locpos = _xml.find('?>', localposition);
		while (true)
		{
			std::string name = extractIdentifier(localposition,_xml);

			if (locpos < localposition || name == "?")
				return _attributes;
			std::string value = extractIdentifier(localposition,_xml);
			if (locpos < localposition)
				throw std::exception("malformed XML");
			std::pair<std::string, std::string> elem;
			elem.first = name;
			elem.second = value;
			_attributes.push_back(elem);
		}
	}
	localposition = _position;
	extractIdentifier(localposition,_xml);

	size_t locpos = _xml.find('>', localposition);
	while (true)
	{
		std::string name = extractIdentifier(localposition,_xml);
		if (locpos < localposition)
			return _attributes;
		std::string value = extractIdentifier(localposition,_xml);
		if (locpos < localposition)
			throw std::exception("malformed XML");
		std::pair<std::string, std::string> elem;
		elem.first = name;
		elem.second = value;
		_attributes.push_back(elem);
	}
	return _attributes;
}
//----< reposition string index >------------------------------------

bool XmlReader::position(size_t p)
{
	if (p < 0 || _xml.size() <= p)
		return false;
	_position = p;
	return true;
}
//----< test stub >--------------------------------------------------

#ifdef TEST_XMLREADER
#include <iostream>

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing XmlReader";
	std::cout << "\n ===================\n";

	if (argc < 3)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}

	CmdLineParser cmdlp(argv[1], argv[2]);
	std::string src = cmdlp.XmlString();
	XmlReader rdr(src);

	std::cout << "\n  source = "  << std::endl;
	std::cout << rdr.srcxml();

	std::cout << "\n  Testing extractIdentifier";
	std::cout << "\n ---------------------------";
	std::string ident;
	size_t position = 1;

	std::cout << "\n  testing next(), tag(), element(), body(), and attributes()";
	std::cout << "\n -----------------------------------------------------------";
	rdr.reset();
	int count = 1;
	while (rdr.next())
	{
		std::cout << "\n  tag " <<count<<":     " << rdr.tag().c_str();
		std::string elem = rdr.element();
		std::cout << "\n  element " << count << ":     " << elem;
		std::string cmt = rdr.comment(elem);
		std::cout << "\n Comment:  " << cmt;
		XmlReader::attribElems attribs = rdr.attributes();
		for (size_t i = 0; i<attribs.size(); ++i)
			std::cout << "\n  attributes: " << attribs[i].first.c_str() << ", " << attribs[i].second.c_str();
		XmlReader::text_childElems text_child = rdr.text_and_child();
		std::vector<std::string> textbody = text_child.first;

		std::cout << "\n  Textnode:    " ;
		for (int i = 0; i < textbody.size(); i++)
		{
			std::cout << textbody[i] << "\n";
		}
		std::cout << "Child node:  ";
		std::vector<std::string> childelem = text_child.second;
		for (int i = 0; i < childelem.size(); i++)
		{
			std::cout << childelem[i] << "\n";
		}
		

		count++;
	}
	std::cout << "\n\n";

	return 0;
}
#endif

