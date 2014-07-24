#include "CommonHeader.h"
#include "FbxFileReader.h"

int main()
{
	FbxFileReader reader;

	reader.ReadFbxASCIIFile("C:\\Users\\Rjb\\Desktop\\test.txt");

	reader.DisplayFbxContent(reader.GetRoot());

	reader.CleanUp();
	char pause_char;
	cin >> pause_char;
	return 0;
}