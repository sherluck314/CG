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
    
    //ʹ�÷��߾��󽫷�����ת��Ϊ����ռ�����
    vec3 Position = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // �ù����ɫ����һ����С�ĳ����������ӣ��ٳ����������ɫ���Ի�û�������
    // float ambientStrength = 0.1;
    vec3 ambient = aStrength * light_color;
    
    // �ѷ��ߺͷ������������б�׼��
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos - Position);
    //�����Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ�졣���ֵ�ٳ��Թ����ɫ���õ����������
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color*diffusefactor;
    
     // ���徵��ǿ�ȱ���
    //float specularStrength = 1.0; 
    // �������߷�������
    vec3 viewDir = normalize(view_pos - Position);
    // ����Χ�Ʒ�����ķ�������
    vec3 reflectDir = reflect(-lightDir, norm);  
    // ���뷴��Ƚ��м���
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specfactor);
    vec3 specular = specularStrength * spec * light_color;      
    // ���������ɫ
    lighting_color = ambient + diffuse + specular;
}