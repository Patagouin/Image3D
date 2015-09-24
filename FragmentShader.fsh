#version 330

in float grey;
uniform float greyMax;

out vec4 fragColor;

void main(void)
{

   fragColor = vec4(grey/greyMax, grey/greyMax, grey/greyMax, 1.0-(grey/greyMax));
//     fragColor = vec4(1.0, 1.0, 1.0, 1.0-grey/greyMax); // opacite 1 = opaque

}
