#version 330

in vec4 oColor;
in float diffTerm;
in float nDotV;
out vec4 outputColor;

void main()
{
    if (nDotV < 0.2) {
        outputColor = vec4(0);
    } else if (diffTerm < 0.2) {
        outputColor = oColor * 0.2;
    } else {
        outputColor = oColor;
    }
}
