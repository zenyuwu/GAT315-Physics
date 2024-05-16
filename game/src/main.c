#include "body.h"
#include "raylib.h"
#include "mathf.h"
#include "raymath.h"
#include "integrator.h"
#include "world.h"
#include "force.h"
#include "render.h"
#include "editor.h"
#include "spring.h"
#include "collision.h"
#include "contact.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 100

int main(void)
{
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;

	int screenWidth = 1200;
	int screenHeight = 800;
	InitWindow(screenWidth, screenHeight, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	//init world
	ncGravity = (Vector2){ 0,0 }; //not sure if this is right

	//game loop
	while (!WindowShouldClose())
	{
		//update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);
		UpdateEditor(position);

		selectedBody = GetBodyIntersect(ncBodies, position);
		if (selectedBody) {
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines((int)screen.x, (int)screen.y, ConvertWorldToPixel(selectedBody->mass * 0.5f) + 5, YELLOW);
		}

		//create body
		//left click
		if (IsMouseButtonPressed(0) && !ncEditorActive) {
			ncBody* body = CreateBody(ConvertScreenToWorld(position), ncEditorData.MassMinValue, ncEditorData.BodyTypeActive);

			body->effectType = ROCKET;
			//try w zero damping
			body->damping = 2.5f;
			//try w zero?
			body->gravityScale = ncEditorData.GravitationValue;
			//ApplyForce(body, (Vector2){GetRandomFloatValue(-300, 300), GetRandomFloatValue(-300, 300)}, FM_VELOCITY);

			AddBody(body);
			InitBody(body, dt);
		}

		//maple's rick click
		//connect springs
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) {
			connectBody = selectedBody;
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) {
			DrawLineBodyToPosition(connectBody, position);
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody) {
			if (selectedBody && selectedBody != connectBody) {
				ncSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), 20);
				AddSpring(spring);
			}
		}

		////right click
		//if (IsMouseButtonDown(1)) {
		//	ncBody* body; //= CreateBody();
		//	body->position = position;
		//	body->mass = 3;
		//	body->inverseMass = 1 / body->mass;
		//	body->type = BT_DYNAMIC;
		//	body->effectType = TRAIL;
		//	body->damping = 2.5f;
		//	body->gravityScale = 20;
		//	InitBody(body, dt);
		//	ApplyForce(body, (Vector2) { GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);
		//}

		////middle click
		//if (IsMouseButtonDown(2)) {
		//	ncBody* body; // = CreateBody();
		//	body->position = position;
		//	body->mass = 10;
		//	body->inverseMass = 1 / body->mass;
		//	body->type = BT_DYNAMIC;
		//	body->effectType = ROCKET;
		//	body->damping = 2.5f;
		//	body->gravityScale = 20;
		//	ApplyForce(body, (Vector2) { GetRandomFloatValue(100, 1000), GetRandomFloatValue(-500, -100) }, FM_VELOCITY);
		//}

		//apply force
		ApplyGravitation(ncBodies, ncEditorData.GravitationValue);
		ApplySpringForce(ncSprings);

		//update bodies
		for (ncBody* body = ncBodies; body; body = body->next) { //body can also be body != NULL
			Step(body, dt);
		}

		//collision
		ncContact_t* contacts = NULL;
		CreateContacts(ncBodies, &contacts);

		//render
		BeginDrawing();
		ClearBackground(BLACK);

		//stats
		DrawText(TextFormat("FPS: %.2f (ms %.2f)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		//DrawCircle((int)position.x, (int)position.y, 20, YELLOW);

		//draw bodies
		for (ncBody* body = ncBodies; body; body = body->next) { //body can also be body != NULL
			switch (body->effectType) {
			case TRAIL:
				for (int i = 0; i < TRAIL_LENGTH - 1; i++) {
					int nextIndex = (body->head + i) % TRAIL_LENGTH;
					DrawLineEx(body->positions[nextIndex], body->positions[i], 2, RED);
				}
				break;
			}
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass * 0.5f), WHITE);
		}

		//draw springs
		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next) { //body can also be body != NULL
			Vector2 screen = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine((int)screen.x, (int)screen.y, (int)screen2.x, (int)screen2.y, YELLOW);
		}

		//draw contacts
		for (ncContact_t* contact = contacts; contact; contact = contact->next) {
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), RED);
		}

		DrawEditor(position);

		EndDrawing();
	}

	CloseWindow();

	free(ncBodies);

	return 0;
}