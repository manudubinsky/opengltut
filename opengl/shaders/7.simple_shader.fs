#version 330 core

flat in float ourColor;
out vec4 FragColor;
void main()
{
   FragColor = vec4(ourColor, ourColor, ourColor, 1.0f);
}