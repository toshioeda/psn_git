#pragma once


//------------------------------------------------------------------------------
//		グローバル関数宣言
//------------------------------------------------------------------------------
double	xfunc_angle360( double angle );																			//角度を360度以内に変換する
double	xfunc_angle_pm180( double angle );																		//角度を±180度に変換する
double	xfunc_kakudo_pp( dPOINT p1 , dPOINT p2 , double* p_deg , double* p_rad );								//2点間角度[deg]
double	xfunc_kakudo( double x1,double y1,double x2,double y2,double* p_deg,double* p_rad);						//角度[deg]
double	xfunc_unit_vect_pp( dPOINT p1 , dPOINT p2 , dPOINT* unit_vect );										//2点間単位ベクトル
double	xfunc_unit_vect( double x1 , double y1 , double x2 , double y2 , double*p_dx , double*p_dy );			//単位ベクトル

double	xfunc_dist_pp( dPOINT p1 , dPOINT p2 );																	//2点間距離
double	xfunc_dist( double x1 , double y1 , double x2 , double y2 );


void	xfunc_print_text( HDC hdc , char* wchar , double tx , double ty , long height , long f_color , char* font_name , long back_color , double angle , long bold );	//文字出力
double	xfunc_rnd();																							//乱数(0-1)
void	xfunc_randomize();																						//乱数パターン変更




