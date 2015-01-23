scene = {

    camera = {
        fov = 75,
        aspectRatio = 16/9,
        position = {0, 2, -50}
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
            material = {"textures/sand.png", "textures/hm3.png"},
            uniformsMap = {
                MVP     = Uniform.MVP
            },
            shaders = {
                "shaders/pass.vert",
                "shaders/terrain.frag",
                "shaders/quad.tcs",
                "shaders/terrain.tes"
            }
        }
    }
}