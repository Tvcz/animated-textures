#include "Light.hpp"
#include <vector>
#include <iostream>

Light::Light(float radius, float speed, GLuint index)
{
    m_radius = radius;
    m_speed = speed;
    m_theta = index * 45.0f; // degrees
    m_color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_VAO = index;
    m_VBO = index;
    m_IBO = index;
}

GLuint Light::GetVAO()
{
    return m_VAO;
}

GLuint Light::GetVBO()
{
    return m_VBO;
}

GLuint Light::GetIBO()
{
    return m_IBO;
}

void Light::Update(float deltaTime)
{
    m_theta += m_speed * deltaTime;
    if (m_theta > 360.0f)
    {
        m_theta -= 360.0f;
    }
}

glm::vec3 Light::GetPosition()
{
    float rads = glm::radians(m_theta);
    float x = m_radius * cos(rads);
    float z = m_radius * sin(rads);
    return glm::vec3(x, 0.0f, z);
}

glm::vec3 Light::GetColor()
{
    return m_color;
}

struct Position {
    float x;
    float y;
    float z;
};

glm::vec3 calculateNormalForCubeVertex(const Position& vertex) {
    glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
    if (vertex.x > 0) {
        normal.x = 1.0f;
    } else if (vertex.x < 0) {
        normal.x = -1.0f;
    }

    if (vertex.y > 0) {
        normal.y = 1.0f;
    } else if (vertex.y < 0) {
        normal.y = -1.0f;
    }

    if (vertex.z > 0) {
        normal.z = 1.0f;
    } else if (vertex.z < 0) {
        normal.z = -1.0f;
    }

    return normal;
}

std::vector<GLfloat> Light::GetVertexData()
{
    std::vector<Position> vertices = {
        {-0.1f, 0.1f, 0.1f},
        {-0.1f, -0.1f, 0.1f},
        {0.1f, -0.1f, 0.1f},
        {0.1f, 0.1f, 0.1f},
        {-0.1f, 0.1f, -0.1f},
        {-0.1f, -0.1f, -0.1f},
        {0.1f, -0.1f, -0.1f},
        {0.1f, 0.1f, -0.1f}
    };

    glm::vec3 lightPos = GetPosition();
    std::vector<GLfloat> vertexData;
    for (const auto& vertex : vertices)
    {
        vertexData.push_back(vertex.x + lightPos.x);
        vertexData.push_back(vertex.y + lightPos.y);
        vertexData.push_back(vertex.z + lightPos.z);
        vertexData.push_back(m_color.r);
        vertexData.push_back(m_color.g);
        vertexData.push_back(m_color.b);
        glm::vec3 normal = calculateNormalForCubeVertex(vertex);
        vertexData.push_back(normal.x);
        vertexData.push_back(normal.y);
        vertexData.push_back(normal.z);
    }
    return vertexData;
}

std::vector<GLuint> Light::GetIndexBufferData()
{
    return {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        2, 3, 7,
        7, 6, 2,
        0, 1, 5,
        5, 4, 0,
        0, 3, 7,
        7, 4, 0,
        1, 2, 5,
        5, 6, 2
    };
}

void Light::PrepBuffers() {
    std::vector<GLfloat> vertices = GetVertexData();
    std::vector<GLuint> indices = GetIndexBufferData();

    // Vertex Arrays Object (VAO) creation
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Vertex Buffer Object (VBO) creation
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Populate our Vertex Buffer
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size()*sizeof(GLfloat), 
                 vertices.data(), 
                 GL_STATIC_DRAW);

    // Index Buffer Object (IBO) creation
    glGenBuffers(1,&m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    // Populate our Index Buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size()*sizeof(GLuint),
                 indices.data(),
                 GL_STATIC_DRAW);

    // Setup Vertex Attributes
    // position (x,y,z)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, (void*)0);

    // color (r,g,b)
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, (void*)(sizeof(GL_FLOAT)*3));
    
    // normal (nx,ny,nz)
    glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, (void*)(sizeof(GL_FLOAT)*6));

    glBindVertexArray(0);    
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Light::UpdateBuffers() {
    std::vector<GLfloat> vertices = GetVertexData();

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size()*sizeof(GLfloat), 
                 vertices.data(), 
                 GL_STATIC_DRAW);
}