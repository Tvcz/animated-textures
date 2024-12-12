#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include <vector>

class Light {
    public:
        Light(float radius, float speed, GLuint index);
        GLuint GetVAO();
        GLuint GetVBO();
        GLuint GetIBO();
        void Update(float deltaTime);
        glm::vec3 GetPosition();
        glm::vec3 GetColor();
        std::vector<GLfloat> GetVertexData();
        std::vector<GLuint> GetIndexBufferData();
        void PrepBuffers();
        void UpdateBuffers();

    private:
        GLuint m_VAO;
        GLuint m_VBO;
        GLuint m_IBO;
        float m_radius;
        float m_speed;
        float m_theta;
        glm::vec3 m_color;
};