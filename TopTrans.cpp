//
//  TopTrans.cpp
//  CSE167 P2
//
//  Created by Yichen Zhang on 10/28/19.
//  Copyright © 2019 Yichen Zhang. All rights reserved.
//

#include "TopTrans.h"
#include "Window.h"
TopTrans::TopTrans(glm::mat4 transM)
{
	M = transM;
	init = M;
}
TopTrans::~TopTrans()
{
	for (Node* child : childs) {
		delete child;
	}
}
void TopTrans::addChild(Node* child) {
	childs.push_back(child);
}
void TopTrans::draw(glm::mat4 C, GLuint program) {
	glm::mat4 M_new = C * M;
	for (Node* child : childs) {
		child->draw(M_new, program);
	}
}

void TopTrans::update(bool flag) {
	if (flag) {
		M = glm::translate(M, glm::vec3(0.0f, 0.025f, 0.0f));
	}
	else {
		M = glm::translate(M, glm::vec3(0.0f, -0.025f, 0.0f));
	}
}
void TopTrans::rotation(int i) {
}

void TopTrans::detectCollision(glm::vec3 eye, glm::mat4 C) {
	glm::mat4 M_new = C * M;
	if ((eye.x > M_new[3][0] - 5.0f) && (eye.x < M_new[3][0] + 5.0f)) {
		if ((eye.z > M_new[3][2] - 5.0f) && (eye.z < M_new[3][2] + 5.0f)) {
			for (Node* child : childs) {
				child->detectCollision(eye, M_new);
			}
		}
	}
}
