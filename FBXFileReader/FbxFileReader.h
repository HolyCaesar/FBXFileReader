#pragma once

#include "CommonHeader.h"
#include "FbxFileNode.h"

// TODO I probably need a new Model class to extract model info from FbxFileReader Class
// Just do everything here by the time being
struct JointInfo{
	string _name;
	long _id;
	float _pre_rotation[3];
	float _lcl_rotation[3];
	float _lcl_translation[3];
	float _lcl_scale[3];
};

struct BindPoseInfo{
	long _id;
	float _matrix[16];
};

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

	void DisplayFbxContent(FbxFileNode* node);

	/*
	* Recursively clear the three
	*/
	void CleanUp();

	/*
	* Getters and Setters
	*/
	FbxFileNode* GetRoot() { return _file_root; }
	vector<JointInfo> GetJointInfo() { return _joint_info_vec; }
	vector<BindPoseInfo> GetBindPoseInfo() { return _bind_pose_info_vec; }

	//////////////////////////////////////////////////////////////

	/*
	* Extract Bone Information
	*/
	void ExtractBoneInfo();

private:
	/*
	* Recursively construct a tree structure for the property
	*/
	void ConstructTreeNode(ifstream &input_file, FbxFileNode* root);

	/*
	* Clean the Tree Structure
	*/
	void CleanSubProperties(FbxFileNode* node);

	/*
	* Locate certain type of property
	*/
	void LocateProperty(FbxFileNode* root, string property_name, vector<FbxFileNode*>& res, bool flag);

private:
	FbxFileNode* _file_root;

	vector<JointInfo> _joint_info_vec;
	vector<BindPoseInfo> _bind_pose_info_vec;
};

