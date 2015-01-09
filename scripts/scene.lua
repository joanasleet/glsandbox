scene = {

    camera = {
        fov = 75,
        aspectRatio = 16/9,
        position = {0, 2, 0}
    },

    objects = {
        -- {
        --     name = "test",
        --     mesh = {
        --         type = MeshType.CUBE,
        --         size = 2,
        --         texres = 1,
        --         position = {0, 1.2, 0}
        --     },
        --     material = {"textures/tiles.png"},
        --     uniforms = {"MVP"},
        --     uniVarFuncs = {UniVarFuncType.ModelViewPerspective},
        --     shaders = {"shaders/base.vert", "shaders/tex.frag"}
        -- },
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
            uniVarFuncs = {UniVarFuncType.ModelViewPerspective},
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
            material = {"textures/hm3.png"},
            uniforms = {"MVP"},
            uniVarFuncs = {UniVarFuncType.ModelViewPerspectiveNoTrans},
            shaders = {
                "shaders/sphere.vert",
                "shaders/sphere.frag",
                "shaders/sphere.tcs",
                "shaders/sphereIN.tes"
            }
        }
        -- {
        --     name = "sky",
        --     mesh = {
        --         type = MeshType.CUBEMAP,
        --         size = 1000,
        --         texres = 0,
        --         position = {0, 0, 0}
        --     },
        --     material = {"textures/left.png", "textures/right.png", "textures/up.png", "textures/down.png", "textures/front.png", "textures/back.png"},
        --     uniforms = {"MVP"},
        --     uniVarFuncs = {UniVarFuncType.ModelViewPerspectiveNoTrans},
        --     shaders = {"shaders/cubemap.vert", "shaders/cubemap.frag"}
        -- }
    }
}