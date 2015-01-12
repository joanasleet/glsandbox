scene = {

    camera = {
        fov = 75,
        aspectRatio = 16/9,
        position = {0, 2, 0}
    },

    objects = {
        {
            name = "terrain",
            mesh = {
                type = MeshType.TERRAIN,
                size = 200,
                texres = 25,
                position = {0, 0, 0}
            },
            material = {"textures/ground.jpg", "textures/hm3.png"},
            uniforms = {"MVP"},
            uniVarFuncs = {Uniform.ModelViewPerspective},
            shaders = {
                "shaders/terrain.vert",
                "shaders/terrain.frag",
                "shaders/terrain.tcs",
                "shaders/terrain.tes"
            }
        },
        {
            name = "sphere",
            mesh = {
                type = MeshType.SPHERE,
                size = 1,
                texres = 1,
                position = {0, 0, 0}
            },
            material = {"textures/gm.png"},
            uniforms = {"MVP"},
            uniVarFuncs = {Uniform.ModelViewPerspective},
            shaders = {
                "shaders/sphere.vert",
                "shaders/sphere.frag",
                "shaders/sphere.tcs",
                "shaders/sphereIN.tes"
            }
        }
    }
}