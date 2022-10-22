//https://www.zhihu.com/people/guo-fen-25-53
#include<stdlib.h>
#include<stdio.h>
#include<memory>
#include<math.h>
#include<unistd.h>
#include<windows.h>
#define pi   3.14159265358979323846
#define c    3   //c是摄像机到投影平面的距离,因为投影平面是z=0,因此摄像机的位置是(0,0,c),看向z轴负半轴方向。
#define screen_width   50
#define screen_height  25
float cube[6][4][3] =//意思是六张平面,三个点,一个法向量,都是用三维向量表示的,但是我不明白这总共六个三角形,怎么能表示一个正方体,目前来看,
//正方体的每个面都只表示了一个三角形,文章的意思是三个点表示一个平面,而不是说这几个点就是立方体的点
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

int face[6][3][3] = {//这个是骰子点的摆位,每个面相当于9个格子,为1的位置表示这个格子是点,面上我们有密密麻麻的扫描点,
	//我们对点的坐标乘3再取整得到的整数刚好符合[0][1][2]三种索引,我们就知道这个点是否在点上
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

void ini()//相当于把骰子立起来
{
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 4; ++j)//遍历每个面的定义顶点及其法线,整体变换到一个立起来的状态
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
		for (int i = 0; i < screen_height; ++i)//同样是二维数组,char数组memset初始化不受限制,但是浮点数组是不行的,只能两轮循环
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
					float m_x = (cube[i][1][0] - cube[i][0][0]);//利用当时构建立方体某个面的直角三点坐标相减得到行边向量
					float m_y = (cube[i][1][1] - cube[i][0][1]);
					float m_z = (cube[i][1][2] - cube[i][0][2]);

					float n_x = (cube[i][2][0] - cube[i][0][0]);//利用当时构建立方体某个面的执教三点坐标相减得到列边向量
					float n_y = (cube[i][2][1] - cube[i][0][1]);
					float n_z = (cube[i][2][2] - cube[i][0][2]);

					float x = m_x * u + n_x * v + cube[i][0][0];//这样在直角点为开始,两层循环,遍历把整个面的离散点的xyz值得出来
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
								float tempu = u - (float)((int)(u * 3.0f)) * 1.0f / 3.0f;//这个临时的坐标意思是当前的uv在向下取整之后所在的那个骰子
								//点格的坐标,要和这个点格中心计算半径得到圆形的点。
								float tempv = v - (float)((int)(v * 3.0f)) * 1.0f / 3.0f;
								if ((tempu - 1.0f / 6.0f) * (tempu - 1.0f / 6.0f) + (tempv - 1.0f / 6.0f) * (tempv - 1.0f / 6.0f) <= 1.0f / 36.0f)
								{
									L = 0;
								}
								else
								{
									L = (L + 0.1) * sqrt(2);//这个好像没啥意义,就是非骰子点的话就有个极低的亮度。
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
