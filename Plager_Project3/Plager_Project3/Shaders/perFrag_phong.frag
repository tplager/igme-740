#version 430
in vec3 color;
in vec3 normal;
in vec3 light1Dir; 
in vec3 light2Dir; 
in vec3 viewDir; 

in vec3 test; 

out vec4 frag_color;

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
  light0.direction = light1Dir; 
  light1.direction = light2Dir; 

  lights[0] = light0; 
  lights[1] = light1; 

  vec3 total_color = vec3(0.0, 0.0, 0.0); 

  for (int i = 0; i < 2; i++) {
    //vec3 reflected = 2 * normal * (dot(normal, lights[i].direction)) - lights[i].direction; 

    float diffuse_intensity = max(dot(normal, lights[i].direction), 0.0);
    float specular_intensity = 0.0; 
    
    if (diffuse_intensity > 0) {
      vec3 reflected = reflect(-lights[i].direction, normal); 
      vec3 viewDir = normalize(-test);
      float specAngle = max(dot(reflected, viewDir), 0.0);
      specular_intensity = pow(specAngle, lights[i].attenuation_coeff);
    }

    vec3 color_intensity = lights[i].ambient_color + (diffuse_intensity * lights[i].diffuse_color) + (specular_intensity * lights[i].specular_color);

    total_color += (color_intensity * 0.5); 
  }

  
  frag_color = vec4(total_color, 1.0f);
}