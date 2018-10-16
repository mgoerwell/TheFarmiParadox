#include "UIComponent.h"

UIComponent::UIComponent(float width, float height, float x, float y) :
    size(width, height), anchor(x, y) {

    // Set default values for a panel
    visible = true;
    vAnchor = ANCHOR_TOP;
    hAnchor = ANCHOR_LEFT;
    anchorXType = ANCHOR_PIXEL;
    anchorYType = ANCHOR_PIXEL;
    xType = UNIT_PERCENT;
    yType = UNIT_PERCENT;
    parent = nullptr;

    _position = &screenPosition;
    _rotation = &rotation;
    _z = 0;

    // Initialize Renderable data with a standard quad
    _normals = { { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 } };
    _elements = { 1, 0, 2, 1, 2, 3 };
    _texCoords = { { 0, 1 },{ 1, 1 },{ 0, 0 },{ 1, 0 } };
    _positions = { {-2.5,2.5,0},{2.5,2.5,0}, {-2.5,-2.5,0}, {2.5,-2.5,0} };
    _color = {0,0,0,0};

    // Generate buffers for this panel
    TypeParam<UIComponent*> param(this);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);
}

UIComponent::~UIComponent() {
    for (UIComponent *child : children)
        delete child;
}

void UIComponent::Resize() {
    if (parent != nullptr) {
        // Calculate pixel size of panel based on Unit Type
        switch (xType) {
        case UNIT_PIXEL:
            screenSize.x = size.x;
            break;
        case UNIT_PERCENT:
            screenSize.x = parent->screenSize.x * (size.x / 100.0f);
            break;
        }
        switch(yType) {
        case UNIT_PIXEL:
            screenSize.y = size.y;
            break;
        case UNIT_PERCENT:
            screenSize.y = parent->screenSize.y * (size.y / 100.0f);
            break;
        case UNIT_SCALE:
            screenSize.y = screenSize.x / aspectRatio;
            break;
        }
        if (xType == UNIT_SCALE) {
            screenSize.x = screenSize.y * aspectRatio;
        }

        // Calculate Anchor Position of panel based on anchor unit type
        glm::vec2 screenAnchor;
        screenAnchor.x = anchorXType == ANCHOR_PERCENT ? anchor.x / 100.0f * parent->screenSize.x / 2.0f : anchor.x;
        screenAnchor.y = anchorYType == ANCHOR_PERCENT ? anchor.y / 100.0f * parent->screenSize.y / 2.0f : anchor.y;

        // Calculate absolute position of panel based on parent panel and anchor types
        switch (hAnchor) {
        case ANCHOR_LEFT:
            screenPosition.x = parent->screenPosition.x + screenAnchor.x;
            break;
        case ANCHOR_HCENTER:
            screenPosition.x = parent->screenPosition.x + parent->screenSize.x / 4 - screenSize.x / 4 + screenAnchor.x;
            break;
        case ANCHOR_RIGHT:
            screenPosition.x = parent->screenPosition.x + parent->screenSize.x / 2 - screenSize.x / 2 - screenAnchor.x;
            break;
        }

        switch (vAnchor) {
        case ANCHOR_TOP:
            screenPosition.y = parent->screenPosition.y + parent->screenSize.y / 2 - screenSize.y / 2 - screenAnchor.y;
            break;
        case ANCHOR_VCENTER:
            screenPosition.y = parent->screenPosition.y + parent->screenSize.y / 4 - screenSize.y / 4 + screenAnchor.y;
            break;
        case ANCHOR_BOTTOM:
            screenPosition.y = parent->screenPosition.y + screenAnchor.y;
            break;
        }

        _z = parent->_z + 1;
    }
    
    // Generate vertices of quad from position and size of panel
    _positions = {
        { screenPosition.x, screenPosition.y + screenSize.y, 0 }, // Top Left
        { screenPosition + screenSize, 0 }, // Top Right
        { screenPosition, 0 }, // Bottom Left
        { screenPosition.x + screenSize.x, screenPosition.y, 0 }  // Bottom Right
    };

    // Tell Renderer to recreate buffers with new position data
    TypeParam<UIComponent*> param(this);
    EventManager::notify(RENDERER_POPULATE_BUFFERS, &param, false);

    // Iterate resize on child panels
    for (UIComponent *child : children) {
        child->Resize();
    }
}

void UIComponent::Add(UIComponent* child) {
    child->parent = this;
    children.push_back(child);
    child->Resize();
}

bool UIComponent::IsTransparent() {
    return (_color.a < 1.0f || _textureLocation != 0);
}

void UIComponent::SetImage(std::string path) {
    TypeParam<std::pair<UIComponent*, std::string>> param(std::pair<UIComponent*,std::string>(this, path));
    EventManager::notify(RENDERER_CREATE_UI_TEXTURE, &param, false);
}

