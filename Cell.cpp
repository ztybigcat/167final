//
//  Cell.cpp
//  CSE167 P2
//
//  Created by Yichen Zhang on 10/28/19.
//  Copyright © 2019 Yichen Zhang. All rights reserved.
//

#include "Cell.h"
#include "Window.h"
Cell::Cell(glm::mat4 transM)
{
	M = transM;
	init = M;
}
Cell::~Cell()
{
	for (Node* child : childs) {
		delete child;
	}
}
void Cell::addChild(Node* child) {
	childs.push_back(child);
}
void Cell::draw(glm::mat4 C, GLuint program) {
	glm::mat4 M_new = C * M;
		for (Node* child : childs) {
			child->draw(M_new, program);
		}
}

void Cell::update(bool flag) {
	if (flag) {
		M = glm::translate(M, glm::vec3(0.0f, 0.025f, 0.0f));
	}
	else {
		M = glm::translate(M, glm::vec3(0.0f, -0.025f, 0.0f));
	}
}
void Cell::rotation(int i) {
}

void Cell::detectCollision(glm::vec3 eye, glm::mat4 C) {
	glm::mat4 M_new = C * M;
	if ((eye.y > M_new[3][1] - 2.0f) && (eye.y < M_new[3][1] + 2.0f)) {
		Window::gameOver();
	}
}
