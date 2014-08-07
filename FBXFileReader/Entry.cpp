#include "CommonHeader.h"
//#include "FbxFileReader.h"
#include "RjbReader.h"

void dfs(FileNode *root) {
	if (root == NULL) return;

	cout << root->_property_name << ": ";
	for (int i=0; i<(int)root->_primitive_data.size(); i++) cout << root->_primitive_data[i] << ' ';
	
	for (int i=0; i<(int)root->_sub_property.size(); i++)
		dfs(root->_sub_property[i]);
}

int main()
{
	/*
	FbxFileReader reader;

	reader.ReadFbxASCIIFile("test.txt");

	//reader.DisplayFbxContent(reader.GetRoot());

	reader.ExtractBoneInfo();

	reader.CleanUp();
	char pause_char;
	cin >> pause_char;
	*/


	RjbReader reader("Hand.fbx");
	FileNode *file = reader.readFile();

	dfs(file);

	return 0;
}
