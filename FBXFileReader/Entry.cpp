#include "CommonHeader.h"
#include "FbxFileReader.h"

int main()
{
	FbxFileReader reader;

	reader.ReadFbxASCIIFile("test.txt");

	//reader.DisplayFbxContent(reader.GetRoot());

	reader.ExtractBoneInfo();

	reader.CleanUp();
	char pause_char;
	cin >> pause_char;
	return 0;
}
