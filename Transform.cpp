//
//  Transform.cpp
//  CSE167 P2
//
//  Created by Yichen Zhang on 10/28/19.
//  Copyright © 2019 Yichen Zhang. All rights reserved.
//

#include "Transform.h"

Transform::Transform(glm::mat4 transM)
{
    M = transM;
	init = M;
}
Transform::~Transform()
{
	for (Node* child : childs) {
		delete child;
	}
}
void Transform::addChild(Node* child){
    childs.push_back(child);
}
void Transform::draw(glm::mat4 C, GLuint program){
    glm::mat4 M_new = C*M;
    for (Node* child : childs){
        child->draw(M_new,program);
    }
}
void Transform::update(bool flag){
    if(flag){
        M = glm::translate(M,glm::vec3(0.0f,0.2f,0.0f));
    }else{
        M = glm::translate(M,glm::vec3(0.0f,-0.2f,0.0f));
    }
}
void Transform::rotation(glm::mat4 C){
    //glm::vec3 trans = glm::vec3(M[3]);
	M = C*init;

	//if (i == 1)
		//M = glm::translate(glm::rotate(glm::radians(10*float(glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f)),glm::vec3(4.0f,0.0f,0.0f))*init;
		//M = glm::translate(glm::vec3(-6.5f, 6.5f, 0.0f))*glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f))*glm::rotate(glm::radians(float(glfwGetTime()*20)),glm::vec3(1.0f,0.0f,0.0f))*glm::scale(glm::vec3(2.0f, 8.0f, 4.0f));
	//else
		//M = glm::translate(glm::rotate(glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f))*M,trans);
}

