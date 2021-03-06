#version 120

uniform sampler2D texture;

uniform vec4 u_Color;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color
    gl_FragColor = pixel * u_Color * gl_Color;
}
