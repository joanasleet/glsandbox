MeshType = {
    PLANE = 0,
    CUBE = 1,
    SPHERE = 2,
    CUBEMAP = 3,
    OVERLAY = 4,
    TERRAIN = 5,
    SKYDOME = 6,
    SCREEN_QUAD = 7
}

Uniform = {

	-- world matrix
    P = 0,
    MV = 1,
    MVnoTrans = 2,
    MVP = 3,
    MVPNoTrans = 4,

    -- object matrix
    objMV = 5,
    objMVNoTrans = 6,

    -- camera
    camPos = 7,

    -- misc
    gTime = 8
}

return MeshType, Uniform
