#include "Bulleto.h"

float minimumCollisionDitance = 1.0f;

Bulleto::Bulleto(float damage) {
	this->damage = damage;
}


bool Bulleto::colliding_with_player(glm::vec2 & player_pos) {
	float length = glm::length(get_position() - player_pos);
	return length < (this->radius + minimumCollisionDitance);
}