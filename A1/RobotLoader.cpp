#include "RobotLoader.hpp"

#pragma comment(lib, "lua51")

#include <iostream>

#include "win32\Lua\lua.hpp"

#include "Node.hpp"

using namespace std;

Node* RobotLoader::load(const string& filePath) {
	// Create a lua state and open the standard libs for it
	lua_State* L;
	L = lua_open();
	luaL_openlibs(L);

	// Load the passed file into the lua state
	luaL_dofile(L, filePath.c_str());

	// Get the global robot table which contains the information
	// about the nodes
	lua_getglobal(L, "robot");
	if(lua_isnil(L, -1)) {
		// If there is no robot table, exit
		lua_close(L);
		return nullptr;
	}

	// Get the number of nodes in the table
	int nodeCount = lua_objlen(L, -1);
	if(nodeCount == 0) {
		// If there are no nodes, the model would be empty
		lua_close(L);
		return nullptr;
	}

	// Create the list of node pointers which will be filled in the following
	Node** nodes = new Node*[nodeCount];

	// Iterate over all nodes in the array
	for(int i = 0; i < nodeCount; ++i) {
		// Get the node with the index i+1 from the lua array
		// (lua arrays begin with the value 1)
		lua_pushinteger(L, i+1);
		lua_gettable(L, -2);

		if(!lua_istable(L, -1)) {
			// The nodes are tables themself
			// if the node we just read is no table
			// then to lua file is corrupt
			cout << "RobotLoader: Node " << i+1 << " is no valid lua table" << endl;
			exit(EXIT_FAILURE);
		}

		// Receive all needed information from the current node
		lua_getfield(L, -1, "x");
		lua_getfield(L, -2, "y");
		lua_getfield(L, -3, "z");
		lua_getfield(L, -4, "length");
		lua_getfield(L, -5, "height");
		lua_getfield(L, -6, "width");
		lua_getfield(L, -7, "jointx");
		lua_getfield(L, -8, "jointy");
		lua_getfield(L, -9, "jointz");
		lua_getfield(L, -10, "rotx");
		lua_getfield(L, -11, "roty");
		lua_getfield(L, -12, "rotz");
		lua_getfield(L, -13, "parent");

		// Transfer the read data to a new node instance
		Node* currentNode = new Node(static_cast<float>(lua_tonumber(L, -13)), 
									 static_cast<float>(lua_tonumber(L, -12)),
									 static_cast<float>(lua_tonumber(L, -11)),
									 static_cast<float>(lua_tonumber(L, -10)),
									 static_cast<float>(lua_tonumber(L, -9)),
									 static_cast<float>(lua_tonumber(L, -8)),
									 static_cast<float>(lua_tonumber(L, -7)),
									 static_cast<float>(lua_tonumber(L, -6)),
									 static_cast<float>(lua_tonumber(L, -5)),
									 static_cast<float>(lua_tonumber(L, -4)),
									 static_cast<float>(lua_tonumber(L, -3)),
									 static_cast<float>(lua_tonumber(L, -2)),
									 i);

		if(!lua_isnil(L, -1)) {
			// There is a parent number specified
			int parent = lua_tointeger(L, -1);

			// Check if the parent number is valid for the current node
			if(parent >= i + 1 || parent < 1) {
				cout << "RobotLoader: Invalid parent number on node " << i+1 << endl;
				exit(EXIT_FAILURE);
			}

			// Set the correct parent for the current node
			currentNode->setParent(nodes[parent - 1]);
		} else {
			if(i > 0) {
				// There are two parent nodes specified in the lua file
				// this is obviously an error since it can cause memory leaks (only one node is returned)
				cout << "RobotLoader: Only one parent node is allowed per model" << endl;
				exit(EXIT_FAILURE);
			}
		}

		// Pop all values of the current node from the lua stack
		lua_pop(L, 14);

		// Insert the current node in the list of nodes
		nodes[i] = currentNode;
	}

	// Get the root node
	Node* root = nodes[0];

	// Delete the list of pointers
	delete[] nodes;

	// Clear the lua stack
	lua_pop(L, 1);

	// Close the lua state
	lua_close(L);

	return root;
}