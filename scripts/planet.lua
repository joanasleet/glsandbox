scene = {

    camera = {
        fov = 75,
        aspectRatio = 16/9,
        position = {0, 10.0 , 0}
    },

    objects = {
        {
            name = "atmosphere",
            mesh = {
                type = MeshType.SPHERE,
                size = 1,
                texres = 1,
                position = {0, 0, 0}
            },
            --material = {"textures/sky.png"},
            uniforms = {"MVP", "v3CameraPos"},
            uniVarFuncs = {Uniform.ModelViewPerspective, Uniform.CamPos},
            shaders = {
                "shaders/pass.vert",
                "shaders/sphereAtmoScat.frag",
                "shaders/quad.tcs",
                "shaders/sphereAtmoScat.tes"
            }
        },
        {
            name = "earth",
            mesh = {
                type = MeshType.SPHERE,
                size = 1,
                texres = 1,
                position = {0, 0, 0}
            },
            material = {"textures/ground.jpg"},
            uniforms = {"MVP"},
            uniVarFuncs = {Uniform.ModelViewPerspective},
            shaders = {
                "shaders/pass.vert",
                "shaders/tex.frag",
                "shaders/quad.tcs",
                "shaders/sphereOUT.tes"
            }
        }
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