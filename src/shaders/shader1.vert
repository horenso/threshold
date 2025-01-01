#version 330 core

layout(location = 0) in vec3 aPosition;  // Vertex position
layout(location = 1) in vec3 aNormal;    // Vertex normal

out vec3 fragColor;  // Output color to fragment shader

uniform mat4 uModel;
uniform mat4 uTransform;
uniform mat4 uProjection;

void main() {
    // Calculate the model-view matrix and normal matrix
    mat4 mvp = uProjection * uTransform * uModel;
    gl_Position = mvp * vec4(aPosition, 1.0);

    // Assign a color based on the normal direction
    if(aNormal.z > 0.9) {
        fragColor = vec3(1.0, 0.0, 0.0);  // Red for faces facing the camera (positive Z)
    } else if(aNormal.z < -0.9) {
        fragColor = vec3(0.0, 1.0, 0.0);  // Green for faces facing away from the camera (negative Z)
    } else if(aNormal.x > 0.9) {
        fragColor = vec3(0.0, 0.0, 1.0);  // Blue for faces facing along the X axis
    } else if(aNormal.x < -0.9) {
        fragColor = vec3(1.0, 1.0, 0.0);  // Yellow for faces facing the opposite X axis
    } else if(aNormal.y > 0.9) {
        fragColor = vec3(1.0, 0.0, 1.0);  // Magenta for faces facing along the Y axis
    } else if(aNormal.y < -0.9) {
        fragColor = vec3(0.0, 1.0, 1.0);  // Cyan for faces facing the opposite Y axis
    } else {
        fragColor = vec3(0.5, 0.5, 0.5);  // Grey for other cases
    }
}
