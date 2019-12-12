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
	if (Window::cFlag){
		//if (Window::unCalc) {
			cResult = culling(glm::vec3(M_new * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			//Window::unCalc = false;
		//}
		if(!cResult){
			for (Node* child : childs) {
				child->draw(M_new, program);
			}
			Window::cullingCount++;
		}
	}else{
		for (Node* child : childs) {
			child->draw(M_new, program);
		}
		Window::cullingCount++;
	}
}
bool TopTrans::culling(glm::vec3 x) {
	if (glm::dot((x - Window::fc), Window::normF) > 15.0f) {
		return true;
	}
	else if (glm::dot((x - Window::nc), Window::normN) > 15.0f) {
		return true;
	}
	else if (glm::dot((x - Window::ntl), Window::normU) > 15.0f) {
		return true;
	}
	else if (glm::dot((x - Window::nbr), Window::normB) > 15.0f) {
		return true;
	}
	else if (glm::dot((x - Window::ntr), Window::normR) > 15.0f) {
		return true;
	}
	else if (glm::dot((x - Window::nbl), Window::normL) > 15.0f) {
		return true;
	}
	return false;
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

