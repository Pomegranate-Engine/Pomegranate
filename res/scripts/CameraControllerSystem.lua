print_info("The camera can be controlled via LuaSystem with WASD keys.\n"..
        "C++ control is disabled while LuaSystem is active.\n"..
        "Press F1 to toggle LuaSystem.")

camera = nil

function tick(entity)
    if has_component(entity, "Camera") and has_component(entity,"CameraController") then
        local cc = get_component(entity,"CameraController")
        local t = get_component(entity,"Transform")
        cc.vx = cc.vx + input_get_axis("a","d")*cc.speed
        cc.vy = cc.vy + input_get_axis("w","s")*cc.speed
        t.pos.x = t.pos.x + cc.vx
        t.pos.y = t.pos.y + cc.vy
        cc.vx = cc.vx * cc.drag
        cc.vy = cc.vy * cc.drag
        camera = t
    end
end

function draw(entity)
    if(has_component(entity, "Transform")) then
        local t = get_component(entity,"Transform")
        if(camera ~= nil) then
            t.pos.x = t.pos.x - camera.pos.x
            t.pos.y = t.pos.y - camera.pos.y
        end
        draw_pixel(t.pos,{r=255,g=255,b=255,a=255})
    end
end