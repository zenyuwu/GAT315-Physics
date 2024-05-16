#include "world.h"
#include "spring.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncSpring_t* ncSprings = NULL;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float k) {
	ncSpring_t* spring = (ncSpring_t*)malloc(sizeof(ncSpring_t));
	assert(spring);
	memset(spring, 0, sizeof(ncSpring_t));
	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->k = k;

	return spring;
}

void AddSpring(ncSpring_t* spring) {
	assert(spring);

	//add element to linked list
	spring->prev = NULL;
	spring->next = ncSprings;

	if (ncSprings) ncSprings->prev = spring;

	//set head of elements to new element
	ncSprings = spring;
}

void DestroySpring(ncSpring_t* spring) {
	assert(spring);

	if (spring->prev) spring->prev->next = spring->next;
	if (spring->next) spring->next->prev = spring->prev;

	if (spring == ncSprings) ncSprings = spring->next;

	free(spring);
}

void DestroyAllBodies() {
	if (!ncSprings) return;

	ncSpring_t* spring = ncBodies;
	while (spring) {
	ncSpring_t* next = spring->next;
	free(spring);
	spring = next;
	}

	ncSprings = NULL;
}

void ApplySpringForce(ncSpring_t* springs) {
	for (ncSpring_t* spring = springs; spring; spring = spring->next)
	{
		Vector2 direction = Vector2Subtract(spring->body2->position, spring->body1->position);
		if (direction.x == 0 && direction.y == 0) continue;

		float length = Vector2Length(direction);
		float x = length - spring->restLength;
		float force = -spring->k * x; // f = -kx <- Hooke's Law

		Vector2 ndirection = Vector2Normalize(direction);

		ApplyForce(spring->body2, Vector2Scale(ndirection, force), FM_FORCE);
		ApplyForce(spring->body1, Vector2Scale(Vector2Negate(ndirection), force), FM_FORCE);
	}
}