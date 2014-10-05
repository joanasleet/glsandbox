VaoType = require "scripts.VaoType"
UniVarFuncType = require "scripts.UniVarFuncType"

scene = {
    {
        name = "ground",
        vaoType = VaoType.PLANE,
        texture = "textures/tiles.png",
        uniforms = {"P", "MV"},
        uniVarFuncs = {UniVarFuncType.Perspective, UniVarFuncType.ModelView},
        shaders = {"tex_light.vert", "tex_light.frag"}
    },
    {
        name = "overlay",
        vaoType = VaoType.OVERLAY,
        texture = "fontsheet/font.png",
        uniforms = {},
        uniVarFuncs = {},
        shaders = {"overlay.vert", "overlay.frag"}
    }
}

return scene
