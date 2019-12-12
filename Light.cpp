#include "Light.h"

Light::Light(glm::vec3 location)
{
    /*
     * TODO: Section 2: Currently, all the points are hard coded below.
     * Modify this to read points from an obj file.
     */
    std::string objFilename = "sphere.obj";
    std::ifstream objFile(objFilename); // The obj file we are reading.
    std::vector<glm::vec3> locPoints;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> uVertices;
    std::vector<int> vIndices;
    std::vector<int> uvIndices;
    std::vector<int> nIndices;
    loc = location;
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
                normals.push_back(normal);
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
    
    for(glm::vec3 point:locPoints)
    {
        points.push_back(point);
    }
    locPoints = points;
    points.clear();
//    float scaling = std::min({1/xWid,1/yWid,1/zWid});
//    for(glm::vec3 point:locPoints)
//    {
//        point *= scaling;
//        points.push_back(point);
//    }
    for(int i=0;i<vIndices.size();i++){
        vertices.push_back(points[vIndices[i]-1]);
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

    /*
     * TODO: Section 4, you will need to normalize the object to fit in the
     * screen.
     */

    // Set the model matrix to an identity matrix.
    model = glm::mat4(1.0f);
    model = glm::translate(model, loc);
    model = glm::scale(model, glm::vec3(0.5f));
    // Set the color.
    color = glm::vec3(0.5f, 0.8f, 0.7f);

    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &lvao);
    glGenBuffers(1, &lvbo);

    // Bind to the VAO.
    glBindVertexArray(lvao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, lvbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
        &vertices[0], GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

Light::~Light()
{
    // Delete the VBO and the VAO.
    glDeleteBuffers(1, &lvbo);
    glDeleteVertexArrays(1, &lvao);
}

void Light::draw()
{
    // Bind to the VAO.
    glBindVertexArray(lvao);
    // Set point size.
    glPointSize(1);
    // Draw points
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void Light::update()
{
    // Spin the cube by 1 degree.
    //spin(0.1f);
}

glm::vec3 Light::returnLocation()
{
    return loc;
}

void Light::spin(float angle, glm::vec3 axis)
{
    // Update the model matrix by multiplying a rotation matrix
    model = glm::rotate(glm::radians(angle), axis)*model;
    loc = glm::mat3(glm::rotate(glm::radians(angle), axis))*loc;
}

void Light::updatePointSizeL(){
}
void Light::updatePointSizeS(){
}
void Light::updateModelSize(double offset){
    if (offset>0){
        loc = loc+glm::normalize(loc)*0.05f;
        model = glm::translate(loc);
        model = glm::scale(model, glm::vec3(0.5f));
    }else if (offset<0){
        loc = loc-glm::normalize(loc)*0.05f;
        model = glm::translate(loc);
        model = glm::scale(model, glm::vec3(0.5f));
    }
}
