uniform float time;

varying vec2 vTexCoord;
varying float vColor;

void main ()
{
  // these can be uniforms
  float particleSpeed = 32.0;
  float particleSystemShape = 3.64;
  float particleSpread = 10.0;
  float particleSystemHeight = 177.0;
  float particleSize = 7.8;

  float t = fract(gl_Vertex.z + particleSpeed * time);
  float s = pow (t, particleSystemShape);
  vec3 pos;

  pos.x = particleSpread * s * cos (62.0 * gl_Vertex.z);
  pos.z = particleSpread * s * cos (163.0 * gl_Vertex.z);
  pos.y = particleSystemHeight * t;
  
  pos += particleSize * (gl_Vertex.x * gl_ModelViewMatrixInverse[0] + gl_Vertex.y * gl_ModelViewMatrixInverse[1]).xyz;

  gl_Position = gl_ModelViewProjectionMatrix * vec4 (pos, 1.0);

  vTexCoord = gl_Vertex.xy;
  vColor = 1.0 - t;
}
