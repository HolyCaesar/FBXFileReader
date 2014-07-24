#include "FbxFileReader.h"


FbxFileReader::FbxFileReader(void)
{
	_file_root = new FileNode();
	_file_root->_property_name = "Root";
	_file_root->_sub_property.clear();
	_file_root->_primitive_data.clear();
}


FbxFileReader::~FbxFileReader(void)
{
}

void FbxFileReader::ReadFbxASCIIFile(const string file_path)
{
	ifstream input_file;
	input_file.open(file_path.c_str(), ios::in);

	if(!input_file.is_open()){
		string error_msg = "Can't Open File " + file_path;
		cout << error_msg << endl;
		exit(0);
	}

	ConstructTreeNode(input_file, _file_root);
	input_file.close();
}

void FbxFileReader::DisplayFbxContent(FileNode* node)
{
	if(node->_property_name == "") return;

	cout << "Property name is " << node->_property_name << ": " << endl;
	cout << "Primitive data are ";
	for(int i = 0; i < node->_primitive_data.size(); i++) {
		cout << node->_primitive_data[i] << " ";
	}
	cout << endl;

	for(int i = 0; i < node->_sub_property.size(); i++) {
		DisplayFbxContent(&(node->_sub_property[i]));
	}
}

void FbxFileReader::CleanUp()
{
	CleanSubProperties(_file_root);
}

/* Private Functions Definition */

void FbxFileReader::ConstructTreeNode(ifstream &input_file, FileNode* root)
{
	string tmp_line_buffer;
	while(!input_file.eof()) {
		// Read data line by line
		getline(input_file, tmp_line_buffer);

		if(tmp_line_buffer.empty()) continue;
		// Skip the comment part
		if(!tmp_line_buffer.empty() && tmp_line_buffer[0] == ';') continue;
		// If touch '}', this means the end of the block
		if(tmp_line_buffer.size() == 1 && tmp_line_buffer == "}") return;


		// Split the line by delimiter space(' ')
		vector<string> tmp_vec;
		string tmp_str;
		stringstream ss(tmp_line_buffer);
		while(getline(ss, tmp_str, ' ')) {
			if(tmp_str == "") continue;
			tmp_vec.push_back(tmp_str);
		}

		// Remove /t manually
		if(tmp_vec.size() != 0 && tmp_vec[0].size() != 0) {
			tmp_str = "";
			for(int i = 0; i < tmp_vec[0].size(); i++) {
				if(tmp_vec[0][i] == '\t') continue;
				tmp_str += tmp_vec[0][i];
			}
			tmp_vec[0] = tmp_str;
		}

		cout << tmp_line_buffer << endl;

		// Read the nested property
		if(tmp_vec.size() != 0 && tmp_vec[tmp_vec.size() - 1] == "{") {
			FileNode new_property_node;
			new_property_node._property_name = tmp_vec[0].substr(0, tmp_vec[0].size() - 1);
			ConstructTreeNode(input_file, &new_property_node);
			root->_sub_property.push_back(new_property_node);
			continue;
		}

		// Read the property
		FileNode new_property_node;
		new_property_node._property_name = tmp_vec[0].substr(0, tmp_vec[0].size() - 1);
		for(int i = 1; i < tmp_vec.size(); i++) {
			new_property_node._primitive_data.push_back(tmp_vec[i]);
		}
		root->_sub_property.push_back(new_property_node);
		
		//cout << tmp_line_buffer << endl;
	}
}

void FbxFileReader::CleanSubProperties(FileNode* sub_property)
{
	if(sub_property->_sub_property.size() == 0) return;

	for(int i = 0; i < sub_property->_sub_property.size(); i++) {
		CleanSubProperties(&(sub_property->_sub_property[i]));
	}
	sub_property->Destroy();
}