VaoType = require "scripts.VaoType"

scene = {
    {
        name = "skybox",
        vaoType = VaoType.CUBEMAP,
        texture = "asd.png",
        uniforms = {"A", "B", "C"}
    },
    {
        name = "terrain",
        vaoType = VaoType.PLANE,
        texture = "asd.png",
        uniforms = {"A", "B", "C"}
    },
    {
        name = "cube",
        vaoType = VaoType.CUBE,
        texture = "asd.png",
        uniforms = {"A", "B", "C"}
    }
}

return scene
