#pragma once

#include <stdio.h>			//�W�����o��
#include <math.h>			//fabs()�Ȃǐ��w�n�֐���

#include	"x_head.h"			//�l�̕W���錾


//------------------------------------------------------------------------------
//		�O���[�o���֐��錾
//------------------------------------------------------------------------------
double	xfunc_angle360( double angle );																			//�p�x��360�x�ȓ��ɕϊ�����
double	xfunc_angle_pm180( double angle );																		//�p�x���}180�x�ɕϊ�����
double	xfunc_kakudo_pp( dPOINT p1 , dPOINT p2 , double* p_deg , double* p_rad );								//2�_�Ԋp�x[deg]
double	xfunc_kakudo( double x1,double y1,double x2,double y2,double* p_deg,double* p_rad);						//�p�x[deg]
double	xfunc_unit_vect_pp( dPOINT p1 , dPOINT p2 , dPOINT* unit_vect );										//2�_�ԒP�ʃx�N�g��
double	xfunc_unit_vect( double x1 , double y1 , double x2 , double y2 , double*p_dx , double*p_dy );			//�P�ʃx�N�g��

double	xfunc_dist_pp( dPOINT p1 , dPOINT p2 );																	//2�_�ԋ���
double	xfunc_dist( double x1 , double y1 , double x2 , double y2 );


void	xfunc_print_text( HDC hdc , char* wchar , double tx , double ty , long height , long f_color , char* font_name , long back_color , double angle , long bold );	//�����o��
double	xfunc_rnd();																							//����(0-1)
void	xfunc_randomize();																						//�����p�^�[���ύX




