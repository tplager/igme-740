#include "RecPrism.h"
#include <vector>

RecPrism::RecPrism()
{
    minPos = vec3(0.0);
    maxPos = vec3(1.0);
    color = vec3(1.0);

    mat4 rotMat = mat4(0.0f); 
    mat4 invRotMat = mat4(0.0f); 

    ambient = 1.0f;
    diffuse = 1.0f;
    phong = 1.0f;
}

RecPrism::~RecPrism()
{
    delete[] vertices; 
}

void RecPrism::init(vec3 minP, vec3 maxP, vec3 c, mat4 rM, mat4 iRM, float a, float d, float p)
{
    minPos = minP;
    maxPos = maxP;
    color = c;

    rotMat = rM;
    invRotMat = iRM;

    ambient = a;
    diffuse = d;
    phong = p;

    vert_num = 0;
    vao = vbo = nbo = ibo = 0;

    cShaderProg = ShaderProgram();
    vfShaderProg = ShaderProgram();

    cShader = ShaderClass();
    vShader = ShaderClass();
    fShader = ShaderClass();
}

void RecPrism::create(const char* v_shader_file, const char* f_shader_file, const char* c_shader_file)
{
    vector<uvec3> ori_vertices; 

    // Top face (y = 1.0f)
    // Counter-clockwise (CCW) with normal pointing out
    ori_vertices.push_back(uvec3(maxPos.x, maxPos.y, minPos.z));
    ori_vertices.push_back(uvec3(minPos.x, maxPos.y, minPos.z));
    ori_vertices.push_back(uvec3(minPos.x, maxPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(minPos.x, maxPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, maxPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, maxPos.y, minPos.z));

    // Bottom face (y = -1.0f)
    ori_vertices.push_back(uvec3(maxPos.x, minPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(minPos.x, minPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(minPos.x, minPos.y, minPos.z));
    ori_vertices.push_back(uvec3(minPos.x, minPos.y, minPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, minPos.y, minPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, minPos.y, maxPos.z));

    // Front face  (z = 1.0f)
    ori_vertices.push_back(uvec3(maxPos.x, maxPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(minPos.x, maxPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(minPos.x, minPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(minPos.x, minPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, minPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, maxPos.y, maxPos.z));

    // Back face (z = -1.0f)
    ori_vertices.push_back(uvec3(maxPos.x, minPos.y, minPos.z));
    ori_vertices.push_back(uvec3(minPos.x, minPos.y, minPos.z));
    ori_vertices.push_back(uvec3(minPos.x, maxPos.y, minPos.z));
    ori_vertices.push_back(uvec3(minPos.x, maxPos.y, minPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, maxPos.y, minPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, minPos.y, minPos.z));

    // Left face (x = -1.0f)
    ori_vertices.push_back(uvec3(minPos.x, maxPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(minPos.x, maxPos.y, minPos.z));
    ori_vertices.push_back(uvec3(minPos.x, minPos.y, minPos.z));
    ori_vertices.push_back(uvec3(minPos.x, minPos.y, minPos.z));
    ori_vertices.push_back(uvec3(minPos.x, minPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(minPos.x, maxPos.y, maxPos.z));

    // Right face (x = 1.0f)
    ori_vertices.push_back(uvec3(maxPos.x, maxPos.y, minPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, maxPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, minPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, minPos.y, maxPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, minPos.y, minPos.z));
    ori_vertices.push_back(uvec3(maxPos.x, maxPos.y, minPos.z));

    vert_num = ori_vertices.size();
    vertices = new vec3[vert_num];

    for (uint i = 0; i < vert_num; i++) {
        vertices[i] = ori_vertices[i];
    }

    // ********** load and set up shaders and shader programs **********
    // load the compute shader and link it to a shader program
    cShader.create(c_shader_file, GL_COMPUTE_SHADER);
    vShader.create(v_shader_file, GL_VERTEX_SHADER);
    fShader.create(f_shader_file, GL_FRAGMENT_SHADER);

    // create shader programs
    cShaderProg.create();
    vfShaderProg.create();

    // link shaders to a shader prorgam
    // Note: A Compute Shader must be in a shader program all by itself. 
    // There cannot be vertex, fragment, etc.shaders in a shader program with the compute shader.
    vfShaderProg.link(vShader); // link vertex shader 
    vfShaderProg.link(fShader); // link fragment shader
    cShaderProg.link(cShader); // link compute shader 

    // after linking shaders to the shader program, it is safter to destroy the shader as they're no longer needed
    vShader.destroy();
    fShader.destroy();
    cShader.destroy();

    // create vbo 
    // generate a new VBO and get the associated ID
    glGenBuffers(1, &vbo);

    // bind VBO in order to use
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // upload data to VBO - data went to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 12, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // clean up

    // Define VAO
    // There are 2 buffer objects (vertices and normals), which should be input "attribute" variables to the vertex shader. 
    // We set up the layout of both of these with vertex array object. 
    // The actual vertices and normal vectors have already been kept on the GPU memory by VBOs. 
    // The purpose of VAO is to have vertices and normal vectors as separate attributes in the vertex shader. 
    // So VAO's attrobites point to these data on the GPU, rather than referring back to any CPU data. 

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // 0 - the layout id in vertex shader

    // Ettributes are disabled by default in OpenGL 4. 
    // We have to explicitly enable each one.
    glEnableVertexAttribArray(0);
}

void RecPrism::draw(mat4 viewMat, mat4 projMat, vec3 lightPos, vec3 eyePos)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glUseProgram(vfShaderProg.id);

    //glMultMatrixf((const float*)glm::value_ptr(rotMat));

    vfShaderProg.setMatrix4fv("modelMat", 1, value_ptr(rotMat));
    vfShaderProg.setMatrix4fv("viewMat", 1, value_ptr(viewMat));
    vfShaderProg.setMatrix4fv("projMat", 1, value_ptr(projMat));
    vfShaderProg.setFloat3V("lightPos", 1, value_ptr(lightPos));
    vfShaderProg.setFloat3V("eyePos", 1, value_ptr(eyePos));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
    glPopMatrix();
}
