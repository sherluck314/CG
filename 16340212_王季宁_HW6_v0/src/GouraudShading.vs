#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 lighting_color; 

uniform vec3 light_pos;
uniform vec3 view_pos;
uniform vec3 light_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float aStrength;
uniform float specularStrength;
uniform float diffusefactor;
uniform int specfactor;


void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    //使用法线矩阵将法向量转换为世界空间坐标
    vec3 Position = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // 用光的颜色乘以一个很小的常量环境因子，再乘以物体的颜色，以获得环境光照
    // float ambientStrength = 0.1;
    vec3 ambient = aStrength * light_color;
    
    // 把法线和方向向量都进行标准化
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos - Position);
    //计算光源对当前片段实际的漫反射影响。结果值再乘以光的颜色，得到漫反射分量
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color*diffusefactor;
    
     // 定义镜面强度变量
    //float specularStrength = 1.0; 
    // 计算视线方向向量
    vec3 viewDir = normalize(view_pos - Position);
    // 计算围绕法线轴的反射向量
    vec3 reflectDir = reflect(-lightDir, norm);  
    // 带入反光度进行计算
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specfactor);
    vec3 specular = specularStrength * spec * light_color;      
    // 计算光照颜色
    lighting_color = ambient + diffuse + specular;
}