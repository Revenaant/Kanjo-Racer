in vec3 vertex;

uniform vec2 offset;

void main (void) {
    gl_Position = gl_ModelViewProjectionMatrix * offset * vertex;

    gl_TexCoord[0] = gl_MultiTexCoord0;
}
