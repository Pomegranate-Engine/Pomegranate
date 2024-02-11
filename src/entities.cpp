class CameraController : public Component
{
public:
    Vec2 velocity;
    float speed = 3.0f;
    float drag = 0.9f;
    CameraController()
    {
        register_component<CameraController>();
        push_data<Vec2>("velocity", &this->velocity);
        push_data<float>("speed", &this->speed);
        push_data<float>("drag", &this->drag);
    }
};

class DestroyAfterTime : public Component
{
public:
    float time = 0.0;
    float length = 5.0;
    DestroyAfterTime()
    {
        register_component<DestroyAfterTime>();
        push_data<float>("time", &this->time);
        push_data<float>("length", &this->length);
    }
};