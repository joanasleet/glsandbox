scene = {

    camera = {
        fov = 75,
        aspectRatio = 16/9,
        position = {0, 10, 0}
    },

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
                size = 100,
                texres = 1,
                position = {0, 0, 0}
            },
            material = {
                "textures/shm.png",
                "textures/cliff.jpg",
                "textures/grass.png",
                "textures/nm.png"
            },
            uniformsMap = {
                P       = Uniform.P,
                MV      = Uniform.MVnoTrans,
                MVP     = Uniform.MVP,
                camPos  = Uniform.camPos
            },
            shaders = {
                "shaders/pass.vert",
                "shaders/terrain.frag",
                "shaders/quad.tcs",
                "shaders/terrain.tes"
                --"shaders/drawNormals.geo"
            }
        }
    }
}
