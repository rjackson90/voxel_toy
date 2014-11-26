#ifndef VOXEL_SYSTEM_H
#define VOXEL_SYSTEM_H

#include "system.h"
#include "math_ext.h"

class VoxelSystem : public System
{
    public:
        virtual void tick(const SubsystemsPtr &, const double) override;

    private:
        class Chunk : Node
        {            
            public:
            Chunk(const Vector &, float, uint8, uint8, uint8);

            // Create a new Geometry object and update the corresponding RenderNode
            void fillGeometry(const SubsystemsPtr &, int);
            
            private:
            // Information to position this chunk in floating-point world coordinates
            Vector origin;
            float block_size;

            // Chunk dimensions in blocks
            uint8 width;
            uint8 height;
            uint8 depth;
            
            // If this flag is true, then tick() will regenerate the associated VBO
            bool dirty;

            // Storage for this Chunk's blocks.
            // Each block occupies only one byte. The LSB is a visibility flag,
            // the remaining 7 are used to select a texture
            uint8 *blocks;
        };

        std::vector<Chunk> chunks;
};

#endif // VOXEL_SYSTEM_H
