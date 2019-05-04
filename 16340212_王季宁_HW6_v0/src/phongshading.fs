#version 330 core
out vec4 FragColor;
//normalΪ�������Ĳ���
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
    // �ù����ɫ����һ����С�ĳ����������ӣ��ٳ����������ɫ���Ի�û�������
    //float aStrength = 0.1;
    vec3 ambient = aStrength * light_Color;
    
    // �ѷ��ߺͷ������������б�׼��
    vec3 norm = normalize(objectNormal);
    vec3 lightDir = normalize(light_pos - Frag_pos);
  //�����Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ�졣���ֵ�ٳ��Թ����ɫ���õ����������
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_Color*diffusefactor;
    
    // ���徵��ǿ�ȱ���
    //float specularStrength = 0.5;
  // �������߷�������
    vec3 viewDir = normalize(view_pos - Frag_pos);
  // ����Χ�Ʒ�����ķ�������
    vec3 reflectDir = reflect(-lightDir, norm);  
  // ���뷴��Ƚ��м���
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specfactor);
    vec3 specular = specularStrength * spec * light_Color;  
        
    vec3 result = (ambient + diffuse + specular) * object_Color;
    FragColor = vec4(result, 1.0);
} 