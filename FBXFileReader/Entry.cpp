#include "CommonHeader.h"
#include "FbxFileReader.h"

int main()
{
	clock_t start;
	start = clock();

	FbxFileReader reader;
	cout << "Start loading fbx file......" << endl;
	reader.ReadFbxASCIIFile("C:\\Users\\Rjb\\Desktop\\Hand.fbx");
	cout << "Loading is finish! Time elapse " << (clock() - start) / (double)CLOCKS_PER_SEC << " sec." << endl;
	//reader.DisplayFbxContent(reader.GetRoot());

	start = clock();
	cout << "Start loading bone information......" << endl;
	reader.ExtractBoneInfo();
	cout << "Loading bone information is finish! Time elapse " << (clock() - start) / (double)CLOCKS_PER_SEC << " sec." << endl;

	reader.CleanUp();

	cout << "End" << endl;
	char pause_char;
	cin >> pause_char;
	return 0;
}