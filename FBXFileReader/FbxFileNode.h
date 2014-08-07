#pragma once

#include "CommonHeader.h"

class FbxFileNode
{
public:
	FbxFileNode(void);
	~FbxFileNode(void);

	void Destroy();

public:
	string _property_name;
	vector<string> _primitive_data;
	vector<FbxFileNode> _sub_property;
};

