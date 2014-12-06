scene = {

    camera = {
        fov = 60,
        aspectRatio = 16/9,
        position = {0, 1, 10}
    },

    objects = {
        {
            name = "test",
            mesh = {
                type = MeshType.OVERLAY,
                size = 1000,
                texres = 1,
                position = {0, 1, 0}
            },
            material = {"textures/brick.png"},
            uniforms = {"MVP"},
            uniVarFuncs = {UniVarFuncType.ModelViewPerspective},
            shaders = {"shaders/debug.vert", "shaders/debug.frag"}
        },
        {
            name = "plane",
            mesh = {
                type = MeshType.PLANE,
                size = 1000,
                texres = 400,
                position = {0, 0, 0}
            },
            material = {"textures/grass.jpg"},
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