#version 460 core

uniform float seconds;
uniform uvec2 window_size;

out vec4 FragColor;

void main()
{
     // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy / window_size.xy;

    // Time varying pixel color
    vec3 col = 0.5 + 0.5 * cos(seconds + uv.xyx + vec3(0,2,4));

    // Output to screen
    FragColor = vec4(col,1.0);
}
