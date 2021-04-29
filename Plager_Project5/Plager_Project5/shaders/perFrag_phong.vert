#version 430

uniform mat4 modelMat;
uniform mat4 viewMat; 
uniform mat4 projMat; 
uniform vec3 light1Pos;
uniform vec3 light2Pos;
uniform vec3 eyePos; 

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 normal;
out vec3 light1Dir;
out vec3 light2Dir; 
out vec3 viewDir; 

out vec4 test; 

void main() {
  vec4 altered_position = modelMat * vec4(vertex_position.xyz, 1); 

  light1Dir = normalize(light1Pos - altered_position.xyz);
  light2Dir = normalize(light2Pos - altered_position.xyz);
  viewDir = normalize(eyePos - altered_position.xyz); 
  normal = vertex_normal; 

  test = altered_position; 
	
  gl_Position = projMat * viewMat * modelMat * vec4(vertex_position, 1.0f);
}




