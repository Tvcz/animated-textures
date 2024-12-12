#pragma once

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>

struct Vertex
{
    float x, y, z;    // position
    float r, g, b;    // color
    float nx, ny, nz; // normals
    float s, t;       // texture
    float tx, ty, tz; // tangent
    float bx, by, bz; // bitangent
};

struct FaceVertex
{
    int positionIndex;
    int textureIndex;
    int normalIndex;
};

struct Face
{
    std::vector<FaceVertex> vertices;
};

class OBJModel
{
public:
    OBJModel(const std::string &filename);
    std::vector<GLfloat> readVertexData();
    std::vector<GLuint> readIndexBufferData();
    std::string readTextureFilename();
    std::string readNormalMapFilename();

private:
    void handleObjLine(std::string line);
    void handleCoordsString(std::string coordsString, std::vector<float> &coords);
    void handleVertexString(std::string vertexString);
    void handleNormalString(std::string normalString);
    void handleTextureString(std::string textureString);
    FaceVertex generateFaceVertexFromString(std::string vertexString);
    void handleFaceString(std::string faceString);
    void handleMaterialLib(std::string materialFilename);
    void addVertexTangentAndBitangentData(std::vector<int> &vertexIndicesInFace);
    void readFacesToBufferData();
    int findOrCreateVertex(int positionIndex, int textureIndex, int normalIndex);
    int addVertexToBufferDataDelayed(Vertex &vertex);
    void addVertexToBufferData(Vertex &vertex);
    Vertex createVertex(int positionIndex, int textureIndex, int normalIndex);
    std::vector<float> mReadVertices;
    std::vector<float> mReadNormals;
    std::vector<float> mReadTextures;
    std::vector<Face> mReadFaces;
    std::vector<GLfloat> mVertexData;
    std::vector<GLuint> mIndexBufferData;
    std::string mTextureFilename;
    std::string mNormalMapFilename;
    std::map<std::string, int> mVertexMap;
    std::string mDirectoryPath;
    std::vector<Vertex> mVertexToBufferedDataDelayed;
};