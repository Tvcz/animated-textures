#version 410 core
// From Vertex Buffer Object (VBO)
// The only thing that can come 'in', that is
// what our shader reads, the first part of the
// graphics pipeline.
layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColor;
layout(location=2) in vec3 vertexNormal;
layout(location=3) in vec2 textureCoordinates;
layout(location=4) in vec3 tangent;
layout(location=5) in vec3 bitangent;

// Uniform variables
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection; // We'll use a perspective projection

// Pass data into the fragment shader
out vec3 FragPos;
out vec3 v_vertexColor;
out vec3 v_vertexNormal;
out vec2 v_textureCoordinates;
out mat3 TBN;

void main()
{
  v_vertexColor = vertexColor;
  v_textureCoordinates = textureCoordinates;
  // TODO: move to CPU
  v_vertexNormal = mat3(transpose(inverse(u_ModelMatrix))) * vertexNormal;
  FragPos = vec3(u_ModelMatrix * vec4(position, 1.0f));

  // Calculate TBN matrix (taken from learnopengl.com tutorial)
  vec3 T = normalize(vec3(u_ModelMatrix * vec4(tangent,   0.0)));
  vec3 B = normalize(vec3(u_ModelMatrix * vec4(bitangent, 0.0)));
  vec3 N = normalize(vec3(u_ModelMatrix * vec4(vertexNormal,    0.0)));
  TBN = mat3(T, B, N); // Correctly assign to the output variable

  vec4 newPosition = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0f);
	gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w);
}


