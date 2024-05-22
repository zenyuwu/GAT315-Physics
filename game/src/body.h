#pragma once
#include "raylib.h"
#include "raymath.h"

#define TRAIL_LENGTH 10
#define CLUSTER_BODIES 5
#define CLUSTER_RADIUS 20
#define CLUSTER_MASS 2
#define CLUSTER_FORCE 300

//make sure these are in order in the dropdown in the gui
typedef enum {
	BT_STATIC,
	BT_KINEMATIC,
	BT_DYNAMIC
} ncBodyType;

typedef enum {
	FM_FORCE,
	FM_IMPULSE,
	FM_VELOCITY
} ncForceMode;

typedef enum {
	CLUSTER,
	TRAIL,
	ROCKET
} ncEffectType;

typedef struct ncBody {
	ncBodyType type;

	ncEffectType effectType;
	Vector2 positions[TRAIL_LENGTH];
	int head;

	//acceleration -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 force;

	float mass;
	float inverseMass;
	float gravityScale;
	float damping;

	float restitution;

	struct ncBody* next;
	struct ncBody* prev;
} ncBody;

inline void ApplyForce(ncBody* body, Vector2 force, ncForceMode forceMode) {
	if (body->type != BT_DYNAMIC) {
		return;
	}

	switch (forceMode) {
	case FM_FORCE:
		body->force = Vector2Add(body->force, force);
		break;
	case FM_IMPULSE:
		//applies a sudden change in momentum
		body->velocity = Vector2Add(body->velocity, Vector2Scale(force, body->inverseMass));
		break;
	case FM_VELOCITY:
		body->velocity = force;
		break;
	}

}

inline void ClearForce(ncBody* body) {
	body->force = Vector2Zero();
}

void Step(ncBody* body, float timestep);

void InitBody(ncBody* body);