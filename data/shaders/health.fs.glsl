#version 330
// From vertex shader
in float pos_x;
in float pos_y;

// Application data
uniform vec2 scale;
uniform float health;
uniform int status;

// Output color
layout(location = 0) out  vec4 color;

bool between(float x, float start, float end){
    return start <= x && x <= end;
}

bool segment_pos(float pos, int n, float step, float segment_thickness) {
    for (int i = 0; i < n; i++){
        if (between(pos, i * step - segment_thickness, i * step + segment_thickness)){
            return true;
        }
    }
    return false;
}

bool stroke_area(float pos, float stroke_thickness) {
//    return false;
    return between(pos, 0, stroke_thickness) || between(pos, 1.0-stroke_thickness, 1.0);
}

void main()
{
    int n = 3; // Number of segments
    float i = 1.0f / n;
    float segment_thickness = 0.01;
    float stroke_thickness = 0.1;
    vec3 stroke_color = vec3(0.0);

    vec3 start_color, end_color;
    start_color = vec3(231.f, 76.f, 60.f) / 256.0;
    end_color = vec3(46.f, 204.f, 113.f) / 256.0;

    float aspect = scale.x / scale.y;
    float strokeX = stroke_thickness / aspect;
    float strokeY = stroke_thickness;
    if (stroke_area(pos_x, strokeX) || stroke_area(pos_y, strokeY)) { // Stroke
        color = vec4(stroke_color, 1.0);
    } else if (pos_x >= segment_thickness && pos_x <= (1.0 - segment_thickness)
        && segment_pos(pos_x, n, i, segment_thickness)) { // thickness of segments
        color = vec4(stroke_color, 0.5); // Blank
    } else { // Color in the health value
        if (status == 1) { // invincible
            start_color =  vec3(52.0, 152.0, 219.0) / 256.0f;
            end_color = vec3(41, 128, 185) / 256.0f;
        }
        if (pos_x < health){
            vec3 fcolor = mix(start_color, end_color, pos_x * health);
            color = vec4(fcolor, 1.0);
        } else {
            color = vec4(0.0, 0.0, 0.0, 1.0);
        }
	}
}
