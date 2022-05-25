#pragma once
/*!������� ��������� ����� �� u������ ����� �� ������� (�1,y1)(x2,y2)
*/
bool point_on_segment(int x1, int y1, int x2, int y2, int x, int y);
/*!������� ��������� ������������ �� ������� (x1,y1)(x2,y2) u (x3,y3)(x4,y4)
*/
bool intersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
/*!������� ��������� ����������� ���� � �������������� k,b � ������� (�1,y1)(x2,y2)
*/
int ray_intersection_segment(double x1, double y1, double x2, double y2, double k, double b, double pointY, double pointX);

//��������� �������� ���������� x,y �����
struct point
{
    unsigned int x[100];
    unsigned int y[100];
};
