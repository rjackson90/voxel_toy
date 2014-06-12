#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>
#include <iostream>

#include "fileops.h"
#include "rendering.h"
#include "config.h"
#include "data_paths.h"

namespace Rendering
{
    class Texture
    {
        public:
        Texture(GLenum, std::string);
        ~Texture();

        static TexturePtr TextureFromConfig(const Core::ConfigParser &, const std::string &);

        void bind() const;

        private:
        GLenum target;
        GLuint tex_obj;

        void loadTGA(char*);
    };

    struct TGAHeader
    {
        char id_len = -1;
        char color_map_type = -1;
        char image_type = -1;

        // Image spec
        short x_origin = -1;
        short y_origin = -1;
        short width = -1;
        short height = -1;
        char depth = -1;
        char descriptor = -1;
    };
}
#endif // TEXTURE_H
