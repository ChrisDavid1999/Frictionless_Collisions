#pragma once
#include "Rigidbody.h"
#include "Transform.h"

struct GameObject
{
    solo::Transform transform;
    solo::Rigidbody rigidbody;
};
