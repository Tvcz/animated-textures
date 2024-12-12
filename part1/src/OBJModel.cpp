// Third Party Libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// header
#include <OBJModel.hpp>

// C++ Standard Template Library (STL)
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

OBJModel::OBJModel(const std::string &filename)
{
    if (filename.find("/") != std::string::npos) {
        mDirectoryPath = filename.substr(0, filename.find_last_of("/"));
    } else {
        mDirectoryPath = ".";
    }
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << filename << std::endl;
        exit(1);
    }
    std::string line;
    while (getline(file, line))
    {
        handleObjLine(line);
    }
    readFacesToBufferData();
}

void OBJModel::handleObjLine(std::string line)
{
    int spaceIndex = line.find(" ");
    std::string firstWord = line.substr(0, spaceIndex);
    std::string restOfLine = line.substr(spaceIndex + 1);
    if (firstWord == "v")
    {
        handleVertexString(restOfLine);
    }
    else if (firstWord == "vt")
    {
        handleTextureString(restOfLine);
    }
    else if (firstWord == "vn")
    {
        handleNormalString(restOfLine);
    }
    else if (firstWord == "f")
    {
        handleFaceString(restOfLine);
    }
    else if (firstWord == "mtllib")
    {
        handleMaterialLib(restOfLine);
    }
    else
    {
        // includes comments, empty lines, ignored commands, etc.
    }
}

void OBJModel::handleCoordsString(std::string coordsString, std::vector<float> &coords)
{
    int spaceIndex = coordsString.find(" ");
    std::string xString = coordsString.substr(0, spaceIndex);
    std::string restOfLine = coordsString.substr(spaceIndex + 1);
    spaceIndex = restOfLine.find(" ");
    std::string yString = restOfLine.substr(0, spaceIndex);
    std::string zString = restOfLine.substr(spaceIndex + 1);
    coords.push_back(std::stof(xString));
    coords.push_back(std::stof(yString));
    coords.push_back(std::stof(zString));
}

void OBJModel::handleVertexString(std::string vertexString)
{
    handleCoordsString(vertexString, mReadVertices);
}

void OBJModel::handleNormalString(std::string normalString)
{
    handleCoordsString(normalString, mReadNormals);
}

void OBJModel::handleTextureString(std::string textureString)
{
    int spaceIndex = textureString.find(" ");
    std::string sString = textureString.substr(0, spaceIndex);
    std::string tString = textureString.substr(spaceIndex + 1);
    mReadTextures.push_back(std::stof(sString));
    mReadTextures.push_back(std::stof(tString));
}

FaceVertex OBJModel::generateFaceVertexFromString(std::string vertexString)
{
    // split vertex to get position index
    int slashIndex = vertexString.find("/");
    std::string positionIndexString = vertexString.substr(0, slashIndex);
    std::string restOfVertexString = vertexString.substr(slashIndex + 1);
    // split rest of vertex to get texture and normal index
    slashIndex = restOfVertexString.find("/");
    if (slashIndex == std::string::npos)
    {
        slashIndex = restOfVertexString.length();
    }
    std::string textureIndexString = restOfVertexString.substr(0, slashIndex);
    std::string normalIndexString = restOfVertexString.substr(slashIndex + 1);
    // build struct for vertex
    FaceVertex faceVertex;

    // handle negative (relative) indices
    int positionIndex = std::stoi(positionIndexString);
    if (positionIndex < 0)
    {
        positionIndex = mReadVertices.size() / 3 + positionIndex + 1;
    }
    if (textureIndexString.empty())
    {
        textureIndexString = "0";
    }
    int textureIndex = std::stoi(textureIndexString);
    if (textureIndex < 0)
    {
        textureIndex = mReadTextures.size() / 3 + positionIndex + 1;
    }
    if (normalIndexString.empty())
    {
        normalIndexString = "0";
    }
    int normalIndex = std::stoi(normalIndexString);
    if (normalIndex < 0)
    {
        normalIndex = mReadNormals.size() / 3 + normalIndex + 1;
    }

    faceVertex.positionIndex = positionIndex;
    faceVertex.textureIndex = textureIndex;
    faceVertex.normalIndex = normalIndex;
    return faceVertex;
}

void OBJModel::handleFaceString(std::string faceString)
{
    Face face;
    std::istringstream stream(faceString);
    std::string vertexString;

    while (stream >> vertexString)
    {
        FaceVertex faceVertex = generateFaceVertexFromString(vertexString);
        face.vertices.push_back(faceVertex);
    }

    mReadFaces.push_back(face);
}

void OBJModel::handleMaterialLib(std::string materialFilename) {
    std::string materialPath = mDirectoryPath + "/" + materialFilename;
    std::cout << "Material path: " << materialPath << std::endl;
    std::ifstream materialFile(materialPath);
    mTextureFilename = "";
    mNormalMapFilename = "";
    if (materialFile.is_open())
    {
        std::string line;
        while (getline(materialFile, line))
        {
            if (line.find("map_Kd") != std::string::npos)
            {
                int spaceIndex = line.find(" ");
                std::string partialFilename = line.substr(spaceIndex + 1);
                mTextureFilename = mDirectoryPath + "/" + partialFilename;
                std::cout << "Texture filename: " << mTextureFilename << std::endl;
            } else if (line.find("map_Bump") != std::string::npos) {
                int spaceIndex = line.find(" ");
                std::string partialFilename = line.substr(spaceIndex + 1);
                mNormalMapFilename = mDirectoryPath + "/" + partialFilename;
                std::cout << "Normal map filename: " << mNormalMapFilename << std::endl;
            }
        }
        if (mTextureFilename == "") {
            std::cerr << "Could not find texture filename in material file: " << materialFilename << std::endl;
            exit(1);
        } else if (mNormalMapFilename == "") {
            std::cerr << "Could not find normal map filename in material file: " << materialFilename << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Could not open material file: " << materialPath << std::endl;
        exit(1);
    }
}

void OBJModel::addVertexTangentAndBitangentData(std::vector<int> &vertexIndicesInFace) {
    // code adapted from https://learnopengl.com/Advanced-Lighting/Normal-Mapping
    // calculate edges and deltas
    Vertex vert1 = mVertexToBufferedDataDelayed[vertexIndicesInFace[0]];
    Vertex vert2 = mVertexToBufferedDataDelayed[vertexIndicesInFace[1]];
    Vertex vert3 = mVertexToBufferedDataDelayed[vertexIndicesInFace[2]];
    glm::vec3 edge1 = glm::vec3(vert2.x - vert1.x, vert2.y - vert1.y, vert2.z - vert1.z);
    glm::vec3 edge2 = glm::vec3(vert3.x - vert1.x, vert3.y - vert1.y, vert3.z - vert1.z);
    glm::vec2 deltaUV1 = glm::vec2(vert2.s - vert1.s, vert2.t - vert1.t);
    glm::vec2 deltaUV2 = glm::vec2(vert3.s - vert1.s, vert3.t - vert1.t);

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    // calculate tangent
    glm::vec3 tangent;
    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    // calculate bitangent
    glm::vec3 bitangent;
    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    
    // add tangent and bitangent to vertex structs
    for (int index : vertexIndicesInFace) {
        Vertex &vertex = mVertexToBufferedDataDelayed[index];
        vertex.tx = tangent.x;
        vertex.ty = tangent.y;
        vertex.tz = tangent.z;
        vertex.bx = bitangent.x;
        vertex.by = bitangent.y;
        vertex.bz = bitangent.z;
    }
}

void OBJModel::readFacesToBufferData()
{
    for (Face face : mReadFaces)
    {
        std::vector<int> vertexIndicesInFace = {};
        std::vector<GLuint> indicesInFace = {};
        for (FaceVertex vertex : face.vertices)
        {
            int index = findOrCreateVertex(vertex.positionIndex, vertex.textureIndex, vertex.normalIndex);
            mIndexBufferData.push_back(index);
            vertexIndicesInFace.push_back(index);
        }
        addVertexTangentAndBitangentData(vertexIndicesInFace);
    }
    for (Vertex &vertex : mVertexToBufferedDataDelayed) {
        addVertexToBufferData(vertex);
    }
}

int OBJModel::findOrCreateVertex(int positionIndex, int textureIndex, int normalIndex)
{
    std::string key = std::to_string(positionIndex) + std::to_string(textureIndex) + std::to_string(normalIndex);
    if (mVertexMap.find(key) != mVertexMap.end())
    {
        return mVertexMap.at(key);
    }
    else
    {
        Vertex vertex = createVertex(positionIndex, textureIndex, normalIndex);
        int index = addVertexToBufferDataDelayed(vertex);
        mVertexMap[key] = index;
        return index;
    }
}

int OBJModel::addVertexToBufferDataDelayed(Vertex &vertex) {
    mVertexToBufferedDataDelayed.push_back(vertex);
    return mVertexToBufferedDataDelayed.size() - 1;
}

void OBJModel::addVertexToBufferData(Vertex &vertex)
{
    mVertexData.push_back(vertex.x);
    mVertexData.push_back(vertex.y);
    mVertexData.push_back(vertex.z);
    mVertexData.push_back(vertex.r);
    mVertexData.push_back(vertex.g);
    mVertexData.push_back(vertex.b);
    mVertexData.push_back(vertex.nx);
    mVertexData.push_back(vertex.ny);
    mVertexData.push_back(vertex.nz);
    mVertexData.push_back(vertex.s);
    mVertexData.push_back(vertex.t);
    mVertexData.push_back(vertex.tx);
    mVertexData.push_back(vertex.ty);
    mVertexData.push_back(vertex.tz);
    mVertexData.push_back(vertex.bx);
    mVertexData.push_back(vertex.by);
    mVertexData.push_back(vertex.bz);
}

Vertex OBJModel::createVertex(int positionIndex, int textureIndex, int normalIndex)
{
    Vertex vertex;
    int adjustedPositionIndex = (positionIndex - 1) * 3;
    vertex.x = mReadVertices[adjustedPositionIndex];
    vertex.y = mReadVertices[adjustedPositionIndex + 1];
    vertex.z = mReadVertices[adjustedPositionIndex + 2];
    if (normalIndex != 0)
    {
        // using vector normals to color cause cooler
        int adjustedNormalIndex = (normalIndex - 1) * 3;
        vertex.r = mReadNormals[adjustedNormalIndex];
        vertex.g = mReadNormals[adjustedNormalIndex + 1];
        vertex.b = mReadNormals[adjustedNormalIndex + 2];
        vertex.nx = mReadNormals[adjustedNormalIndex];
        vertex.ny = mReadNormals[adjustedNormalIndex + 1];
        vertex.nz = mReadNormals[adjustedNormalIndex + 2];
    }
    else
    {
        // handle unspecified normals
        vertex.r = 1.0f;
        vertex.g = 1.0f;
        vertex.b = 1.0f;
        vertex.nx = 0.0f;
        vertex.ny = 0.0f;
        vertex.nz = 0.0f;
    }
    if (textureIndex != 0)
    {
        int adjustedTextureIndex = (textureIndex - 1) * 2;
        vertex.s = mReadTextures[adjustedTextureIndex];
        vertex.t = mReadTextures[adjustedTextureIndex + 1];
    }
    else
    {
        // handle unspecified texture
        vertex.s = 0.0f;
        vertex.t = 0.0f;
    }
    return vertex;
}

std::vector<GLfloat> OBJModel::readVertexData()
{
    return mVertexData;
}

std::vector<GLuint> OBJModel::readIndexBufferData()
{
    return mIndexBufferData;
}

std::string OBJModel::readTextureFilename()
{
    return mTextureFilename;
}

std::string OBJModel::readNormalMapFilename() {
    return mNormalMapFilename;
}
