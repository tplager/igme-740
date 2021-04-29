#version 430

uniform mat4 modelMat;
uniform mat4 viewMat; 
uniform mat4 projMat; 
uniform vec3 light1Pos;
uniform vec3 light2Pos;
uniform vec3 eyePos; 

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 color;

struct light {
    vec3 direction; 
    vec3 ambient_color; 
    vec3 diffuse_color; 
    vec3 specular_color; 
    float attenuation_coeff; 
};

light lights[2]; 

light light0 = light(
  vec3(0.0, 0.0, 0.0), 
  vec3(0.0, 0.15, 0.0), 
  vec3(1.0, 1.0, 0.0), 
  vec3(1.0, 0.0, 0.0), 
  20
);
light light1 = light(
  vec3(0.0, 0.0, 0.0), 
  vec3(0.0, 0.0, 0.15), 
  vec3(1.0, 0.0, 1.0), 
  vec3(1.0, 0.0, 0.0), 
  20
);

void main() {
  vec4 altered_position = modelMat * vec4(vertex_position.xyz, 1); 

  light0.direction = normalize(light1Pos - altered_position.xyz); 
  light1.direction = normalize(light2Pos - altered_position.xyz); 

  lights[0] = light0; 
  lights[1] = light1; 
  
  vec3 viewDir = normalize(eyePos - altered_position.xyz); 

  vec3 total_color = vec3(0.0, 0.0, 0.0); 

  for (int i = 0; i < 2; i++) {
    float diffuse_intensity = max(dot(vertex_normal, lights[i].direction), 0.0);
    float specular_intensity = 0.0; 
    
    if (diffuse_intensity > 0) {
	  vec3 reflected = 2 * vertex_normal * (dot(vertex_normal, lights[i].direction)) - lights[i].direction; 
      //vec3 reflected = reflect(-lights[i].direction, vertex_normal); 
      //vec3 viewDir = normalize(-test.xyz);
      float specAngle = max(dot(reflected, viewDir), 0.0);
      specular_intensity = pow(specAngle, lights[i].attenuation_coeff);
    }

    vec3 color_intensity = lights[i].ambient_color + (diffuse_intensity * lights[i].diffuse_color) + (specular_intensity * lights[i].specular_color);

    total_color += (color_intensity * 0.5); 
  }

  color = total_color; 
	
  gl_Position = projMat * viewMat * modelMat * vec4(vertex_position, 1.0f);
}


