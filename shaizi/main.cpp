//https://www.zhihu.com/people/guo-fen-25-53
#include<stdlib.h>
#include<stdio.h>
#include<memory>
#include<math.h>
#include<unistd.h>
#include<windows.h>
#define pi   3.14159265358979323846
#define c    3   //c���������ͶӰƽ��ľ���,��ΪͶӰƽ����z=0,����������λ����(0,0,c),����z�Ḻ���᷽��
#define screen_width   50
#define screen_height  25
float cube[6][4][3] =//��˼������ƽ��,������,һ��������,��������ά������ʾ��,�����Ҳ��������ܹ�����������,��ô�ܱ�ʾһ��������,Ŀǰ����,
//�������ÿ���涼ֻ��ʾ��һ��������,���µ���˼���������ʾһ��ƽ��,������˵�⼸�������������ĵ�
{
	{
		{-0.5, -0.5,  0.5}, { 0.5, -0.5,  0.5}, {-0.5,  0.5,  0.5}, { 0.0,  0.0,  1.0}
	},
	{
		{-0.5, -0.5,  0.5}, {-0.5, -0.5, -0.5}, {-0.5,  0.5,  0.5}, {-1.0,  0.0,  0.0}
	},
	{
		{-0.5, -0.5,  0.5}, {-0.5, -0.5, -0.5}, { 0.5, -0.5,  0.5}, { 0.0, -1.0,  0.0}
	},
	{
		{-0.5,  0.5,  0.5}, { 0.5,  0.5,  0.5}, {-0.5,  0.5, -0.5}, { 0.0,  1.0,  0.0}
	},

	{
		{ 0.5, -0.5,  0.5}, { 0.5, -0.5, -0.5}, { 0.5,  0.5,  0.5}, { 1.0,  0.0,  0.0}
	},
	{
		{-0.5, -0.5, -0.5}, { 0.5, -0.5, -0.5}, {-0.5,  0.5, -0.5}, { 0.0,  0.0, -1.0}
	 }
};

int face[6][3][3] = {//��������ӵ�İ�λ,ÿ�����൱��9������,Ϊ1��λ�ñ�ʾ��������ǵ�,�������������������ɨ���,
	//���ǶԵ�������3��ȡ���õ��������պ÷���[0][1][2]��������,���Ǿ�֪��������Ƿ��ڵ���
	{
		{0, 0, 0}, {0, 1, 0}, {0, 0, 0}
	},
	{
		{0, 0, 1}, {0, 0, 0}, {1, 0, 0}
	},
	{
		{0, 0, 1}, {0, 1, 0}, {1, 0, 0}
	},
	{
		{1, 0, 1}, {0, 0, 0}, {1, 0, 1}
	},
	{
		{1, 0, 1}, {0, 1, 0}, {1, 0, 1}
	},
	{
		{1, 0, 1}, {1, 0, 1}, {1, 0, 1}
	}
};

int judgeFace(int ID, float x, float y)
{
	return face[ID][(int)(3.f * y)][(int)(3.f * x)];
}

void ini()//�൱�ڰ�����������
{
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 4; ++j)//����ÿ����Ķ��嶥�㼰�䷨��,����任��һ����������״̬
		{
			float x = cube[i][j][0];
			float y = cube[i][j][1];
			float z = cube[i][j][2];

			cube[i][j][0] = (sqrt(3) / 6.f + 0.5) * x - sqrt(3) / 3.f * y + (-0.5 + sqrt(3) / 6.f) * z;
			cube[i][j][1] = (sqrt(3) / 3.f) * x + (sqrt(3) / 3.f) * y + (sqrt(3) / 3.f) * z;
			cube[i][j][2] = (-0.5 + sqrt(3) / 6.f) * x + (-sqrt(3) / 3.f) * y + (sqrt(3) / 6.f + 0.5) * z;
		}
	}
}

void renderFrame()
{
	ini();
	double time = 0;
	while (1)
	{
		time += 0.01;
		float z_buffer[screen_height + 1][screen_width + 1];
		for (int i = 0; i < screen_height; ++i)//ͬ���Ƕ�ά����,char����memset��ʼ����������,���Ǹ��������ǲ��е�,ֻ������ѭ��
		{
			for (int j = 0; j < screen_width; ++j)
			{
				z_buffer[i][j] = -100;
			}
		}
		char output[screen_height + 1][screen_width + 1];
		memset(output, ' ', sizeof(output));
		for (int i = 0; i < 6; ++i)
		{
			for (float u = 0.0f; u < 1.0f; u += 0.01)
			{
				for (float v = 0.0f; v < 1.0f; v += 0.01)
				{
					float m_x = (cube[i][1][0] - cube[i][0][0]);//���õ�ʱ����������ĳ�����ֱ��������������õ��б�����
					float m_y = (cube[i][1][1] - cube[i][0][1]);
					float m_z = (cube[i][1][2] - cube[i][0][2]);

					float n_x = (cube[i][2][0] - cube[i][0][0]);//���õ�ʱ����������ĳ�����ִ��������������õ��б�����
					float n_y = (cube[i][2][1] - cube[i][0][1]);
					float n_z = (cube[i][2][2] - cube[i][0][2]);

					float x = m_x * u + n_x * v + cube[i][0][0];//������ֱ�ǵ�Ϊ��ʼ,����ѭ��,���������������ɢ���xyzֵ�ó���
					float y = m_y * u + n_y * v + cube[i][0][1];
					float z = m_z * u + n_z * v + cube[i][0][2];

					float rotation_x = cos(time) * x - sin(time) * z;
					float rotation_y = y;
					float rotation_z = sin(time) * x + cos(time) * z;

					float normal_x = (cube[i][3][0]) * cos(time) - sin(time) * (cube[i][3][2]);
					float normal_y = cube[i][3][1];
					float normal_z = (cube[i][3][0]) * sin(time) + cos(time) * (cube[i][3][2]);

					int screen_x = (rotation_x / (1 - rotation_z / c) + 1.f) / 2 * screen_width;
					int screen_y = (rotation_y / (1 - rotation_z / c) + 1.f) / 2 * screen_height;
					float screen_z = rotation_z / (1 - rotation_z / c);

					float L = normal_z;
					if (L > 0)
					{
						if (z_buffer[screen_y][screen_x] < screen_z)
						{
							z_buffer[screen_y][screen_x] = screen_z;
							if (judgeFace(i, u, v) == 1)
							{
								float tempu = u - (float)((int)(u * 3.0f)) * 1.0f / 3.0f;//�����ʱ��������˼�ǵ�ǰ��uv������ȡ��֮�����ڵ��Ǹ�����
								//��������,Ҫ�����������ļ���뾶�õ�Բ�εĵ㡣
								float tempv = v - (float)((int)(v * 3.0f)) * 1.0f / 3.0f;
								if ((tempu - 1.0f / 6.0f) * (tempu - 1.0f / 6.0f) + (tempv - 1.0f / 6.0f) * (tempv - 1.0f / 6.0f) <= 1.0f / 36.0f)
								{
									L = 0;
								}
								else
								{
									L = (L + 0.1) * sqrt(2);//�������ûɶ����,���Ƿ����ӵ�Ļ����и����͵����ȡ�
								}
							}
							else
							{
								L = (L + 0.1) * sqrt(2);
							}
							int luminance_index = L * 8;
							if (luminance_index > 11) luminance_index = 11;
							output[screen_y][screen_x] = ".,-~<=#%&$@"[luminance_index];
						}
					}
					else
					{
						if (z_buffer[screen_y][screen_x] < screen_z) z_buffer[screen_y][screen_x] = screen_z;
					}
				}
			}
		}
		for (int i = 0; i < screen_height; ++i)
		{
			for (int j = 0; j < screen_width; ++j)
			{
				putchar(output[i][j]);
			}
			putchar('\n');
		}
		Sleep(15);
		printf("\x1b[26A");
	}
}

int main()
{
	renderFrame();
	return 0;
}
