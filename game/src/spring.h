#pragma once

typedef struct ncSpring {
	struct ncBody* body1;
	struct ncBody* body2;
	float restLength;
	float k; //stiffness

	struct ncSpring* next;
	struct ncSpring* prev; //maple had these as structs?
} ncSpring_t;

extern ncSpring_t* ncSprings;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float k);
void AddSpring(ncSpring_t* spring);
void DestroySpring(ncSpring_t* spring);
void DestroyAllBodies();

void ApplySpringForce(ncSpring_t* springs);