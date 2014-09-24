print("Lua ––––––––––––––––––––––")
scene = {
    {
        name = "skybox",
        vaoType = 0,
        texture = "asd.png",
        uniforms = {"A", "B", "C"}
    },
    {
        name = "terrain",
        vaoType = 0,
        texture = "asd.png",
        uniforms = {"A", "B", "C"}
    },
    {
        name = "cube",
        vaoType = 0,
        texture = "asd.png",
        uniforms = {"A", "B", "C"}
    }
}

function printMesh(m)
    print("name: "..m.name)
    print("vaoType: "..m.vaoType)
    print("texture: "..m.texture)
    print("uniforms: "..table.concat(m.uniforms,', '))
    print("")
end

function parseMesh(m)
    
end

function printScene(s)
    for i=1, #s do
        printMesh(s[i])
    end
end

function unpackScene(s)
    unpacked_scene = {#s}
    local mesh
    local uniform
    
    for i=1, #s do
        mesh = s[i]
        table.insert(unpacked_scene, mesh.name)
        table.insert(unpacked_scene, mesh.vaoType)
        table.insert(unpacked_scene, mesh.texture)
        uniforms = mesh.uniforms
        table.insert(unpacked_scene, #uniforms)
        for i=1, #uniforms do
            table.insert(unpacked_scene, uniforms[i])
        end
    end
    
    return table.unpack(unpacked_scene)
end

print(unpackScene(scene))

--printScene(scene)
print("–––––––––––––––––––––– Lua")


return 1337, "herpa derp"
