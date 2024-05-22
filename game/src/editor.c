#include "editor.h"
#include "body.h"
#include "render.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;
Rectangle editorRect;
Texture2D cursorTexture;
bool EditorBoxActive = true;

bool MainWindowActive = true;
float SliderMinimumValue = 0.0f;
float SliderMaximumValue = 0.0f;
bool BodyTypeEditMode = false;
int BodyTypeActive = 0;
float BodyGravityValue = 0.0f;
float BodyDampingValue = 0.0f;
float WorldGravityValue = 0.0f;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

    ncEditorData.anchor01 = (Vector2){ 792,48 };
    ncEditorData.EditorBoxActive = true;
    ncEditorData.MassMinValue = 2.0f;
    ncEditorData.MassMaxValue = 0.1f;
    ncEditorData.GravitationValue = 3000.0f;
    ncEditorData.BodyTypeEditMode = false;
    ncEditorData.BodyTypeActive = 0;
    ncEditorData.DampingValue = 0.0f;
    ncEditorData.GravityScaleValue = 0.0f;

    editorRect = (Rectangle){ ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 304, 616 };

    Image image = LoadImage("resources/reticle.png");
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    //hide cursor
    
    //
}

bool IsMouseHoveringRectanglee(Rectangle rect)
{
    Vector2 mousePosition = GetMousePosition();
    return (mousePosition.x >= rect.x &&
        mousePosition.x < rect.x + rect.width &&
        mousePosition.y >= rect.y &&
        mousePosition.y < rect.y + rect.height);
}

void UpdateEditor(Vector2 position)
{
    ncEditorIntersect = EditorBoxActive;
    // Check for mouse interaction within the editor box
    if (IsMouseHoveringRectanglee(editorRect))
    {
        ncEditorActive = true;
    }
    else {
        ncEditorActive = false;
    }

    // Update cursor position based on mouse position
    //ncEditorData.anchor01 = position;
    //editorRect.x = position.x + 0;
    //editorRect.y = position.y + 0;

    // Update values based on slider and dropdown interaction (assuming RayGui handles these events)
    ncEditorData.MassMinValue = SliderMinimumValue;  // Assuming slider updates minimum mass value
    ncEditorData.MassMaxValue = SliderMaximumValue;  // Assuming slider updates maximum mass value
    ncEditorData.GravitationValue = BodyGravityValue;  // Assuming slider updates body gravity scale
    ncEditorData.DampingValue = BodyDampingValue;    // Assuming slider updates damping value
    ncEditorData.GravityScaleValue = WorldGravityValue; // Assuming slider updates world gravity scale
    ncEditorData.BodyTypeActive = BodyTypeActive;
    //if (GuiDropdownBox((Rectangle) { 792, 192, 264, 24 }, "BT_STATIC;BT_KINEMATIC;BT_DYNAMIC", & BodyTypeActive, BodyTypeEditMode))
    //{
    //    ncEditorData.BodyTypeActive = BodyTypeActive;  // Update body type based on dropdown selection
    //    ncEditorData.BodyTypeEditMode = !BodyTypeEditMode;  // Toggle edit mode on dropdown interaction
    //}
}

void DrawEditor(Vector2 position)
{
    if (ncEditorData.BodyTypeEditMode) GuiLock();

    if (ncEditorData.EditorBoxActive) {
        ncEditorData.EditorBoxActive = !GuiWindowBox((Rectangle) { ncEditorData.anchor01.x + 0, ncEditorData.BodyTypeActive + 0, 304, 616 }, "EDITOR");
        GuiSliderBar((Rectangle) { 880, 96, 176, 24 }, "Body Minimum Mass", NULL, & SliderMinimumValue, 0, 10);
        GuiSliderBar((Rectangle) { 880, 144, 176, 24 }, "Body Maximum Mass", NULL, & SliderMaximumValue, 0, 100);
        GuiSliderBar((Rectangle) { 888, 240, 168, 24 }, "Body Gravity Scale", NULL, & BodyGravityValue, 0, 100);
        GuiSliderBar((Rectangle) { 888, 288, 168, 24 }, "Body Damping", NULL, & BodyDampingValue, 0, 100);
        GuiSliderBar((Rectangle) { 888, 336, 168, 24 }, "World Gravity Scale", NULL, & WorldGravityValue, 0, 100);
        if (GuiDropdownBox((Rectangle) { 792, 192, 264, 24 }, "BT_STATIC;BT_KINEMATIC;BT_DYNAMIC", & BodyTypeActive, BodyTypeEditMode)) BodyTypeEditMode = !BodyTypeEditMode;
        
    }
    DrawTexture(cursorTexture, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, WHITE);
    //
    GuiUnlock();
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass * 0.5f)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}