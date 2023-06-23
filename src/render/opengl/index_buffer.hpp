#pragma once

namespace cppsim {

class IndexBuffer
{

  public:
    IndexBuffer(const unsigned short* data, unsigned count);
    void delete_buffer();
    IndexBuffer() = default;

    void bind() const;
    void unbind() const;

    [[nodiscard]] unsigned get_count() const { return count; }
    [[nodiscard]] unsigned get_render_id() const { return render_id; }

  private:
    unsigned render_id;
    unsigned count;
};
} // namespace cppsim