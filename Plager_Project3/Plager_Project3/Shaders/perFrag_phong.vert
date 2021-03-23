#version 430

uniform mat4 modelMat;
uniform mat4 viewMat; 
uniform mat4 projMat; 
uniform vec3 light1Pos;
uniform vec3 light2Pos;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 color;
out vec3 normal;
out vec3 light1Dir;
out vec3 light2Dir; 
out vec3 viewDir; 

void main() {
  vec3 eyePos = viewMat[3].xyz; 

  light1Dir = normalize(light1Pos - vertex_position);
  light2Dir = normalize(light2Pos - vertex_position);
  viewDir = normalize(eyePos - vertex_position); 
  normal = vertex_normal; 
  color = vec3(1.0, 1.0, 0.0);
	
  gl_Position = projMat * viewMat * modelMat * vec4(vertex_position, 1.0f);
}




