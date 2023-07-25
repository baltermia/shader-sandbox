#version 460 core

uniform float seconds;
uniform uvec2 window_size;

out vec4 FragColor;

void main()
{
    float smallerSize = min(window_size.x, window_size.y);
    
    vec2 uv = (gl_FragCoord.xy * 2 - window_size.xy) / smallerSize;
  
    float dist = sqrt(pow(uv.x, 2) + pow(uv.y, 2));
    
    float col = sin(dist * seconds * 4);
    
    col = round(col * 1.5);

    FragColor = vec4(vec3(col), 1);
}
