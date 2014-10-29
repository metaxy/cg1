#pragma once

#include <string>

class Node;

class RobotLoader {
public:
	/* Static method to load a robot model from a lua script file
	@param filePath: The relative path to the *.lua file
	*/
	static Node* load(const std::string& filePath);
};