precision mediump float;
varying vec2 v_texCoord;
uniform sampler2D s_texture;
void main()
{
  vec2 m_texCoord = v_texCoord;
  m_texCoord.s = 1.0 - m_texCoord.s;
  gl_FragColor =  texture2D( s_texture, abs(m_texCoord) );
  //gl_FragColor =  vec4(vec3(abs(v_texCoord.y)),1.0);
}

