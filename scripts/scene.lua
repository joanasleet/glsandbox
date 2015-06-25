MeshType = require "scripts.MeshType"
Uniform  = require "scripts.Uniform"

scene = {

    objects = {
        --{
        --    mesh = {
        --        type = MeshType.CUBEMAP,
        --        size = 250
        --    },
        --    material = {
        --        "textures/skybox/stars_right.jpg",
        --        "textures/skybox/stars_left.jpg",
        --        "textures/skybox/stars_top.jpg",
        --        "textures/skybox/stars_top.jpg",
        --        "textures/skybox/stars_front.jpg",
        --        "textures/skybox/stars_back.jpg"
        --    },
        --    uniformsMap = {
        --        MVP     = Uniform.MVP
        --    },
        --    shaders = {
        --        "shaders/cubemap.vert",
        --        "shaders/cubemap.frag"
        --    }
        --},
        --{
        --    name = "sky",
        --    mesh = {
        --        type = MeshType.SPHERE,
        --    },
        --    material = {"textures/sky/skymap.png"},
        --    uniformsMap = {
        --        MVP     = Uniform.MVP,
        --        gTime   = Uniform.gTime
        --    },
        --    shaders = {
        --        "shaders/pass.vert",
        --        "shaders/skymap.frag",
        --        "shaders/quad.tcs",
        --        "shaders/sphereIN.tes"
        --    }
        --},
        {
            name = "terrain",
            mesh = {
                type = MeshType.TERRAIN,
                size = 100.0,
                texres = 1.0,
                position = {0.0, 0.0, 0.0}
            },
            material = {
                "textures/shm.png",
                "textures/cliff.jpg",
                "textures/grass.png",
                "textures/nm.png"
            },
            uniformsMap = {
                MVP     = Uniform.MVP
            },
            shaders = {
                "shaders/pass.vert",
                "shaders/terrain.frag",
                "shaders/quad.tcs",
                "shaders/terrain.tes"
            }
        },
        {
            name = "quad",
            mesh = {
                type = MeshType.SCREEN_QUAD,
                size = 1,
                texres = 1,
                position = { 0, 0, 0 }
            },
            material = {},
            uniformsMap = {
                MVP = Uniform.MVP
            },
            shaders = {
                "shaders/debug.vert",
                "shaders/debug.frag"
            }
        }
    }
}
