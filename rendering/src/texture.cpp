#include "texture.h"

using namespace Rendering;

Texture::Texture(GLenum tg, std::string path) : target(tg)
{
    using namespace std;

    // Generate texture
    tex_obj = 0;
    glGenTextures(1, &tex_obj);

    // If the path is good, load texture file
    int length = getLength(path);
    if(length > 0)
    {
        // Allocate a buffer to hold the file contents
        char* buffer = new char[length];
        getBlob(path, buffer, length);

        // Push file contents to GPU
        loadTGA(buffer);
        delete[] buffer;
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &tex_obj);
}


void Texture::bind()
{
    glBindTexture(target, tex_obj);
}

void Texture::loadTGA(char *buffer)
{
    using namespace std;
    
    TGAHeader header;

    // Read the first three header fields
    blobRead(header.id_len, buffer);
    blobRead(header.color_map_type, buffer);
    blobRead(header.image_type, buffer);

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
    blobRead(header.x_origin, buffer);
    blobRead(header.y_origin, buffer);
    blobRead(header.width, buffer);
    blobRead(header.height, buffer);
    blobRead(header.depth, buffer);
    blobRead(header.descriptor, buffer);

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
         << header.width << "x" << header.height << " " << header.depth << " bits per pixel, " 
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

