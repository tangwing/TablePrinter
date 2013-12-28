//
//  ConsoleTable <https://github.com/tangwing/PrettyConsoleTable>
//  A single-header only C++ library for easy (and pretty, for later versions) display 
//  of tables in console applications.
//
//  Copyright (c) 2013 SHANG Lei <shang.france@gmail.com>
//
//  This library is released under the WTFPL Licence.
//  http://www.wtfpl.net/
//
#ifndef CONSOLETABLE_H
#define CONSOLETABLE_H

#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

///@brief Instantiate this class and config it to represente a table, then use Print() to render table units one by one. 
class ConsoleTable
{
public:
	///@brief The constructor which concern about the title and the structure of the table.
	///@param filename If not empty, ouput is redirected to a file.
	ConsoleTable(string title, int lineCount, int colCount, string filename = ""):
		Title(title),
		LineCount(lineCount),
		ColCount(colCount),
		CurrentEleCount(0),
		HaveLineNumber(true),
		HaveLineDiv(false),
		HaveColNumber(false),
		HaveColDiv(true)
	{
		if(filename.length()==0)
		{
			pOut = &cout;
			EnableExtendedAsciiChar(true);
		}
		else 
		{
			pOut = new ofstream(filename, ofstream::app);
			//It's a pity that normal files don't support extended ascii codes.
			EnableExtendedAsciiChar(false);
		}
		TableWidth = 0;
		LineNumberColWidth = 3;
		ColWidthDefault = 5;
		ostringstream StringBuilder;
		for(int i=0; i<ColCount; i++)
		{
			StringBuilder.str("");
			StringBuilder<<i;
			ColHeaders.push_back(StringBuilder.str());
		}
		ColWidths = vector<int>(ColCount, ColWidthDefault);
	}

	///@brief Set a column header. We will decide the width of each column from its header text length.
	///The other way to set the width of col: @see SetColWidth(int width, int colIndice)
	ConsoleTable & SetColHeader(unsigned int colIndice, string header)
	{
		ColHeaders[colIndice]=(header);
		ColWidths[colIndice]=(header.size());
		return *this;
	}

	///@brief Set the width of a specific column.
	///@param width The new width value.
	///@param colIndice The 0-based indice of the column to set, -1 as default standing for all columns
	ConsoleTable & SetColWidth(unsigned int width, int colIndice = -1)
	{
		if(colIndice > -1 && ColWidths.size() > colIndice)
		{
			ColWidths[colIndice] = width;
		}else if(colIndice == -1) //Same width for all columns
		{
			ColWidthDefault = width;
			ColWidths = vector<int>( ColCount, width);
		}
		return *this;
	}

	///@brief Render the first empty table unit with the specified value.
	template <typename T>
	ConsoleTable & Print(T ele)
	{
		if(CurrentEleCount == 0)
		{//Print table header
			PrintTableHeader();
		}
		int colIndice = CurrentEleCount%ColCount;
		
		if( colIndice == 0)	//New line begins
		{
			*pOut<<endl<<CharColDiv<<setfill(' ');
			if(HaveLineNumber)
				*pOut<<setw(LineNumberColWidth)<< CurrentEleCount/ColCount <<CharColDiv;
		}
		*pOut<<setfill(' ')<<setw(ColWidths[colIndice])<<ele<<CharColDiv;
		CurrentEleCount ++;
		
		if(CurrentEleCount == LineCount*ColCount)	//Finish
		{
			*pOut<<endl<<CharDownLeft<<setfill(CharLineDiv);
			if(HaveLineNumber) *pOut<<setw(LineNumberColWidth+1)<<CharDownMiddle;
			for(int i = 0; i<ColCount-1; i++)
				*pOut<<setw(ColWidths[i]+1)<<CharDownMiddle;
			*pOut<<setw(ColWidths[ColCount-1]+1)<<CharDownRight<<endl<<endl;
		}
		else if( HaveLineDiv && CurrentEleCount%ColCount == 0 )	//line ends, print divider
		{
			*pOut<<endl<<CharMiddleLeft<<setfill(CharLineDiv);
			if(HaveLineNumber) *pOut<<setw(LineNumberColWidth+1)<<CharMiddleMiddle;
			for(int i = 0; i<ColCount-1; i++)
				*pOut<<setw(ColWidths[i]+1)<<CharMiddleMiddle;
			*pOut<<setw(ColWidths[ColCount-1]+1)<<CharMiddleRight;
		}
		return * this;
	}

	///@briet Turn to use plain-old Ascii chars
	void EnableExtendedAsciiChar(bool enable)
	{
		if(enable)
		{
			CharColDiv = (char)179;
			CharLineDiv = (char)196;
			CharUpLeft = (char)218;
			CharUpMiddle = (char)194;
			CharUpRight = (char)191;	
			CharDownLeft = (char)192;
			CharDownMiddle = (char)193;
			CharDownRight = (char)217;
			CharMiddleLeft = (char)195;
			CharMiddleMiddle = (char)197;
			CharMiddleRight = (char)180;
		}
		else
		{
			CharColDiv = '|';
			CharDownLeft = CharDownRight = CharMiddleLeft = CharMiddleMiddle = CharMiddleRight = CharUpLeft = CharUpMiddle = CharUpRight = '+';
			CharLineDiv = CharDownMiddle = '-';
		}

	}

	~ConsoleTable()
	{
		if(pOut != &cout)
			delete pOut;
	}

//____________________ Getters & setters ____________________
	ConsoleTable & SetTitle(string title){Title=(title); return *this;}
	ConsoleTable & SetHaveLineNumber(bool haveLineNumber){HaveLineNumber=(haveLineNumber); return *this;}
	ConsoleTable & SetHaveLineDiv(bool haveLineDiv){HaveLineDiv=(haveLineDiv); return *this;}
	ConsoleTable & SetHaveColNumber(bool haveColNumber){HaveColNumber=(haveColNumber); return *this;}
	ConsoleTable & SetHaveColDiv(bool haveColDiv){HaveColDiv=(haveColDiv); return *this;}
	

//==================== Private zone ====================
private:
	ostream* pOut;
	string Title;
	int LineCount;
	int ColCount;
	int CurrentEleCount;
	int TableWidth;
	int ColWidthDefault;
	int LineNumberColWidth;
	bool HaveLineNumber;
	bool HaveLineDiv ;
	bool HaveColNumber;
	bool HaveColDiv;
	//bool IsColWidthDynamic;
	char CharColDiv ;
	char CharLineDiv ;
	char CharUpLeft ;
	char CharUpMiddle ;
	char CharUpRight ;
	char CharDownLeft ;
	char CharDownMiddle ;
	char CharDownRight ;
	char CharMiddleLeft ;
	char CharMiddleMiddle ;
	char CharMiddleRight ;

	vector<string> ColHeaders;
	vector<int> ColWidths;

	ConsoleTable(ConsoleTable&);
	ConsoleTable & operator=(ConsoleTable&);

	ConsoleTable & PrintTableHeader()
	{
		//First of all, compute the table width
		TableWidth = accumulate(ColWidths.begin(), ColWidths.end(), TableWidth);
		if(HaveColDiv)	TableWidth += ColCount + 1;
		if(HaveLineNumber) TableWidth += LineNumberColWidth + 1;

		int titleMarge = (TableWidth - 2 - (int)Title.length())/2;
		if(titleMarge<1)titleMarge = 1;
		*pOut<<endl<<CharUpLeft<<setfill(CharLineDiv)<<setw(TableWidth-2)<<CharLineDiv<<CharUpRight<<endl;
		*pOut<<CharColDiv<<setfill(' ')<<setw(titleMarge)<<""<< Title <<setw(TableWidth - 1 - titleMarge - (int)Title.length())<<CharColDiv<<endl;
		*pOut<<CharUpLeft<<setfill(CharLineDiv);
		if(HaveLineNumber) *pOut<<setw(LineNumberColWidth+1)<<CharUpMiddle;
		for(int i = 0; i<ColCount-1; i++)
			*pOut<<setw(ColWidths[i]+1)<<CharUpMiddle;
		*pOut<<setw(ColWidths[ColCount-1]+1)<<CharUpRight;
		(*pOut).fill(' ');
		//Print column headers
		*pOut<<endl<<CharColDiv;
		if(HaveLineNumber) *pOut<<setw(LineNumberColWidth)<<""<<CharColDiv;
		for(int i = 0; i<ColCount; i++)
			*pOut<<setw(ColWidths[i])<<ColHeaders[i]<<CharColDiv;
		//Print the divider below the column headers 
		*pOut<<endl<<CharMiddleLeft<<setfill(CharLineDiv);
		if(HaveLineNumber) *pOut<<setw(LineNumberColWidth+1)<<CharMiddleMiddle;
		for(int i = 0; i<ColCount-1; i++)
			*pOut<<setw(ColWidths[i]+1)<<CharMiddleMiddle;
		*pOut<<setw(ColWidths[ColCount-1]+1)<<CharMiddleRight;
		(*pOut).fill(' ');

		return *this;
	}
};
#endif