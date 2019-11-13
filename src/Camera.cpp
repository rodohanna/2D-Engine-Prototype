#include "Camera.h"

Rect camera = {};

void Camera::set_camera(Rect &rect)
{
    camera = rect;
};

Rect Camera::get_camera()
{
    return camera;
};
