
#include "meshes.h"

/* Vertex constructor. NB: Vertices are NOT Vectors, however, Vertices CONTAIN Vectors */
Vertex::Vertex(const Vector &pos = Vector(), const Vector &rgb = Vector(), 
               const Vector &norm = Vector())
    : position(pos), normal(norm), color(rgb)
{}

/* Mesh constructor. Every parameter is required */
Mesh::Mesh()
    : vertex_count(0), index_count(0), buffers{0,0}, vao(0), locMVP(0)
{
    // OpenGL objects need to be allocated on the GPU
    glGenBuffers(2, &buffers[0]);
    glGenVertexArrays(1, &vao);
    program = glCreateProgram();
}

/* This function makes creating a shader program easier than pouring water out of a boot.
 * Even if there were directions on the heel.
 *
 * The two parameters are the filesystem path to the vertex shader and fragment shader, respectively,
 * which compose the program to be created
 */
void Mesh::loadProgram(std::string vs_path, std::string fs_path)
{
    // Create shader objects
    GLuint vs, fs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);

    // Load source and compile each shader
    if(!Mesh::loadShaderFile(vs_path, vs))
        return;
    if(!Mesh::loadShaderFile(fs_path, fs))
        return;

    // Link the program
    std::cout << "Creating program object" << std::endl;
    glAttachShader(this->program, vs);
    glAttachShader(this->program, fs);
    std::cout << "Linking program" << std::endl;
    glLinkProgram(this->program);

    //Check for link errors
    GLint linked;
    glGetProgramiv(this->program, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLint blen = 0;
        GLsizei slen = 0;

        std::cout << "Linking failed: " << std::endl;
        glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &blen);

        if(blen > 1)
        {
            GLchar* link_log = new GLchar[blen];

            glGetProgramInfoLog(this->program, blen, &slen, link_log);
            std::cout << link_log << std::endl;

            delete[] link_log;
        }
    }

    std::cout << "Linking succeeded" << std::endl;
    glDeleteShader(vs);
    glDeleteShader(fs);
}

/* One of the components of the cheater-mode program creation function, this function
 * loads a shader from disk and compiles it on the GPU
 */
bool Mesh::loadShaderFile(std::string path, GLuint shader)
{
    // Load shader source from text file
    std::cout << "Opening shader source file (" << path << ") for read operation" << std::endl;
    std::ifstream in(path);
    if(!in.good())
    {
        std::cout << "Failed to open file " << path << std::endl;
        return false;
    }
    std::string source(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    in.close();
    std::cout << "Got shader source" << std::endl;
    
    // Move shader source to GPU
    std::cout << "Loading shader source" << std::endl;
    const char *source_ptr = source.c_str();
    glShaderSource(shader, 1,&source_ptr, NULL);
    std::cout << "Compiling shader" << std::endl;
    glCompileShader(shader);

    // Compile shader, display any errors
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        std::cout << "Shader compilation failure" << std::endl;
        GLint blen = 0;
        GLsizei slen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &blen);

        if(blen > 1)
        {
            GLchar* compiler_log = new GLchar[blen];

            glGetShaderInfoLog(shader, blen, &slen, compiler_log);
            std::cout << compiler_log << std::endl << std::endl;

            delete[] compiler_log;
        }
        else
        {
            std::cout << "No compiler log available" << std::endl;
        }
        return false;
    }

    std::cout << "Shader compilation succeeded" << std::endl;
    return true;
}
bool Mesh::loadTextureFile(std::string path)
{
    TGAHeader header;
    std::cout << "Opening texture file (" << path << ")" <<std::endl;
    std::ifstream in(path);
    if(!in.good())
    {
        std::cout << "Failed to open file";
        return false;
    }
    in >> header;
    in.close();
    std::cout << "Texture complilation succeded" << std::endl;
    std::cout << "ID length:" << static_cast<int>(header.id_len) << std::endl;
    std::cout << "Color Map type:" << static_cast<int>(header.color_map_type) << std::endl;
    std::cout << "Image type:" << static_cast<int>(header.image_type) << std::endl; 
    return true;
}

/* This function takes a bunch of raw data in memory, and organizes it into a Mesh object.
 */
void Mesh::loadData(Vertex *verts, int vert_count, short *index_array, int index_count)
{
    // Store the number of vertices and the number of indices in the mesh object
    vertex_count = vert_count;
    this->index_count = index_count;

    // Bind vertex array, vertex buffer, element buffer
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTEX_BUFFER_INDEX]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[ELEMENT_BUFFER_INDEX]);

    // Initialize the buffers by loading in data
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vert_count, (void *) verts, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * index_count, (void *)index_array, 
                 GL_STATIC_DRAW);

    // Tell OpenGL about the layout of the data in the vertex buffer, map attributes to shader 
    // indexes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) sizeof(Vector));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(Vector)));
    
    // Enable vertex attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // unbind the buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
}

/* Thanks to the magic of programmable shader hardware, every mesh can be drawn just about the same
 * way. At least for simple meshes. 
 * NOT IMPLEMENTED: instanced rendering
 * NOT IMPLEMENTED: WON'T DO: deferred rendering
 */
void Mesh::draw(glm::mat4 mvp)
{
    // Select shader, load uniform data
    glUseProgram(program);
    locMVP = glGetUniformLocation(program, "mvp");
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));

    // Bind this mesh's VAO and issue draw command
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, index_count , GL_UNSIGNED_SHORT, (GLvoid*) 0);

    // Unbind VAO and program
    glBindVertexArray(0);
    glUseProgram(0);

}

/* This function creates a cube mesh, perfect for testing */
void test_cube(Mesh &mesh)
{
    // Cubes have 8 vertices
    Vertex verts[8];

    verts[0].position = Vector(-1.0f,  1.0f,  1.0f);
    verts[1].position = Vector( 1.0f,  1.0f,  1.0f);
    verts[2].position = Vector( 1.0f, -1.0f,  1.0f);
    verts[3].position = Vector(-1.0f, -1.0f,  1.0f);
    verts[4].position = Vector(-1.0f,  1.0f, -1.0f);
    verts[5].position = Vector( 1.0f,  1.0f, -1.0f);
    verts[6].position = Vector( 1.0f, -1.0f, -1.0f);
    verts[7].position = Vector(-1.0f, -1.0f, -1.0f);

    verts[0].color = Vector(0.0f, 1.0f, 0.0f);
    verts[1].color = Vector(1.0f, 1.0f, 0.0f);
    verts[2].color = Vector(0.0f, 1.0f, 0.0f);
    verts[3].color = Vector(0.0f, 0.0f, 0.0f);
    verts[4].color = Vector(0.0f, 1.0f, 1.0f);
    verts[5].color = Vector(1.0f, 1.0f, 1.0f);
    verts[6].color = Vector(0.0f, 1.0f, 1.0f);
    verts[7].color = Vector(0.0f, 0.0f, 1.0f);

    for(int i = 0; i < 8; i++)
    {
        verts[i].normal = verts[i].position.normalize();
    }

    int index_length = 6*2*3;
    short indices[] = 
    {
        0, 3, 1,
        1, 3, 2,
        4, 1, 0,
        1, 4, 5,
        7, 5, 4,
        5, 6, 7,
        3, 0, 4,
        4, 7, 3,
        3, 7, 6,
        6, 2, 3,
        5, 1, 2,
        2, 6, 5
    };
    
    mesh.loadData(&verts[0], 8, &indices[0], index_length);
}

/* This function creates a sphere using the Latitude/Longitude method. Shitty model, but it's really
 * easy to figure out appropriate texture coordinates.
 */
void test_latlong_sphere(Mesh &mesh)
{
    const int lat_div = 36;
    const int long_div = 76;

    const int vert_count = lat_div * long_div;

    Vertex verts[vert_count];

    for(int i =0; i < vert_count; i++)
    {
        float phi = radians(360.0f / long_div * (i % long_div));
        float theta = radians(180.0f / lat_div * (i / long_div));
        
        verts[i].position = Vector(cos(theta) * cos(phi), cos(theta) * sin(phi), sin(theta));
        verts[i].position.x = verts[i].position.x * 2.0f - 1.0f;
        verts[i].position.y = verts[i].position.y * 2.0f - 1.0f;
        verts[i].position.z = verts[i].position.z * 2.0f - 1.0f;

        verts[i].color = Vector(cos(theta) * cos(phi), cos(theta) * sin(phi), sin(theta));
        verts[i].normal = verts[i].position.normalize();
    }

    // This is just a placeholder, clearly incorrect
    short indices[10];

    mesh.loadData(&verts[0], vert_count, &indices[0], 10);
}
