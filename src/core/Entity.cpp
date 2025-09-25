#include "Entity.h"
#include<glm/gtc/matrix_transform.hpp>

glm::mat4 Entity::getModelMatrix() {
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(m_model.position));
	model = glm::rotate(model, glm::radians(m_model.rotation), glm::vec3(0, 0, 1));
	model = glm::scale(model,glm::vec3(m_model.size));

	return model;
}