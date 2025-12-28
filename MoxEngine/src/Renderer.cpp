#include "Renderer.h"
#include "pch.h"
Renderer::Renderer(GameObject* obj) { _transform = obj->_transform.get(); _parent = obj; }
Renderer::Renderer(Transform* transform) { _transform = transform; }