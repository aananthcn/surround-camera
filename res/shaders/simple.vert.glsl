attribute vec4 a_position;
attribute vec2  a_texCoord;
uniform mat4 mvpMatrix;
varying vec2 v_texCoord;
void main()
{
   gl_Position = mvpMatrix * a_position;
   v_texCoord = a_texCoord;
}
