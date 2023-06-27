#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"

#include <glad/gl.h>

namespace cppsim {

class DrawCall
{

  public:
    enum class Primitive : GLenum {
        Invalid = GL_NONE,
        Points = GL_POINTS,
        Lines = GL_LINES,
        LineStrip = GL_LINE_STRIP,
        LineLoop = GL_LINE_LOOP,
        LinesAdjacency = GL_LINES_ADJACENCY,
        LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
        Triangles = GL_TRIANGLES,
        TriangleStrip = GL_TRIANGLE_STRIP,
        TriangleFan = GL_TRIANGLE_FAN,
        TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
        TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
        Patches = GL_PATCHES
    };

    enum class PolygonMode : GLenum {
        Fill = GL_FILL,
        Lines = GL_LINE

    };

    DrawCall() = default;
    DrawCall(i32 f, i32 c, Primitive p) : first(f), count(c), primitive(p) {}
    DrawCall(i32 f, i32 c) : first(f), count(c), primitive(Primitive::Triangles), polygon_mode(PolygonMode::Fill) {}
    DrawCall(i32 f, i32 c, PolygonMode p) : first(f), count(c), primitive(Primitive::Triangles), polygon_mode(p) {}


    void draw() const;


  private:
    i32 first;
    i32 count;
    Primitive primitive;
    PolygonMode polygon_mode;
};


} // namespace cppsim
