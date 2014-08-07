
#include "FbxFileNode.h"

FbxFileNode::FbxFileNode(void)
{
	_property_name = "";
	_primitive_data.clear();
	_sub_property.clear();
}


FbxFileNode::~FbxFileNode(void)
{
}

void FbxFileNode::Destroy()
{
	_property_name = "";
	_primitive_data.clear();
	_sub_property.clear();
}
