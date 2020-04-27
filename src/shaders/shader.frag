#version 460 core

out vec4 FragColor;

in vec4 vertexColor;

uniform vec2 window_dim;
uniform dvec2 lower_bounds;
uniform dvec2 upper_bounds;
uniform int max_iter;

dvec2 z = dvec2(0.0, 0.0);
vec4 palette[16] = {
	vec4(0.26, 0.12, 0.06, 1.0),
	vec4(0.10, 0.03, 0.10, 1.0),
	vec4(0.04, 0.00, 0.18, 1.0),
	vec4(0.02, 0.02, 0.29, 1.0),
	vec4(0.00, 0.03, 0.39, 1.0),
	vec4(0.05, 0.17, 0.54, 1.0),
	vec4(0.09, 0.32, 0.69, 1.0),
	vec4(0.22, 0.49, 0.82, 1.0),
	vec4(0.53, 0.71, 0.90, 1.0),
	vec4(0.83, 0.93, 0.97, 1.0),
	vec4(0.95, 0.91, 0.75, 1.0),
	vec4(0.97, 0.79, 0.37, 1.0),
	vec4(1.00, 0.67, 0.00, 1.0),
	vec4(0.80, 0.50, 0.00, 1.0),
	vec4(0.60, 0.34, 0.00, 1.0),
	vec4(0.42, 0.20, 0.01, 1.0)
};

void main() {
	dvec2 normalized_coords = gl_FragCoord.xy / dvec2(window_dim);
	dvec2 c = normalized_coords * (upper_bounds - lower_bounds) + lower_bounds;

	vec4 inside_color = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color = inside_color;
	int iter = 0;
	float squared_modulus;

	for (; iter < max_iter; iter++) {
		z = dvec2(
			z.x * z.x - z.y * z.y + c.x,
			2.0 * z.x * z.y + c.y
		);

		squared_modulus = float(z.x * z.x + z.y * z.y);
		if (squared_modulus >= (1 << 16)) {
			break;
		}
	}

	if (iter < max_iter) {
		float log_zn = log(squared_modulus) / 2.0;
		float nu = log(log_zn / log(2)) / log(2);
		float color_value = iter + 1.0 - nu;

		int color_index = int(floor(color_value));
		vec4 color_1 = palette[color_index % palette.length()];
		vec4 color_2 = palette[(color_index + 1) % palette.length()];
		color = mix(color_1, color_2, fract(color_value));
	}

	FragColor = color;
}