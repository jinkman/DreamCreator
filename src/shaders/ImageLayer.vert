R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord;

    uniform bool bFlipY;
    uniform mat4 model;
    uniform mat4 projection;

    out vec2 texCoord;

    void main()
    {
        texCoord = aTexCoord;
        gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
        if(bFlipY) {
            gl_Position.y *= -1.0;
        }
    }
)"