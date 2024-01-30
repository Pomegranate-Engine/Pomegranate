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

function pre_draw()
    if(camera~=nil) then
        if has_component(camera,"UIText") then
            local text = get_component(camera,"UIText")
            text.text = "FPS: "..tostring(1.0/delta_time)

            local transform = get_component(camera,"UITransform")
            transform.position.x = 0
            transform.position.y = 512
        end
    end
end