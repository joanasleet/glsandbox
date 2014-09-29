VaoType = require "scripts.VaoType"

scene = {
    {
        name = "skybox",
        vaoType = VaoType.CUBEMAP,
        texture = "asd.png",
        uniforms = {"A", "B", "C"},
        shaders = {"overlay.vert", "overlay.frag"}
    }
}

return scene
