//
//  Token.cpp
//  CSE167 P2
//
//  Created by Yichen Zhang on 10/28/19.
//  Copyright © 2019 Yichen Zhang. All rights reserved.
//

#include "Token.h"
#include "Window.h"
Token::Token(glm::mat4 transM)
{
	M = transM;
	init = M;
}
Token::~Token()
{
}
void Token::addChild(Node* child) {
	childs.push_back(child);
}
void Token::draw(glm::mat4 C, GLuint program) {
	glm::mat4 M_new = C * M;
		for (Node* child : childs) {
			child->draw(M_new, program);
		}
}

void Token::update(bool flag) {
	if (flag) {
		M = glm::translate(M, glm::vec3(0.0f, 0.025f, 0.0f));
	}
	else {
		M = glm::translate(M, glm::vec3(0.0f, -0.025f, 0.0f));
	}
}
void Token::rotation(int i) {
}

void Token::detectCollision(glm::vec3 eye, glm::mat4 C) {
	glm::mat4 M_new = C * M;

	if (!childs.empty()) {
		if ((eye.x > M_new[3][0] - 1.0f) && (eye.x < M_new[3][0] + 1.0f)) {
			if ((eye.y > M_new[3][1] - 1.0f) && (eye.y < M_new[3][1] + 1.0f)) {
				if ((eye.z > M_new[3][2] - 1.0f) && (eye.z < M_new[3][2] + 1.0f)) {
					for (Node* child : childs) {
						if (child == Window::g_gold) {
							Window::incScore();
						}
						else if (child == Window::g_bomb) {
							Window::decScore();
						}
					}
					childs.clear();
				}
			}
		}
	}
}
