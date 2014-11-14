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

jointDiam = 60

robot = {}
-- Torso
robot[1] = node(0, 0, 0,
				200, 300, 100,
				0, 0, 0,
				0, 0, 0, nil)
-- Right upper arm
robot[2] = node(210, 125, 0, -- x, y, z
				100, 50, 50, -- length, height, width
				-80, 0, 0,   -- jointx, jointy, jointz
				0, 0, 0, 1)  -- rotx, roty, rotz, parentID
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
-- Head
robot[6] = node(0, 150 + jointDiam + 40, 0,
				100, 80, 50,
				0, -40 - jointDiam/2, 0,
				0, 0, 0, 1)

-- Right upper leg
robot[7] = node(100 - 35, -150 - jointDiam - 50, 0,
				70, 100, 70,
				0, 50 + jointDiam/2, 0,
				0, 0, 0, 1)

-- Right lower leg
robot[8] = node(0, -50 - jointDiam - 40, 0,
				60, 80, 60,
				0, 40 + jointDiam/2, 0,
				0, 0, 0, 7)

-- Left upper leg
robot[9] = node(-100 + 35, -150 - jointDiam - 50, 0,
				70, 100, 70,
				0, 50 + jointDiam/2, 0,
				0, 0, 0, 1)

-- Left lower leg
robot[10] = node(0, -50 - jointDiam - 40, 0,
				60, 80, 60,
				0, 40 + jointDiam/2, 0,
				0, 0, 0, 9)

