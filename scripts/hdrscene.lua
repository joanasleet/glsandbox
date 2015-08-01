MeshType = require "scripts.MeshType"
Uniform  = require "scripts.Uniform"

scene = {

    textures = {
        {
            name = "earth",
            mesh = {
                type = MeshType.SphereOut,
                size = 10,
                texres = 1,
                position = {0, 0, 0}
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
                MVP = Uniform.MVP
            },
            shaders = {
                "shaders/cubemap.vert",
                "shaders/cubemap.frag"
            }
        },
        {
            name = "space",
            mesh = {
                type = MeshType.CubeIn,
                size = 1000,
                texres = 1,
                position = {0, 0, 0}
            },
            material = {
                "textures/space/jajspace2_right.jpg",
                "textures/space/jajspace2_left.jpg",
                "textures/space/jajspace2_top.jpg",
                "textures/space/jajspace2_top.jpg",
                "textures/space/jajspace2_front.jpg",
                "textures/space/jajspace2_back.jpg"
            },
            uniformsMap = {
                MVP = Uniform.MVPNoTrans
            },
            shaders = {
                "shaders/cubemap.vert",
                "shaders/cubemap.frag"
            }
        }
    },

    objects = {
        {
            name = "ScreenQuad",
            mesh = {
                type     = MeshType.Overlay,
                size     = 1,
                texres   = 1,
                position = { 0, 0, 0 }
            },
            material = { 
                "space"
            },
            shaders = {
                "shaders/proj_pass.vert",
                "shaders/tex.frag"
            },
            uniformsMap = {
                MVP = Uniform.MVP
            }
        }
    }
}
