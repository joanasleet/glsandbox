MeshType = require "scripts.MeshType"
Uniform  = require "scripts.Uniform"

scene = {

    objects = {
        --{
        --    name = "skybox",
        --    mesh = {
        --        type = MeshType.CubeIn,
        --        size = 1000,
        --        texres = 1,
        --        position = {0, 0, 0}
        --    },
        --    material = {
        --        --"textures/sunclouds/xp.png",
        --        --"textures/sunclouds/xn.png",
        --        --"textures/sunclouds/yp.png",
        --        --"textures/sunclouds/yn.png",
        --        --"textures/sunclouds/zp.png",
        --        --"textures/sunclouds/zn.png"
        --        "textures/dummy.png",
        --        "textures/dummy.png",
        --        "textures/dummy.png",
        --        "textures/dummy.png",
        --        "textures/dummy.png",
        --        "textures/dummy.png"
        --    },
        --    uniformsMap = {
        --        MVP = Uniform.MVPNoTrans
        --    },
        --    shaders = {
        --        "shaders/cubemap.vert",
        --        "shaders/cubemap.frag"
        --    }
        --},
        {
            name = "plane",
            mesh = {
                size = 1000,
                texres = 20,
                physics = 1,
                position = {0, 5, 0},
                type = MeshType.Plane
            },
            material = {
                "textures/grid.png"
            },
            uniformsMap = {
                --mv  = Uniform.objMV,
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
                physics = 1,
                position = { 0, 5, 0 },
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
                MVP = Uniform.MVP,
                mv  = Uniform.objMV
            },
            shaders = {
                "shaders/cubemap.vert",
                "shaders/cubemap.frag"
            }
        }
    }
}
