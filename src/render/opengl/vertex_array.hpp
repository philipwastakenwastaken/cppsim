#pragma once

#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"
#include <glad/gl.h>


namespace cppsim {

    class VertexArray {

    public:
        VertexArray() {
            glGenVertexArrays(1, &render_id);
        }

        ~VertexArray() = default;

        void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;
        void delete_buffer();

        void bind() const;
        void unbind() const;

        [[nodiscard]] unsigned get_render_id() const {return render_id;}
    private:
        unsigned render_id = 0;


    };

}