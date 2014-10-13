VaoType = require "scripts.VaoType"
UniVarFuncType = require "scripts.UniVarFuncType"

scene = {
    {
        name = "ground",
        vaoType = VaoType.PLANE,
        texture = "textures/tiles.png",
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

return scene
