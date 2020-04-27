#include "shader.h"
#include "window.h"

GLuint shader::window_dim_uniform;
GLuint shader::lower_bounds_uniform;
GLuint shader::upper_bounds_uniform;
GLuint shader::max_iter_uniform;

void shader::init(void)
{
    if (glewInit() != GL_NO_ERROR)
        throw std::runtime_error("Could not initialize GLEW!");

    GLuint program_uid = glCreateProgram();
    GLuint vert_uid = shader::create_from_file("src/shaders/shader.vert", GL_VERTEX_SHADER);
    GLuint frag_uid = shader::create_from_file("src/shaders/shader.frag", GL_FRAGMENT_SHADER);

    glAttachShader(program_uid, vert_uid);
    glAttachShader(program_uid, frag_uid);

    glLinkProgram(program_uid);

    int success;
    glGetProgramiv(program_uid, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        std::string error_message = std::string("Program linking failed");
        throw std::runtime_error(error_message.c_str());
    }

    glUseProgram(program_uid);

    glDeleteShader(frag_uid);
    glDeleteShader(vert_uid);

    shader::window_dim_uniform = glGetUniformLocation(program_uid, "window_dim");
    shader::lower_bounds_uniform = glGetUniformLocation(program_uid, "lower_bounds");
    shader::upper_bounds_uniform = glGetUniformLocation(program_uid, "upper_bounds");
    shader::max_iter_uniform = glGetUniformLocation(program_uid, "max_iter");

    glUniform2f(shader::window_dim_uniform, window::width, window::height);
    glUniform2d(shader::lower_bounds_uniform, window::lower_bounds.x, window::lower_bounds.y);
    glUniform2d(shader::upper_bounds_uniform, window::upper_bounds.x, window::upper_bounds.y);
    glUniform1i(shader::max_iter_uniform, window::max_iter);
}

GLuint shader::create_from_file(const char* path, GLenum type)
{
    std::ifstream file = std::ifstream(path);
    std::ostringstream file_content;
    std::string line;

    if (!file.is_open()) {
        std::string error_message = std::string("File not found: ") + path + "\n";
        throw std::runtime_error(error_message.c_str());
    }

    while (std::getline(file, line)) {
        file_content << line << "\n";
    }

    GLuint shader_uid = glCreateShader(type);

    std::string string_content = file_content.str();
    GLint length = string_content.length();
    const GLchar* shader_source = string_content.c_str();
    glShaderSource(shader_uid, 1, &shader_source, &length);

    glCompileShader(shader_uid);

    int success;

    glGetShaderiv(shader_uid, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        char infoLog[1024];
        glGetShaderInfoLog(shader_uid, 1024, NULL, infoLog);
        std::string error_message = std::string("Shader compilation failed: ") + path + "\n" + infoLog + "\n";
        throw std::runtime_error(error_message.c_str());
    }

    return shader_uid;
}
