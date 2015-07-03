MeshType = require "scripts.MeshType"
Uniform  = require "scripts.Uniform"

scene = {

    objects = {
        --{
        --    name = "atmosphere",
        --    mesh = {
        --        type = MeshType.SPHERE,
        --        size = 1,
        --        texres = 1,
        --        position = {0, 0, 0}
        --    },
        --    material = {},
        --    uniformsMap = {
        --        gTime       = Uniform.gTime,
        --        MVP         = Uniform.MVP,
        --        v3CameraPos = Uniform.camPos
        --    },
        --    shaders = {
        --        "shaders/pass.vert",
        --        "shaders/sphereAtmoScat.frag",
        --        "shaders/quad.tcs",
        --        "shaders/sphereAtmoScat.tes"
        --    }
        --},
        {
            name = "earth",
            mesh = {
                type = MeshType.SPHERE,
                size = 1,
                texres = 1,
                position = {0, 0, 0}
            },
            material = {
                --"textures/grass.png",
                --"textures/cliff.jpg",
                --"textures/multitex.png"
            },
            uniformsMap = {
                MVP = Uniform.MVP
            },
            shaders = {
                "shaders/debug.vert",
                "shaders/debug.frag"
            }
        }
        --{
        --    name = "space",
        --    mesh = {
        --        type = MeshType.CUBEIN,
        --        size = 1000,
        --        texres = 1,
        --        position = {0, 0, 0}
        --    },
        --    material = {
        --        "textures/space/jajspace2_right.jpg",
        --        "textures/space/jajspace2_left.jpg",
        --        "textures/space/jajspace2_top.jpg",
        --        "textures/space/jajspace2_top.jpg",
        --        "textures/space/jajspace2_front.jpg",
        --        "textures/space/jajspace2_back.jpg"
        --    },
        --    uniformsMap = {
        --        MVP = Uniform.MVPNoTrans
        --    },
        --    shaders = {
        --        "shaders/cubemap.vert",
        --        "shaders/cubemap.frag"
        --    }
        --}
        -- {
        --     name = "terrain",
        --     mesh = {
        --         type = MeshType.TERRAIN,
        --         size = 2,
        --         texres = 25,
        --         position = {0, 0, 0}
        --     },
        --     material = {"textures/ground.jpg", "textures/hm3.png"},
        --     uniforms = {"MVP"},
        --     uniVarFuncs = {Uniform.ModelViewPerspective},
        --     shaders = {
        --         "shaders/terrain.vert",
        --         "shaders/terrain.frag",
        --         "shaders/terrain.tcs",
        --         "shaders/terrain.tes"
        --     }
        -- }
    }
}
