VaoType = require "scripts.VaoType"
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
    {
        name = "sphere",
        vaoType = VaoType.SPHERE,
        texture = "textures/brick.png",
        uniforms = {"MVP"},
        uniVarFuncs = {UniVarFuncType.ModelViewPerspective},
        shaders = {"shaders/debug.vert", "shaders/debug.frag"}
    }
}

return scene

-- scene = {
--      camera = {},
--      player = {},
--      objects = {},
--      ???
--}
