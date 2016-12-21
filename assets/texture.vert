#version 150

// Uniforms.
uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

// Attributes.
#ifdef GL_ES
   in  highp vec4 ciPosition;
   in  highp vec2 ciTexCoord0;
   in  highp vec4 ciColor;
   in  highp vec3 ciNormal;
   out highp vec2 TexCoord0;
   out highp vec4 Color;
   out highp vec3 Normal;
#else
   in  vec4 ciPosition;
   in  vec2 ciTexCoord0;
   in  vec4 ciColor;
   in  vec3 ciNormal;
   out vec2 TexCoord0;
   out vec4 Color;
   out vec3 Normal;
#endif

/* standard attributes and uniforms. */

void main(void)
{
   gl_Position = ciModelViewProjection * ciPosition;
   Normal      = normalize (ciNormalMatrix * ciNormal);
   TexCoord0   = ciTexCoord0;
   Color       = ciColor;
}
