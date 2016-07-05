uniform sampler2D flametex;

varying vec2 vTexCoord;
varying float vColor;

void main ()
{
  // this can be a uniform
  float particleShape = 0.18;

  float fade = pow (dot(vTexCoord, vTexCoord), particleShape);
  gl_FragColor = (1.0 - fade) * texture2D(flametex, vec2(vColor, 0.0));
}
