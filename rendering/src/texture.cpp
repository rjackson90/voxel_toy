#include "texture.h"

using namespace Rendering;

Texture::Texture(GLenum tg, std::string path) : target(tg)
{
    using namespace std;

    // Generate texture
    tex_obj = 0;
    glGenTextures(1, &tex_obj);

    // If the path is good, load texture file
    int length = Core::getLength(path);
    if(length > 0)
    {
        // Allocate a buffer to hold the file contents
        char* buffer = new char[length];
        Core::getBlob(path, buffer, length);

        // Push file contents to GPU
        loadTGA(buffer);
        delete[] buffer;
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &tex_obj);
}

TexturePtr Texture::TextureFromConfig(
        const Core::ConfigParser &parser, const std::string &section)
{
    try
    {
        GLenum target;
        std::string t_str = parser.get("target", section);
        if(t_str.compare("GL_TEXTURE_1D") == 0) target = GL_TEXTURE_1D;
        if(t_str.compare("GL_TEXTURE_2D") == 0) target = GL_TEXTURE_2D;
        if(t_str.compare("GL_TEXTURE_3D") == 0) target = GL_TEXTURE_3D;
        if(t_str.compare("GL_TEXTURE_1D_ARRAY") == 0) target = GL_TEXTURE_1D_ARRAY;
        if(t_str.compare("GL_TEXTURE_2D_ARRAY") == 0) target = GL_TEXTURE_2D_ARRAY;
        if(t_str.compare("GL_TEXTURE_RECTANGLE") == 0) target = GL_TEXTURE_RECTANGLE;
        if(t_str.compare("GL_TEXTURE_CUBE_MAP") == 0) target = GL_TEXTURE_CUBE_MAP;

        auto texPtr = std::make_shared<Texture>(target, Paths::rendering + 
                parser.get("path", section));
        return texPtr;
    }
    catch(const std::exception &ex)
    {
        std::cerr << "Error creating texture: " << ex.what() << std::endl;
    }

    auto error_tex = std::make_shared<Texture>(GL_TEXTURE_2D, Paths::rendering + "error.tga");
    return error_tex;
}

void Texture::bind() const
{
    glBindTexture(target, tex_obj);
}

void Texture::loadTGA(char *buffer)
{
    using namespace std;
    
    TGAHeader header;

    // Read the first three header fields
    Core::blobRead(header.id_len, buffer);
    Core::blobRead(header.color_map_type, buffer);
    Core::blobRead(header.image_type, buffer);

    // Skip reading the colormap spec if there is no colormap
    if( header.color_map_type == 0)
    {
        buffer += (2 * sizeof(short)) + sizeof(char);
    }
    else
    {
        cout << "Abandoning TGA load: images with colormaps are unsupported." << endl;
        return;
    }

    // Check image_type for compatibility
    if( header.image_type != 2 )
    {
        cout << "Abandoning TGA load: only uncompressed truecolor images are supported" << endl;
        return;
    }

    // Read the image spec
    Core::blobRead(header.x_origin, buffer);
    Core::blobRead(header.y_origin, buffer);
    Core::blobRead(header.width, buffer);
    Core::blobRead(header.height, buffer);
    Core::blobRead(header.depth, buffer);
    Core::blobRead(header.descriptor, buffer);

    // Check for compatibility, print image properties
    if(header.width != header.height)
    {
        cout << "Abandoning TGA load: non-square textures are unsupported." << endl;
        return;
    }
    if(!((header.width != 0) && !(header.width & (header.width - 1))))
    {
        cout << "Abandoning TGA load: non-power-of-two textures are unsupported." << endl;
        return;
    }
    cout << "Image Properties: Origin (" << header.x_origin << " ," << header.y_origin << "), "
         << header.width << "x" << header.height << " " << (int)header.depth << " bits per pixel, " 
         << (header.descriptor & 56) << " bits alpha." << endl;

    // Skip over the image ID if present, then push data to GPU
    if(header.id_len > 0)
    {
        buffer += header.id_len;
    }
    glBindTexture(target, tex_obj);
    glTexImage2D(target, 0, GL_RGBA, header.width, header.height, 0, 
            GL_BGRA, GL_UNSIGNED_BYTE, buffer);
    return;
}

