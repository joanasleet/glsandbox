--function unpackScene(s)
--    unpacked_scene = {#s}
--    local mesh
--    local uniform
--                
--    for i=1, #s do
--        mesh = s[i]
--        table.insert(unpacked_scene, mesh.name)
--        table.insert(unpacked_scene, mesh.vaoType)
--        table.insert(unpacked_scene, mesh.texture)
--        uniforms = mesh.uniforms
--        table.insert(unpacked_scene, #uniforms)
--        for i=1, #uniforms do
--            table.insert(unpacked_scene, uniforms[i])
--        end
--    end
--    
--    return table.unpack(unpacked_scene)
--end

function unpackScene(s)
    unpacked_scene = {}
    local mesh
    local uniform
                
    for i=1, #s do
        mesh = s[#s-i+1]
        
        shaders = mesh.shaders
        count = #shaders
        for i=1, count do
            table.insert(unpacked_scene, shaders[count-i+1])
        end
        table.insert(unpacked_scene, count)
        
        uniVarFuncs = mesh.uniVarFuncs
        count = #uniVarFuncs
        for i=1, count do
            table.insert(unpacked_scene, uniVarFuncs[count-i+1])
        end
        
        uniforms = mesh.uniforms
        count = #uniforms
        for i=1, count do
            table.insert(unpacked_scene, uniforms[count-i+1])
        end
        table.insert(unpacked_scene, count)
        
        table.insert(unpacked_scene, mesh.texture)
        table.insert(unpacked_scene, mesh.vaoType)
        table.insert(unpacked_scene, mesh.name)
    end
    
    table.insert(unpacked_scene, #s)
    
    return table.unpack(unpacked_scene)
end

return unpackScene(dofile("scripts/scene.lua"))
