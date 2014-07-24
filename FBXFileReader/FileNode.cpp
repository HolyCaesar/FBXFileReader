#include "FileNode.h"


FileNode::FileNode(void)
{
	_property_name = "";
	_primitive_data.clear();
	_sub_property.clear();
}


FileNode::~FileNode(void)
{
}

void FileNode::Destroy()
{
	_property_name = "";
	_primitive_data.clear();
	_sub_property.clear();
}
