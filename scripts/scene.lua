scene = {

    camera = {
        fov = 75,
        aspectRatio = 16/9,
        position = {0, 2, 0}
    },

    objects = {
        {
            name = "sky",
            mesh = {
                type = MeshType.SPHERE,
            },
            material = {"textures/skymap1.png"},
            uniformsMap = {
                MVP     = Uniform.MVP,
                gTime   = Uniform.gTime
            },
            shaders = {
                "shaders/pass.vert",
                "shaders/skymap.frag",
                "shaders/quad.tcs",
                "shaders/sphereIN.tes"
            }
        },
        {
            name = "terrain",
            mesh = {
                type = MeshType.TERRAIN,
                size = 100,
                texres = 25,
                position = {0, 0, 0}
            },
            material = {"textures/sandHD.jpg", "textures/desertHM.png"},
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
            name = "water",
            mesh = {
                type = MeshType.PLANE,
                size = 200,
                texres = 25,
                position = {0, 1, 0}
            },
            --material = {"textures/sand.png", "textures/hm4.png"},
            uniformsMap = {
                MVP     = Uniform.MVP
            },
            shaders = {
                "shaders/debug.vert",
                "shaders/debug.frag"
                --"shaders/quad.tcs",
                --"shaders/terrain.tes"
            }
        }
    }
}