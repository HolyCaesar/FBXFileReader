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

/* Bone information */
void FbxFileReader::ExtractBoneInfo()
{
	vector<FileNode> properties_vec;
	LocateProperty(_file_root, "Model", properties_vec, false);

	for(int i = 0; i < properties_vec.size(); i++) {
		FileNode tmp_property;
		for(int j = 0; j < properties_vec[i]._sub_property.size(); j++) {
			if(properties_vec[i]._sub_property[j]._property_name == "Properties70") {
				tmp_property = properties_vec[i]._sub_property[j];
				break;
			}
		}

		// Load Joint Info
		JointInfo tmp_joint_info;
		memset(&tmp_joint_info, 0, sizeof(tmp_joint_info));
		tmp_joint_info._id = atol(properties_vec[i]._primitive_data[0].c_str());
		for(int j = 0; j < tmp_property._sub_property.size(); j++) {
			string property = tmp_property._sub_property[j]._primitive_data[0];
			// Get PreRotation Data
			if(property == "\"PreRotation\"") {
				int primitive_num = tmp_property._sub_property[j]._primitive_data.size();
				tmp_joint_info._pre_rotation[0] = atof(tmp_property._sub_property[j]._primitive_data[primitive_num - 3].c_str());
				tmp_joint_info._pre_rotation[1] = atof(tmp_property._sub_property[j]._primitive_data[primitive_num - 2].c_str());
				tmp_joint_info._pre_rotation[2] = atof(tmp_property._sub_property[j]._primitive_data[primitive_num - 1].c_str());
			}
			else if(property == "\"Lcl") {
				if(tmp_property._sub_property[j]._primitive_data[1] == "Translation\"") {
					int primitive_num = tmp_property._sub_property[j]._primitive_data.size();
					tmp_joint_info._lcl_translation[0] = atof(tmp_property._sub_property[j]._primitive_data[primitive_num - 3].c_str());
					tmp_joint_info._lcl_translation[1] = atof(tmp_property._sub_property[j]._primitive_data[primitive_num - 2].c_str());
					tmp_joint_info._lcl_translation[2] = atof(tmp_property._sub_property[j]._primitive_data[primitive_num - 1].c_str());
				}
				else if(tmp_property._sub_property[j]._primitive_data[1] == "Translation\"") {
					int primitive_num = tmp_property._sub_property[j]._primitive_data.size();
					tmp_joint_info._lcl_rotation[0] = atof(tmp_property._sub_property[j]._primitive_data[primitive_num - 3].c_str());
					tmp_joint_info._lcl_rotation[1] = atof(tmp_property._sub_property[j]._primitive_data[primitive_num - 2].c_str());
					tmp_joint_info._lcl_rotation[2] = atof(tmp_property._sub_property[j]._primitive_data[primitive_num - 1].c_str());
				}
			}
		}
		_joint_info_vec.push_back(tmp_joint_info);
	}

	// Load Bind Pose Info
	properties_vec.clear();
	LocateProperty(_file_root, "Pose", properties_vec, false);
	BindPoseInfo tmp_bindpose_info;
	for(int i = 0; i < properties_vec.size(); i++) {
		FileNode tmp_property = properties_vec[i];
		for(int j = 0; j < tmp_property._sub_property.size(); j++) {
			if(tmp_property._sub_property[j]._property_name == "PoseNode"){
				FileNode matrix_node = tmp_property._sub_property[j]._sub_property[1]._sub_property[0];
				for(int k = 0; k < matrix_node._primitive_data.size(); k++){
					tmp_bindpose_info._matrix[k] = atof(matrix_node._primitive_data[k].c_str());
				}
				_bind_pose_info_vec.push_back(tmp_bindpose_info);
			}
		}
	}
	char cc;
	cin >> cc;
}

/* Private Functions Definition */

void FbxFileReader::ConstructTreeNode(ifstream &input_file, FileNode* root)
{
	string tmp_line_buffer;
	while(!input_file.eof()) {
		// Read data line by line
		getline(input_file, tmp_line_buffer);

		// Remove \t manually
		string tmp_str = "";
		for(int i = 0; i < tmp_line_buffer.size(); i++) {
			if(tmp_line_buffer[i] == '\t') continue;
			else if(tmp_line_buffer[i] == ',') {
				tmp_str += " ";
				continue;
			}
			tmp_str += tmp_line_buffer[i];
		}
		tmp_line_buffer = tmp_str;

		if(tmp_line_buffer.empty()) continue;
		// Skip the comment part
		if(!tmp_line_buffer.empty() && tmp_line_buffer[0] == ';') continue;
		// If touch '}', this means the end of the block
		if(/*tmp_line_buffer.size() == 1 && */tmp_line_buffer[0] == '}') return;


		// Split the line by delimiter space(' ')
		vector<string> tmp_vec;
		stringstream ss(tmp_line_buffer);
		while(getline(ss, tmp_str, ' ')) {
			if(tmp_str == "") continue;
			tmp_vec.push_back(tmp_str);
		}

		// TEST code
		//cout << tmp_line_buffer << endl;

		// Read the nested property
		if(tmp_vec.size() != 0 && tmp_vec[tmp_vec.size() - 1] == "{") {
			FileNode new_property_node;
			new_property_node._property_name = tmp_vec[0].substr(0, tmp_vec[0].size() - 1);
			for(int i = 1; i < tmp_vec.size() - 1; i++) {
				new_property_node._primitive_data.push_back(tmp_vec[i]);
			}

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
	}
}

void FbxFileReader::LocateProperty(FileNode* root, string property_name, vector<FileNode> &res, bool flag)
{
	if(flag == true) return;

	for(int i = 0; i < root->_sub_property.size(); i++) {
		if(root->_sub_property[i]._property_name == property_name) {
			res.push_back(root->_sub_property[i]);
			flag = true;
		}
		else {
			LocateProperty(&(root->_sub_property[i]), property_name, res, false);
		}
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