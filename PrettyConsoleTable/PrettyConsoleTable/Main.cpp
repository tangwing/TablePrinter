#include "ConsoleTable.h"

int main()
{
	ConsoleTable ct("Test Table", 2,2);
	ct.SetColHeader(0, "col1").
		SetColHeader(1, "Col2 is very long");
	ct.Print(3);
	ct.Print(4.6);
	ct.Print("hi");
	ct.Print(6);
	getchar();
	return 0;
}