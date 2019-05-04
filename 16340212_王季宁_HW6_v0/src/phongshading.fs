#version 330 core
out vec4 FragColor;
//normal为法向量的参数
in vec3 objectNormal;  
in vec3 Frag_pos;  
  
uniform vec3 light_pos; 
uniform vec3 view_pos; 
uniform vec3 light_Color;
uniform vec3 object_Color;
uniform float aStrength;
uniform float specularStrength;
uniform float diffusefactor;
uniform int specfactor;

void main() {
    // 用光的颜色乘以一个很小的常量环境因子，再乘以物体的颜色，以获得环境光照
    //float aStrength = 0.1;
    vec3 ambient = aStrength * light_Color;
    
    // 把法线和方向向量都进行标准化
    vec3 norm = normalize(objectNormal);
    vec3 lightDir = normalize(light_pos - Frag_pos);
  //计算光源对当前片段实际的漫反射影响。结果值再乘以光的颜色，得到漫反射分量
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_Color*diffusefactor;
    
    // 定义镜面强度变量
    //float specularStrength = 0.5;
  // 计算视线方向向量
    vec3 viewDir = normalize(view_pos - Frag_pos);
  // 计算围绕法线轴的反射向量
    vec3 reflectDir = reflect(-lightDir, norm);  
  // 带入反光度进行计算
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specfactor);
    vec3 specular = specularStrength * spec * light_Color;  
        
    vec3 result = (ambient + diffuse + specular) * object_Color;
    FragColor = vec4(result, 1.0);
} 