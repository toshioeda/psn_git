#pragma once
//------------------------------------------------------------------------------
//人を生成したり動かしたりするメインのクラス宣言
//------------------------------------------------------------------------------


class x_simuB
{

public:
	x_simuB();
	~x_simuB();

//変数
public:
	//Windowsのシステム変数
	HWND		m_hwnd;					//表示のウインドウハンドル（windowの識別変数）
	//設定
	double		m_field_width;			//フィールドの幅[m]
	double		m_field_length;			//フィールドの長さ[m](人はこのフィールドの左右に幅いっぱいに集まります)
	double		m_disp_ratio;			//表示時の1.0[m]のドット数
	long		m_disp_offset_y;		//Ｙ座標中心位置
	long		m_man_max;				//人の全体人数
	S_MAN*		m_man_ptr;				//各人の属性（後で人数分確保する）
	//シミュレーション制御変数
	long		m_counta;				//逐次制御回数 カウント
	double		mf_cal_interval;		//計算ピッチ時間[sec]	１カウントで進む時間
	double		m_genzai_jikoku_sec;	//計算上の時刻[sec]
	//グラフィックリソース
	HPEN		m_color_pen[8];			//ペン
	HBRUSH		m_color_brush[8];		//ブラシ
	//表示モード
	long		m_disp_vector_sw;		//方向ベクトルの表示非表示モード
	//実時間計測(Window始動後のミリ秒カウンタ)[milli sec]
	unsigned int	m_simu_tyuudann_start;		//中断開始時刻
	unsigned int	m_simu_tyuudann_time;		//中断していた時間
	unsigned int	m_simu_start_time;			//計算開始時刻


//関数
public:
void	x_simuB::disp_simu();																	//描画
long	x_simuB::make_fuzzy_new_vector( long index );											//ファジィーで回避ベクトルを計算
double	x_simuB::fuzzy_suiron(long my_index , long aite_index );								//ファジィ推論計算
double	x_simuB::xfunc_daikei_area(double height, double bottom_length , double top_height );	//台形の面積
double	x_simuB::xfunc_fuzzy_strong( double val , double start_pos , double end_pos );			//ファジィ強度の計算
void	x_simuB::real_to_view( double rx1 , double ry1 , double* vx1 , double* vy1 );			//実座標をビュー座標に変換
void	x_simuB::view_to_real( double vx1 , double vy1 , double* rx1 , double* ry1 );			//ビューを実に変換
long	x_simuB::disp_man_one( HDC hdc , long index );											//一人描画
long	x_simuB::next_step_all();																//全員の次のステップを計算
long	x_simuB::next_step_one( long index );													//一人の次のステップを計算
long	x_simuB::check_next_position( long index );												//次の位置に移動可能かどうかを探る


};