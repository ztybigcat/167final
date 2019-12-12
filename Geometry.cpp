//
//  Geometry.cpp
//  CSE167 P2
//
//  Created by Yichen Zhang on 10/28/19.
//  Copyright © 2019 Yichen Zhang. All rights reserved.
//

#include "Geometry.h"
Geometry::Geometry(std::string objFilename, GLfloat pointSize, glm::vec3 co)
    : pointSize(pointSize)
{
    std::ifstream objFile(objFilename); // The obj file we are reading.
    std::vector<glm::vec3> locPoints;
    std::vector<glm::vec3> uVertices;
    std::vector<glm::vec3> tempNormals;
    std::vector<int> vIndices;
    std::vector<int> uvIndices;
    std::vector<int> nIndices;
	color = co;
    // Check whether the file can be opened.
    if (objFile.is_open())
    {
        std::string line; // A line in the file.

        // Read lines from the file.
        while (std::getline(objFile, line))
        {
            // Turn the line into a string stream for processing.
            std::stringstream ss;
            ss << line;
            
            // Read the first word of the line.
            std::string label;
            ss >> label;

            // If the line is about vertex (starting with a "v").
            if (label == "v")
            {
                // Read the later three float numbers and use them as the
                // coordinates.
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;

                // Process the point. For example, you can save it to a.
                locPoints.push_back(point);
            }
            // If the line is about normals (starting with a "vn").
            if (label == "vn")
            {
                // Read the later three float numbers and use them as the
                // coordinates.
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;

                // Process the point. For example, you can save it to a.
                tempNormals.push_back(normal);
            }
            // If the line is about face (starting with a "f").
            if ( label == "f")
            {
                //Read
                std::string fStrings[3];
                int vec[3],uvec[3],nvec[3];
                ss >> fStrings[0] >> fStrings[1] >> fStrings[2];
                for(int i=0;i<3;i++)
                {
                    sscanf(fStrings[i].c_str(),"%d/%d/%d", &vec[i], &uvec[i], &nvec[i]);
                    vIndices.push_back(vec[i]);
                    uvIndices.push_back(uvec[i]);
                    nIndices.push_back(nvec[i]);
                }
            }
        }
    }
    else
    {
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }

    objFile.close();
    
//    float xmax = locPoints[0].x;
//    float xmin = locPoints[0].x;
//    float ymax = locPoints[0].y;
//    float ymin = locPoints[0].y;
//    float zmax = locPoints[0].z;
//    float zmin = locPoints[0].z;
//    for(glm::vec3 point:locPoints)
//    {
//        xmax = point.x > xmax ? point.x : xmax;
//        xmin = point.x < xmin ? point.x : xmin;
//        ymax = point.y > ymax ? point.y : ymax;
//        ymin = point.y < ymin ? point.y : ymin;
//        zmax = point.z > zmax ? point.z : zmax;
//        zmin = point.z < zmin ? point.z : zmin;
//    }
//    glm::vec3 centPoint;
//    float xcent = (xmax+xmin)/2;
//    float ycent = (ymax+ymin)/2;
//    float zcent = (zmax+zmin)/2;
//    float xWid = xmax-xmin;
//    float yWid = ymax-ymin;
//    float zWid = zmax-zmin;
//    centPoint.x = xcent;
//    centPoint.y = ycent;
//    centPoint.z = zcent;
//    for(glm::vec3 point:locPoints)
//    {
//        point -= centPoint;
//        points.push_back(point);
//    }
//    locPoints = points;
//    points.clear();
//    float scaling = std::min({20/xWid,17/yWid,20/zWid});
    for(glm::vec3 point:locPoints)
    {
//        point *= scaling;
        points.push_back(point);
    }
    for(int i=0;i<vIndices.size();i++){
        vertices.push_back(points[vIndices[i]-1]);
        //uVertices.push_back(points[uvIndices[i]-1]);
        normals.push_back(tempNormals[nIndices[i]-1]);
    }
//    points =
//    {
//        glm::vec3(-2.5, 2.5, 2.5),
//        glm::vec3(-2.5, -2.5, 2.5),
//        glm::vec3(2.5, -2.5, 2.5),
//        glm::vec3(2.5, 2.5, 2.5),
//        glm::vec3(-2.5, 2.5, -2.5),
//        glm::vec3(-2.5, -2.5, -2.5),
//        glm::vec3(2.5, -2.5, -2.5),
//        glm::vec3(2.5, 2.5, -2.5)
//    };

    // Set the model matrix to an identity matrix.
    model = glm::mat4(1);

    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
        &vertices[0], GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

Geometry::~Geometry()
{
    // Delete the VBO and the VAO.
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Geometry::rendering(){
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Set point size.
    glPointSize(pointSize);
    // Draw points
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void Geometry::setModelMatrixM(glm::mat4 M){
    model = M;
}

void Geometry::draw(glm::mat4 M, GLuint program){
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(color));
    setModelMatrixM(M);
    rendering();
}

void Geometry::update(bool flag){}
