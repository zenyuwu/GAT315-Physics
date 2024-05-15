#include "body.h"
#include "world.h"
#include "integrator.h"
#include "mathf.h"

void Step(ncBody* body, float timestep) {
	body->force = Vector2Add(body->force, Vector2Scale(Vector2Scale(ncGravity, body->gravityScale), body->mass));
	body->acceleration = Vector2Scale(body->force, body->inverseMass);

	SemiImplicitEuler(body, timestep);

	//apply damping
	float damping = 1 / (1 + (body->damping * timestep));
	body->velocity = Vector2Scale(body->velocity, damping);

	ClearForce(body);

	switch (body->effectType) {
		case TRAIL:
			body->positions[body->head] = body->position;
			body->head = (body->head + 1) % TRAIL_LENGTH;
			break;
	}
}

void InitBody(ncBody* body, float timestep) {
	switch (body->effectType) {
		case TRAIL:
			for (int i = 0; i < TRAIL_LENGTH; i++) {
				body->positions[i] = body->position;
			}
			break;
		case CLUSTER:
			for (int i = 0; i < CLUSTER_BODIES; i++) {
				ncBody* childBody; // = CreateBody();
				childBody->position = Vector2Add(body->position, (Vector2) { GetRandomFloatValue(-CLUSTER_RADIUS, CLUSTER_RADIUS), GetRandomFloatValue(-CLUSTER_RADIUS, CLUSTER_RADIUS) });
				childBody->mass = CLUSTER_MASS;
				childBody->inverseMass = 1 / childBody->mass;
				childBody->damping = 2.5f;
				childBody->gravityScale = 10;
				childBody->type = BT_DYNAMIC;
				ApplyForce(childBody, (Vector2) { GetRandomFloatValue(-CLUSTER_FORCE, CLUSTER_FORCE), GetRandomFloatValue(-CLUSTER_FORCE, CLUSTER_FORCE)}, FM_VELOCITY);
				Step(childBody, timestep);
			}
			break;
	}
}