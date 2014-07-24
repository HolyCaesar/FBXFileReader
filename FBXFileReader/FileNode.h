#pragma once

#include "CommonHeader.h"

class FileNode
{
public:
	FileNode(void);
	~FileNode(void);

	void Destroy();

public:
	string _property_name;
	vector<string> _primitive_data;
	vector<FileNode> _sub_property;
};

