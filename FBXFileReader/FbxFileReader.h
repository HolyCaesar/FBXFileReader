#pragma once
#pragma once

#include "CommonHeader.h"
#include "FileNode.h"

class FbxFileReader
{
public:
	FbxFileReader(void);
	~FbxFileReader(void);

	public:
	/*
	* Since Fbx files are stored in a XML way, so, a multi-branches will be constructed here to form a hierarchical structures
	* that represent the logical relationship in Fbx files.
	*/
	void ReadFbxASCIIFile(const string file_path);

	void DisplayFbxContent(FileNode* node);

	/*
	* Recursively clear the three
	*/
	void CleanUp();

	/*
	* Getters and Setters
	*/
	FileNode* GetRoot() { return _file_root; }

private:
	/*
	* Recursively construct a tree structure for the property
	*/
	void ConstructTreeNode(ifstream &input_file, FileNode* root);

	/*
	* Clean the Tree Structure
	*/
	void CleanSubProperties(FileNode* node);

private:
	FileNode* _file_root;
};

