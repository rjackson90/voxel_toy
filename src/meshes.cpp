#include "meshes.h"

/* Vertex constructor. NB: Vertices are NOT Vectors, however, Vertices CONTAIN Vectors */
Vertex::Vertex(const Vector &pos = Vector(), const Vector &rgb = Vector(), 
               const Vector &norm = Vector())
    : position(pos), normal(norm), color(rgb)
{}

/* Mesh constructor. Every parameter is required */
Mesh::Mesh()
    : vertex_count(0), index_count(0), buffers{0,0}, vao(0), locMVP(0), locTexColor(0), 
      locTexNormal(0)
{
    // OpenGL objects need to be allocated on the GPU
    glGenBuffers(2, &buffers[0]);
    glGenVertexArrays(1, &vao);
    program = glCreateProgram();
    glGenTextures(2, &textures[0]);
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
    std::string source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
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

/* This function reads TGA images from disk and stores the contents in a texture object on the GPU
 */
bool Mesh::loadTextureFile(std::string path, GLuint texture)
{
    using std::cout;
    using std::endl;

    // Open file
    TGAHeader header;
    cout << "Opening texture file (" << path << ")" << endl;
    std::ifstream in(path, std::ifstream::binary | std::ifstream::in);
    if(!in.good())
    {
        cout << "Failed to open file";
        return false;
    }

    // Read header information from file
    char *buffer = new char[sizeof(short)];

    in.read(buffer, sizeof(char));
    header.id_len = *buffer;

    in.read(buffer, sizeof(char));
    header.color_map_type = *buffer;

    in.read(buffer, sizeof(char));
    header.image_type = *buffer;

    in.read(buffer, sizeof(short));
    header.f_index = *((short*)buffer);

    in.read(buffer, sizeof(short));
    header.m_len = *((short*)buffer);

    in.read(buffer, sizeof(char));
    header.e_size = *buffer;

    in.read(buffer, sizeof(short));
    header.x_origin = *((short*)buffer);

    in.read(buffer, sizeof(short));
    header.y_origin = *((short*)buffer);

    in.read(buffer, sizeof(short));
    header.width = *((short*)buffer);

    in.read(buffer, sizeof(short));
    header.height = *((short*)buffer);

    in.read(buffer, sizeof(char));
    header.depth = *buffer;

    in.read(buffer, sizeof(char));
    header.descriptor = *buffer;

    delete[] buffer;

    
    /* Check this image to ensure compliance with established technical requirements:
     *  - Square image with side length equal to a power of two.
     *  - No color map, image should be uncompressed true color
     */
    if(header.id_len > 0)
    {
        char *id = new char[header.id_len];
        in.read(id, header.id_len);
        cout << "Image ID: " << id << endl;
        delete[] id;
    }
    if(header.color_map_type != 0)
    {
        std::cout << "Failed to load image: colormap type (" 
                  << static_cast<int>(header.color_map_type) << ") is unsupported." << std::endl;
        return false;
    }
    if(header.image_type != 2)
    {
        std::cout << "Failed to load image: image type (" 
                  << static_cast<int>(header.image_type) << ") is not an uncompressed truecolor "
                  << "image and is therefore not supported." << std::endl;
        return false;
    }

    // Print out the image properties
    std::cout << "Image properties: " << std::endl;
    std::cout << "origin: (" << header.x_origin  << ", " << header.y_origin << ") "
              << "dimensions: (" << header.width << ", " << header.height << ") "
              << static_cast<int>(header.depth)  << " bits per pixel, "
              << (header.descriptor & 56) << " bits alpha."
              << std::endl << std::endl;

    // Allocate a buffer to hold the raw image data, read it from the file then close the file
    int size = header.width * header.height * (header.depth/8);
    char *data = new char[size];
    in.read(data, size);
    in.close();

    // Push image data to the GPU, release heap allocated buffer
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, header.width, header.height, 0, 
            GL_BGRA, GL_UNSIGNED_BYTE, data);
    delete[] data;

    // Texture coordinates should repeat, not clamp
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Apply bilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    return true;
}

/* Load a color texture and a normal texture
 */
void Mesh::loadTextures(std::string color_path, std::string normal_path)
{
    loadTextureFile(color_path, textures[0]);
    loadTextureFile(normal_path, textures[1]);
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
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(Vector)));
    
    // Enable vertex attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

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
    locTexColor = glGetUniformLocation(program, "texColor");
    locTexNormal = glGetUniformLocation(program, "texNormal");

    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform1i(locTexColor, 0);

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

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

    verts[0].uv = Vector(1.0f, 1.0f, 0.0f);
    verts[1].uv = Vector(0.0f, 1.0f, 0.0f);
    verts[2].uv = Vector(0.0f, 0.0f, 0.0f);
    verts[3].uv = Vector(1.0f, 0.0f, 0.0f);
    verts[4].uv = Vector(0.0f, 1.0f, 0.0f);
    verts[5].uv = Vector(1.0f, 1.0f, 0.0f);
    verts[6].uv = Vector(1.0f, 0.0f, 0.0f);
    verts[7].uv = Vector(0.0f, 0.0f, 0.0f);

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
