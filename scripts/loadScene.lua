MeshType = require "scripts.MeshType"
UniVarFuncType = require "scripts.UniVarFuncType"

function unpackScene(s)
    local unpacked_scene = {}

    -- objects
    local objects = s.objects
    local count, object, shaders, uniVarFuncs, uniforms, materials, mesh
    
    for i=1, #objects do
        object = objects[#objects-i+1]
        
        -- shaders
        shaders = object.shaders
        if shaders then
            count = #shaders
            if count then
                for i=1, count do
                    table.insert(unpacked_scene, shaders[count-i+1])
                end
                table.insert(unpacked_scene, count)
            else
                table.insert(unpacked_scene, "shaders/debug.frag")
                table.insert(unpacked_scene, "shaders/debug.vert")
                table.insert(unpacked_scene, 2)
            end
        else
            table.insert(unpacked_scene, "shaders/debug.frag")
            table.insert(unpacked_scene, "shaders/debug.vert")
            table.insert(unpacked_scene, 2)
        end
        
        -- uniform callbacks
        uniVarFuncs = object.uniVarFuncs
        if uniVarFuncs then
            count = #uniVarFuncs
            if count then
                for i=1, count do
                    table.insert(unpacked_scene, uniVarFuncs[count-i+1])
                end
            else

            end
        else

        end
        
        --uniform variables
        uniforms = object.uniforms
        if uniforms then
            count = #uniforms
            if count then
                for i=1, count do
                    table.insert(unpacked_scene, uniforms[count-i+1])
                end
                table.insert(unpacked_scene, count)
            else
                table.insert(unpacked_scene, 0)
            end
        else
            table.insert(unpacked_scene, 0)
        end

        -- materials
        materials = object.material
        if materials then
            count = #materials
            if count then
                for i=1, count do
                    table.insert(unpacked_scene, materials[count-i+1])
                end
                table.insert(unpacked_scene, count)
            else
                table.insert(unpacked_scene, "textures/dummy.png")
                table.insert(unpacked_scene, 1)
            end
        else
            table.insert(unpacked_scene, "textures/dummy.png")
            table.insert(unpacked_scene, 1)
        end
        
        -- mesh
        mesh = object.mesh
        if mesh then
            if mesh.position then
                table.insert(unpacked_scene, (mesh.position[3] or 0.0))
                table.insert(unpacked_scene, (mesh.position[2] or 0.0))
                table.insert(unpacked_scene, (mesh.position[1] or 0.0))
            else
                table.insert(unpacked_scene, 0.0)
                table.insert(unpacked_scene, 0.0)
                table.insert(unpacked_scene, 0.0)
            end
            table.insert(unpacked_scene, (mesh.texres or 1.0))
            table.insert(unpacked_scene, (mesh.size or 10.0))
            table.insert(unpacked_scene, (mesh.type or MeshType.CUBE))
        else
            table.insert(unpacked_scene, 0.0)
            table.insert(unpacked_scene, 0.0)
            table.insert(unpacked_scene, 0.0)
            table.insert(unpacked_scene, 1.0)
            table.insert(unpacked_scene, 10.0)
            table.insert(unpacked_scene, MeshType.CUBE)
        end

        -- name
        table.insert(unpacked_scene, (object.name or "unnamed object"))
    end
    table.insert(unpacked_scene, #objects)

    -- camera
    local camera = s.camera
    if camera then
        if camera.position then
            table.insert(unpacked_scene, (camera.position[3] or 0.0))
            table.insert(unpacked_scene, (camera.position[2] or 0.0))
            table.insert(unpacked_scene, (camera.position[1] or 0.0))
        else
            table.insert(unpacked_scene, 0.0)
            table.insert(unpacked_scene, 0.0)
            table.insert(unpacked_scene, 0.0)
        end
        table.insert(unpacked_scene, (camera.aspectRatio or 16.0/9.0))
        table.insert(unpacked_scene, (camera.fov or 60.0))
    else
        table.insert(unpacked_scene, 0.0)
        table.insert(unpacked_scene, 0.0)
        table.insert(unpacked_scene, 0.0)
        table.insert(unpacked_scene, 16.0/9.0)
        table.insert(unpacked_scene, 60.0)
    end
    
    return table.unpack(unpacked_scene)
end

dofile("scripts/scene.lua") -- provides scene variable into this scope

return unpackScene(scene)
