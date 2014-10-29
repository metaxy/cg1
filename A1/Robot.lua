function node(x, y, z,
			  length, height, width,
			  jointx, jointy, jointz,
			  rotx, roty, rotz,
			  parent) 
	local ret = {}
	ret.x = x
	ret.y = y
	ret.z = z
	ret.length = length
	ret.height = height
	ret.width = width
	ret.jointx = jointx
	ret.jointy = jointy
	ret.jointz = jointz
	ret.rotx = rotx
	ret.roty = roty
	ret.rotz = rotz
	ret.parent = parent	
	return ret
end

robot = {}
-- Torso
robot[1] = node(0, 0, 0,
				200, 300, 100,
				0, 0, 0,
				0, 0, 0, nil)
-- Right upper arm
robot[2] = node(210, 125, 0, -- x, y, z
				100, 50, 50, -- "
				-80, 0, 0,
				0, 0, 0, 1)
-- Left upper arm
robot[3] = node(-210, 125, 0,
				100, 50, 50,
				80, 0, 0,
				0, 0, 0, 1)
-- Right lower arm
robot[4] = node(145, 0, 0,
				70, 40, 40,
				-65, 0, 0,
				0, 0, 0, 2)
-- Left lower arm
robot[5] = node(-145, 0, 0,
				70, 40, 40,
				65, 0, 0,
				0, 0, 0, 3)