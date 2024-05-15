#include "world.h"
#include "body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity;

ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType) {
	ncBody* body = (ncBody*)malloc(sizeof(ncBody));
	assert(body);
	memset(body, 0, sizeof(ncBody));
	body->position = position;
	body->mass = mass;
	body->inverseMass = (bodyType == BT_DYNAMIC) ? 1 / mass : 0;
	body->type = bodyType;

	return body;
}

void AddBody(ncBody* body) {
	assert(body);

	//add element to linked list
	body->prev = NULL;
	body->next = ncBodies;

	if (ncBodies) ncBodies->prev = body;

	//set head of bodies to new element
	ncBodies = body;

	ncBodyCount++;
}

void DestroyBody(ncBody* body) {
	assert(body);

}