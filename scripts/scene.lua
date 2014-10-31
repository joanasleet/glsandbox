MeshType = require "scripts.MeshType"
UniVarFuncType = require "scripts.UniVarFuncType"

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

scene = {

    camera = {
        fov = 60,
        aspectRatio = 16/9,
        position = {0, 10, 500}
    },

    objects = {
        {
            name = "sphere",
            mesh = {
                type = MeshType.PLANE,
                size = 1000,
                texres = 1,
                position = {0, -50, -500}
            },
            material = {"textures/brick.png"},
            uniforms = {"MVP"},
            uniVarFuncs = {UniVarFuncType.ModelViewPerspective},
            shaders = {"shaders/debug.vert", "shaders/debug.frag"}
        }
    }
}

return scene

-- scene = {
--      camera = {},
--      player = {},
--      objects = {},
--      ???
--}
