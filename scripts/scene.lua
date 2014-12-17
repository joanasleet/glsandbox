scene = {

    camera = {
        fov = 75,
        aspectRatio = 16/9,
        position = {0, 1, 15}
    },

    objects = {
        {
            name = "test",
            mesh = {
                type = MeshType.CUBE,
                size = 2,
                texres = 1,
                position = {0, 1.2, 0}
            },
            material = {"textures/tiles.png"},
            uniforms = {"MVP"},
            uniVarFuncs = {UniVarFuncType.ModelViewPerspective},
            shaders = {"shaders/base.vert", "shaders/tex.frag"}
        },
        {
            name = "plane",
            mesh = {
                type = MeshType.PLANE,
                size = 1000,
                texres = 300,
                position = {0, 0, 0}
            },
            material = {"textures/ground.jpg"},
            uniforms = {"MVP"},
            uniVarFuncs = {UniVarFuncType.ModelViewPerspective},
            shaders = {"shaders/base.vert", "shaders/tex.frag"}
        },
        {
            name = "sky",
            mesh = {
                type = MeshType.CUBEMAP,
                size = 1000,
                texres = 0,
                position = {0, 0, 0}
            },
            material = {"textures/left.png", "textures/right.png", "textures/up.png", "textures/down.png", "textures/front.png", "textures/back.png"},
            uniforms = {"MVP"},
            uniVarFuncs = {UniVarFuncType.ModelViewPerspective},
            shaders = {"shaders/cubemap.vert", "shaders/cubemap.frag"}
        }
    }
}