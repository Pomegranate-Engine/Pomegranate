print_pass("Camera Controller being registered")
register_component("CameraController", {
    speed = 3.0,
    drag = 0.9,
    vx = 0,
    vy = 0,
})

function init(entity)
    print("Requiring")
    require_component(entity,"Camera")
    print("Camera")
    require_component(entity,"Transform")
    print("Transform")
    require_component(entity,"UIText")
    print("UIText")
end

