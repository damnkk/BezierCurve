#include <easyx.h>
#include <iostream>
#include <graphics.h> // 引用图形库头文件
#include<glm\glm.hpp>
#include <vector>
#include <math.h>
#define delayTime 10 //0.01秒刷新一次
using namespace glm;
using namespace std;
unsigned long long befTime = 0;


vector<vec2> PointBezier;

void initSupLine(vec2 begin,vec2 end,vec2 control)
{
	//画辅助线
	setlinecolor(GREEN);
	line(begin.x, begin.y, end.x, end.y);
	setlinecolor(RED);
	line(begin.x, begin.y, control.x, control.y);
	line(control.x, control.y, end.x, end.y);
}
//二阶贝塞尔曲线
void doubleBezier(vec2 begin, vec2 end, vec2 control, float* timeSpeed)
{
	float BElength = length(begin - end);
	*timeSpeed = clamp(*timeSpeed,(float)0.0, BElength);
	float controlRate = *timeSpeed / BElength;
	vec2 leftControlPoint = mix(begin, control, 1.0 - controlRate);
	vec2 rightControlPoint = mix(control, end, 1.0 - controlRate);
	setlinecolor(YELLOW);
	line(leftControlPoint.x, leftControlPoint.y, rightControlPoint.x, rightControlPoint.y);
	vec2 BezierPoint = mix(leftControlPoint, rightControlPoint, 1 - controlRate);
	PointBezier.push_back(BezierPoint);

}

void drawLine(vec2  begin, vec2  end, vec2 control, float* time)
{
	initSupLine(begin, end, control);
	cout << *time << endl;
	doubleBezier(begin, end, control, time);
	for (vector<vec2>::iterator it = PointBezier.begin(); it != PointBezier.end(); it++)
	{
		vec2 pointBox = *it;
		putpixel(pointBox.x, pointBox.y, RGB(0, 0, 255));
	}
}


int main()
{
	
	initgraph(500, 400, EW_SHOWCONSOLE | EW_DBLCLKS);
	BeginBatchDraw();
	setbkcolor(WHITE);
	vec2 begin = { 90,200 };
	vec2 end = { 400,200 };
	vec2 control = { 90,65 };
	float lineControl = 0.00f;
	float* numControl = &lineControl;
	setlinecolor(BLACK);
	line(0, 0, 500, 400);
	ExMessage m;
	while (1)
	{
		while (peekmessage(&m, -1, true))
		{
			switch (m.message)
			{
			case WM_MOUSEMOVE:
				break;
			case WM_LBUTTONDOWN:
					control.x = m.x;
					control.y = m.y;
					*numControl = 0.0f;
					vector<vec2>().swap(PointBezier);
					break;
			default:
				break;
			}
		}
		*numControl += 1.5f;
		drawLine(begin, end, control, numControl);
		FlushBatchDraw();
		Sleep(delayTime);
		cleardevice();
	}
	EndBatchDraw();
	system("pause");
	closegraph();
	return 0;
}