//
//  ConsoleTable v0.0
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
	ConsoleTable(string title, int lineCount, int colCount):
		Title(title),
		LineCount(lineCount),
		ColCount(colCount),
		CurrentEleCount(0),
		HaveLineNumber(true),
		HaveLineDiv(false),
		HaveColNumber(false),
		HaveColDiv(true),
		//IsColWidthDynamic(true),
		ColDivChar('|'),
		LineDivChar('-')
	{
		TableWidth = 0;
		LineNumberColWidth = 3;
		ColWidthDefault = 5;
		ColHeaders = vector<string>(ColCount, "");
		ColWidths = vector<int>(ColCount, ColWidthDefault);
	}

	///@brief Set a column header. We will decide the width of each column from its header text length.
	///The other way to set the width of col: @see SetColWidth(int width, int colIndice)
	ConsoleTable & SetColHeader(int colIndice, string header)
	{
		ColHeaders[colIndice]=(header);
		ColWidths[colIndice]=(header.size());
		return *this;
	}

	///@brief Set the width of a specific column.
	///@param width The new width value.
	///@param colIndice The 0-based indice of the column to set, -1 as default standing for all columns
	ConsoleTable & SetColWidth(int width, int colIndice = -1)
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
			cout<<endl<<ColDivChar<<setfill(' ');
			if(HaveLineNumber)
				cout<<setw(LineNumberColWidth)<< CurrentEleCount/ColCount <<ColDivChar;
		}
		cout<<setfill(' ')<<setw(ColWidths[colIndice])<<ele<<ColDivChar;
		CurrentEleCount ++;
		
		if( HaveLineDiv && CurrentEleCount%ColCount == 0 && CurrentEleCount != LineCount*ColCount)	//line ends, print divider
		{
			cout<<endl<<'+'<<setfill(LineDivChar);
			if(HaveLineNumber) cout<<setw(LineNumberColWidth+1)<<'+';
			for(int i = 0; i<ColCount; i++)
				cout<<setw(ColWidths[i]+1)<<'+';
		}
		if(CurrentEleCount == LineCount*ColCount)	//Finish
			cout<<endl<<'+'<<setfill(LineDivChar)<<setw(TableWidth-1)<<'+'<<endl;
		return * this;
	}

//____________________ Getters & setters ____________________
	ConsoleTable & SetTitle(string title){Title=(title); return *this;}
	ConsoleTable & SetHaveLineNumber(bool haveLineNumber){HaveLineNumber=(haveLineNumber); return *this;}
	ConsoleTable & SetHaveLineDiv(bool haveLineDiv){HaveLineDiv=(haveLineDiv); return *this;}
	ConsoleTable & SetHaveColNumber(bool haveColNumber){HaveColNumber=(haveColNumber); return *this;}
	ConsoleTable & SetHaveColDiv(bool haveColDiv){HaveColDiv=(haveColDiv); return *this;}
	ConsoleTable & SetColDivChar(char colDiv){ColDivChar=colDiv; return *this;}
	ConsoleTable & SetLineDivChar(char lineDiv){LineDivChar=lineDiv; return *this;}
	

//==================== Private zone ====================
private:
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
	char ColDivChar;
	char LineDivChar;

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
		cout<<'+'<<setfill(LineDivChar)<<setw(TableWidth-2)<<LineDivChar<<'+'<<endl;
		cout<<ColDivChar<<setfill(' ')<<setw(titleMarge)<<""<< Title <<setw(TableWidth - 1 - titleMarge - (int)Title.length())<<ColDivChar<<endl;
		cout<<'+'<<setfill(LineDivChar)<<setw(TableWidth-1)<<'+';
		cout.fill(' ');
		//Print column headers
		cout<<endl<<ColDivChar;
		if(HaveLineNumber) cout<<setw(LineNumberColWidth)<<""<<ColDivChar;
		for(int i = 0; i<ColCount; i++)
			cout<<setw(ColWidths[i])<<ColHeaders[i]<<ColDivChar;
		//Print the divider below the column headers 
		cout<<endl<<'+'<<setfill(LineDivChar);
		if(HaveLineNumber) cout<<setw(LineNumberColWidth+1)<<'+';
		for(int i = 0; i<ColCount; i++)
			cout<<setw(ColWidths[i]+1)<<'+';
		cout.fill(' ');

		return *this;
	}
};
#endif