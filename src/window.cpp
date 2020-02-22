#include "window.h"
#include "shader.h"
#include <math.h>

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

const double zoom_factor = 98.0 / 100.0;
const double step = 1.0 - zoom_factor;
const double iter_factor = zoom_factor;
const window::bounds default_lower_bounds = { -2.5, -1. };
const window::bounds default_upper_bounds = { 1., 1. };

GLFWwindow* window::ref;
int window::width;
int window::height;
int window::max_iter = 100;
window::bounds window::lower_bounds = default_lower_bounds;
window::bounds window::upper_bounds = default_upper_bounds;

void resize_callback(GLFWwindow* window, int width, int height) {
    window::width = width;
    window::height = height;
    
    glUniform2f(shader::window_dim_uniform, window::width, window::height);
    glViewport(0, 0, window::width, window::height);
}

double get_center(double lower, double upper) {
    return (upper - lower) / 2.0 + lower;
}

void zoom(double zoom_factor) {
    double scaled_lower_x = window::lower_bounds.x * zoom_factor;
    double scaled_lower_y = window::lower_bounds.y * zoom_factor;
    double scaled_upper_x = window::upper_bounds.x * zoom_factor;
    double scaled_upper_y = window::upper_bounds.y * zoom_factor;
    
    double current_center_x = get_center(window::upper_bounds.x, window::lower_bounds.x);
    double current_center_y = get_center(window::upper_bounds.y, window::lower_bounds.y);
    double new_center_x = get_center(scaled_upper_x, scaled_lower_x);
    double new_center_y = get_center(scaled_upper_y, scaled_lower_y);

    double translation_amount_x = new_center_x - current_center_x;
    double translation_amount_y = new_center_y - current_center_y;

    window::lower_bounds.x = scaled_lower_x - translation_amount_x;
    window::upper_bounds.x = scaled_upper_x - translation_amount_x;
    window::lower_bounds.y = scaled_lower_y - translation_amount_y;
    window::upper_bounds.y = scaled_upper_y - translation_amount_y;
}

void move(direction dir) {
    double amount = dir == DOWN || dir == LEFT ? -step : step;
    
    if (dir == UP || dir == DOWN) {
        amount *= (window::upper_bounds.y - window::lower_bounds.y);
        window::upper_bounds.y += amount;
        window::lower_bounds.y += amount;
    } else {
        amount *= (window::upper_bounds.x - window::lower_bounds.x);
        window::upper_bounds.x += amount;
        window::lower_bounds.x += amount;
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    switch (key) {
        case GLFW_KEY_UP:
            zoom(zoom_factor);
            break;
        case GLFW_KEY_DOWN:
            zoom(1.0 / zoom_factor);
            break;
        case GLFW_KEY_W:
            move(UP);
            break;
        case GLFW_KEY_S:
            move(DOWN);
            break;
        case GLFW_KEY_A:
            move(LEFT);
            break;
        case GLFW_KEY_D:
            move(RIGHT);
            break;
        case GLFW_KEY_C:
            window::lower_bounds = default_lower_bounds;
            window::upper_bounds = default_upper_bounds;
            break;
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_LEFT:
            window::max_iter *= iter_factor;
            window::max_iter = fmax(window::max_iter, 1.0);
            break;
        case GLFW_KEY_RIGHT:
            window::max_iter = ceil(window::max_iter / iter_factor);
            break;
        default:
            break;
    }

    glUniform2d(shader::lower_bounds_uniform, window::lower_bounds.x, window::lower_bounds.y);
    glUniform2d(shader::upper_bounds_uniform, window::upper_bounds.x, window::upper_bounds.y);
    glUniform1i(shader::max_iter_uniform, window::max_iter);
}

void window::init(int width, int height, const char* title) {
    if (!glfwInit())
        throw std::exception("Could not initialize GLWF!");

    window::width = width;
    window::height = height;

    window::ref = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window::ref) {
        glfwTerminate();
        throw std::exception("Could not create a GLWF window!");
    }
    
    // Sizing
    glfwSetWindowSizeCallback(window::ref, resize_callback);
    glfwSetWindowAspectRatio(window::ref, 7, 4);

    // Events
    glfwSetKeyCallback(window::ref, key_callback);

    glfwMakeContextCurrent(window::ref);
}

void window::display(void) {
    while (!glfwWindowShouldClose(window::ref)) {

        glClear(GL_COLOR_BUFFER_BIT);

        buffers::render();

        glfwSwapBuffers(window::ref);
        glfwPollEvents();
    }

    glfwTerminate();
}