MeshType = require "scripts.MeshType"
Uniform  = require "scripts.Uniform"

scene = {

    objects = {
        {
            name = "plane",
            mesh = {
                size = 1000,
                texres = 20,
                physics = 0,
                position = {0, 10, 0},
                type = MeshType.Plane
            },
            material = {
                "textures/kacheln.png"
            },
            uniformsMap = {
                mv  = Uniform.objMV,
                MVP = Uniform.MVP
            },
            shaders = {
                "shaders/proj_pass.vert",
                "shaders/tex.frag"
            }
        },
        {
            name = "sphere",
            mesh = {
                size = 10,
                texres = 1,
                physics = 0,
                position = { 0, 15, 0 },
                type = MeshType.SphereOut
            },
            material = {
                "textures/earthCM/earthXp.png",
                "textures/earthCM/earthXn.png",
                "textures/earthCM/earthYp.png",
                "textures/earthCM/earthYn.png",
                "textures/earthCM/earthZp.png",
                "textures/earthCM/earthZn.png"
            },
            uniformsMap = {
                mv  = Uniform.objMV,
                MVP = Uniform.MVP
            },
            shaders = {
                "shaders/cubemap.vert",
                "shaders/cubemap.frag"
            }
        },
        {
            name = "sphere",
            mesh = {
                size = 50,
                texres = 1,
                physics = 0,
                position = { 20, 15, 0 },
                type = MeshType.CubeOut
            },
            material = {
                "textures/earthCM/earthXp.png",
                "textures/earthCM/earthXn.png",
                "textures/earthCM/earthYp.png",
                "textures/earthCM/earthYn.png",
                "textures/earthCM/earthZp.png",
                "textures/earthCM/earthZn.png"
            },
            uniformsMap = {
                mv  = Uniform.objMV,
                MVP = Uniform.MVP
            },
            shaders = {
                "shaders/cubemap.vert",
                "shaders/cubemap.frag"
            }
        }
    }
}
