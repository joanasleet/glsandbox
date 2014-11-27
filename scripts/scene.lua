--[[
scene = {
    {
        name = "ground",
        vaoType = VaoType.PLANE,
        texture = "textures/concrete.jpg",
        uniforms = {"P", "MV"},
        uniVarFuncs = {UniVarFuncType.Perspective, UniVarFuncType.ModelView},
        shaders = {"shaders/tex_light.vert", "shaders/tex_light.frag"}
    },
    {
        name = "overlay",
        vaoType = VaoType.OVERLAY,
        texture = "fontsheet/font.png",
        uniforms = {},
        uniVarFuncs = {},
        shaders = {"shaders/overlay.vert", "shaders/overlay.frag"}
    }
}
]]

-- scene = {
--      camera = {},
--      player = {},
--      objects = {},
--      ???
--}

scene = {

    camera = {
        fov = 60,
        aspectRatio = 16/9,
        position = {0, 0, 10}
    },

    objects = {
        {
            name = "test",
            mesh = {
                type = MeshType.OVERLAY,
                size = 1000,
                texres = 1,
                position = {0, 0, 0}
            },
            material = {"textures/brick.png"},
            uniforms = {"MVP"},
            uniVarFuncs = {UniVarFuncType.ModelViewPerspective},
            shaders = {"shaders/debug.vert", "shaders/debug.frag"}
        },
        {
            mesh = {
                type = MeshType.PLANE,
                size = 1000,
                texres = 100,
                position = {0, 0, 0}
            },
            material = {"textures/tiles.png"},
            uniforms = {"MVP"},
            uniVarFuncs = {UniVarFuncType.ModelViewPerspective},
            shaders = {"shaders/base.vert", "shaders/tex.frag"}
        }
    }
}