#version 430 core

in vec2 fTexCoords;

layout(location = 0) out float fragColor;

uniform sampler2D ssaoInput;
uniform int noiseSize;

void main() 
{
   int nSize = noiseSize / 2;
   vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
   float result = 0.0;
   for (int x = -nSize; x < nSize; ++x) 
   {
      for (int y = -nSize; y < nSize; ++y) 
      {
         vec2 offset = (vec2(float(x), float(y))) * texelSize;
         result += texture(ssaoInput, fTexCoords + offset).r;
      }
   }
 
   fragColor = result / float(noiseSize * noiseSize);
}