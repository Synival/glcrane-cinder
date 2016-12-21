#version 150

// Uniforms
uniform sampler2D uTex0;
uniform float     uTime;

// Incoming varying.
#ifdef GL_ES
   in highp vec2 TexCoord0;
   in highp vec4 Color;
   in highp vec3 Normal;
#else
   in vec2 TexCoord0;
   in vec4 Color;
   in vec3 Normal;
#endif

// Hard-coded light from top-back.
const vec3 LightDir = vec3 (0.00, 0.707, 0.707);

void main(void)
{
   // Get color from texture.
   vec4 texColor = texture2D (uTex0, TexCoord0);

   // Get intensity from normal map.
   vec3 n = normalize (Normal);
   float i = max (dot (n, LightDir) * 0.50 + 0.50, 0.00);

   // Build our output color from input color, texture, and normal.
   vec4 FinalColor = Color * texColor;
   FinalColor.rgb *= i;

   // Crystal refraction effects!
   float t = uTime / 16.00;
   float r = (1.00 - i) * 2.00;
   FinalColor.rgb *= mix (
      vec3 (1.00, 1.00, 1.00),
      vec3 (
         sin ((t + 0.00 + i) * 3.141 * 4.00) * 0.50 + 0.50,
         sin ((t + 0.33 + i) * 3.141 * 4.00) * 0.50 + 0.50,
         sin ((t + 0.66 + i) * 3.141 * 4.00) * 0.50 + 0.50
      ),
      r);

   // Output our final color.
   gl_FragColor = FinalColor;
}
