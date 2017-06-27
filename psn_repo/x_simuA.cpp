

//------------------------------------------------------------------------------
//人を生成したり動かしたりするメイン関数です。
//------------------------------------------------------------------------------
//★ 人格を変えて観察するのに都合の良い変数です。
//● 人格設定項目です。
//
#include <Windows.h>
#include <stdio.h>
#include <math.h>
//
#include "x_head.h"
#include "x_functions.h"
#include "x_simuA.h"
//
#pragma warning(disable : 4996)			//古いタイプの関数を使うと出て来る注意を止めさせる宣言文。


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//		コンストラクタ（生成時の処理）
//------------------------------------------------------------------------------
x_simuA::x_simuA()
{

	//設定するもの
	m_field_width=			1;				//☆	//初期配置幅[m]
	m_field_length=			20;				//☆	//フィールドの長さ[m]	(人はこのフィールドの左右に幅いっぱいに集まります)
	m_man_max=				60;				//☆	//人の全体人数[psn]
	//
	
	//シミュレーション制御変数
	mf_cal_interval=		1.0/30;			//☆	//計算ピッチ時間[sec]	１カウントで進む時間
	m_counta=				0;				//逐次制御回数
	m_genzai_jikoku_sec=	0.0;			//時刻[sec]

	//描画関連
	m_hwnd=					NULL;			//表示するウインドウハンドル（windowの識別変数）
	m_disp_vector_sw=		0;				//方向ベクトルの表示非表示

	//最初のウィンドウサイズ
	HWND	desktop_hwnd=	GetDesktopWindow();			//デスクトップのウインドウハンドル
	RECT	rect;
	GetClientRect( desktop_hwnd , &rect );				//ウィンドウの描画範囲のサイズ取得
	m_disp_ratio=			( rect.right - rect.left ) * 0.7 / m_field_length ;
	m_disp_offset_y=		( rect.bottom - rect.top ) / 2 ;

	//各人の属性をセット
	double	pos_y;
	m_man_ptr=				(S_MAN*)calloc( m_man_max , sizeof( S_MAN ) );		//各人の属性（人数分確保する）
	//全員のループ
	for(long nn=0;nn<m_man_max;nn++){
		S_MAN*	pman=	&m_man_ptr[nn];					//n番目の人クラスの人のポインタ
		//ファジィー　強度
		pman->body_haba=			0.2;			//●人の半径[m]
		pman->m_walk_speed=			1.0;			//●歩行速度[m/s]
		pman->m_zenpou_angle=		80.0;			//●前方にいると判断できる視野角[deg]
		//ファジィー　メンバーシップ
		//距離近い
		pman->near_start_pos=		1.0;//[m]		//●近いと感じ始める距離[m]（中心間距離から、体半径を除いた、空隙距離）
		pman->near_end_pos=			0.1;//[m]		//●絶対近いと感じる距離[m]
		//遠い
		pman->far_start_pos=		0.5;//[m]		//●遠いと感じ始める距離[m]（中心間距離から、体半径を除いた、空隙距離）
		pman->far_end_pos=			2.0;//[m]		//●絶対遠いと感じる距離[m]
//		pman->far_end_pos=			5.0;//[m]		//●絶対遠いと感じる距離[m]

		//方向向かってくる
		pman->kuru_start_pos=		60.0;//[deg]	//●「向かってくる」と感じはじめる相手の顔の角度[deg]
		pman->kuru_end_pos=			15.0;//[deg]	//●絶対「向かってくる」と感じる相手の顔の角度[deg]
		//pman->kuru_start_pos=		45.0;//[deg]	//●「向かってくる」と感じはじめる角度[deg]
		//pman->kuru_end_pos=			15.0;//[deg]	//●絶対「向かってくる」と感じる角度[deg]
		//向かってこない
		pman->konai_start_pos=		45.0;//[deg]	//●「遠ざかる」と感じはじめる角度[deg]
		pman->konai_end_pos=		90.0;//[deg]	//●絶対「遠ざかる」と感じる角度[deg]
		//pman->konai_start_pos=		45.0;//[deg]	//●「遠ざかる」と感じはじめる角度[deg]
		//pman->konai_end_pos=		110.0;//[deg]	//●絶対「遠ざかる」と感じる角度[deg]

		//意識強度
		pman->near_kuru_base=		1.0;			//★近い・近づく　場合の係数
		pman->near_konai_base=		0.5;			//★近い・遠ざかる　場合の係数
		pman->fars_kuru_base=		0.2;			//★遠い・近づく　場合の係数
		pman->fars_konai_base=		0.0;			//★遠い・遠ざかる　場合の係数
		//
//左右2チームに分ける
		long	team=				( nn % 2 );							//偶数と奇数で分ける
		switch( team ){
			case 0:{				//左側チーム
				//Ｙ座標を計算
				long	tiem_i=					nn / 2 ;				//チーム内の番号0,1,2,3,
				pos_y=	((tiem_i+1)/2) * m_field_width / m_man_max;		//Ｙ座標は二人ずつ絶対値が同じになる
				if( ( tiem_i % 2 ) == 0 ) pos_y=	-pos_y;				//偶数番目の人は上側に配置

				//最初の位置
				pman->m_now_pos.x=				0;						//偶数は左
				pman->m_now_pos.y=				-pos_y;					//幅いっぱいに整列　なので　最初は重なっている

				pman->m_color_code=				5;	//水色				//円の色番号
				pman->m_brush_code=				5;	//水色				//塗りつぶしの色番号
				//目的の位置
				pman->m_now_target_pos.x=		m_field_length;			//立ち位置の反対側
				pman->m_now_target_pos.y=		0;						//幅の中心位置
			}break;

			case 1:{				//右側チーム
				//Ｙ座標は左チームと同じ
				//最初の位置
				pman->m_now_pos.x=				m_field_length;			//奇数は右
				pman->m_now_pos.y=				-pos_y;					//幅いっぱいに整列　なので　最初は重なっている

				pman->m_color_code=				6;	//黄色				//円の色番号
				pman->m_brush_code=				0;	//黒				//塗りつぶしの色番号
				//目的の位置
				pman->m_now_target_pos.x=		0;						//立ち位置の反対側
				pman->m_now_target_pos.y=		0;						//幅の中心位置
			}break;

		}
		//
		xfunc_unit_vect_pp( pman->m_now_pos , pman->m_now_target_pos , &pman->m_now_target_dir );		//最初の進行方向（単位ベクトル）
		pman->m_now_target_angle=		xfunc_kakudo( 0,0 , pman->m_now_target_dir.x , pman->m_now_target_dir.y , NULL , NULL );							//進行方向角度[deg]
	}
}


//------------------------------------------------------------------------------
//		デストラクタ（破棄する時の処理）
//------------------------------------------------------------------------------
x_simuA::~x_simuA()
{
	free( m_man_ptr );							//各人の属性の破棄
}




//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//		描画（裏で書いて表に出す）
//------------------------------------------------------------------------------
void	x_simuA::disp_simu()
{
	//作成するビットマップのサイズ
	RECT	rect;
	GetWindowRect( m_hwnd , &rect );																//ウィンドウのサイズを取得
	long	width=				rect.right - rect.left;
	long	height=				rect.bottom - rect.top;
	
	//ビットマップの生成
	HDC		src_hdc=			GetDC( m_hwnd );													//表のＨＤＣを取得
	HBITMAP	mem_bitmap=			CreateCompatibleBitmap( src_hdc , width , height ) ;				//ビットマップを作成
	HDC		m_Mem_hdc=			CreateCompatibleDC( src_hdc ) ;										//裏ＤＣを生成
	HBITMAP	m_old_Mem_bitmap=	(HBITMAP)SelectObject( m_Mem_hdc , mem_bitmap ) ;					//裏ＤＣにビットマップをリンク
	//

	//ペンとブラシを作る（時間のかかる処理なので逐一やらない）
	long	rgb[8]=				{0 , RGB(0,0,255) , RGB(255,0,0) , RGB(255,0,255) , RGB(0,255,0) , RGB(0,255,255) , RGB(255,255,0) , RGB(255,255,255)};	//基本の7色
	m_color_pen[0]=				(HPEN)GetStockObject( NULL_PEN );								//透明なペン
	m_color_brush[0]=			(HBRUSH)GetStockObject( NULL_BRUSH );							//透明なブラシ
	for( long ii=1;ii<8;ii++){
		m_color_pen[ii]=		CreatePen( PS_SOLID , 1 , rgb[ii] );							//ペン
		m_color_brush[ii]=		CreateSolidBrush( rgb[ii] );									//ブラシ
	}
	//
	HBRUSH	old_brush=			(HBRUSH)SelectObject( m_Mem_hdc , GetStockObject( NULL_BRUSH ) );	//とりあえず透明なブラシを選択（HDCは選択されたブラシをロックしてしまうので、最後に元に戻すため）
	HPEN	old_pen=			(HPEN)SelectObject( m_Mem_hdc , m_color_pen[0] );					//とりあえず歴東名ペンを選択（HDCは選択されたペンをロックしてしまうので、最後に元に戻すため）
	//
	
	//全員描画
	for(long ii=0;ii<m_man_max;ii++){																//全員ループ
		disp_man_one( m_Mem_hdc , ii );																//一人づつ描画
	}
	//	
	
	//状況文字列の表示
	double	cal_sokudo=			m_genzai_jikoku_sec * 1000.0 / (double)( timeGetTime() - m_simu_start_time - m_simu_tyuudann_time );		//計算速度
	char	wwc[200];
	sprintf( wwc , "  %6.1f[秒](%5.1f倍)%5.3f[秒/ステップ] %d[人] W:ベクトル　S:停止/再開　R:初めから" , m_genzai_jikoku_sec , cal_sokudo , mf_cal_interval , m_man_max );	//経過時間と人数
	xfunc_print_text( m_Mem_hdc , wwc , 0 , 0 , 32 , RGB(255,255,255) , "ＭＳ ゴシック" , -1, 0.0 , 0 );
	
	//裏で書いたビットマップを表に転送
	BitBlt( src_hdc , 0 , 0 , width , height , m_Mem_hdc , 0 , 0 , SRCCOPY );						//表に出す


	//リソースの開放
	SelectObject( m_Mem_hdc , old_pen );															//ブラシを元に戻す
	SelectObject( m_Mem_hdc , old_brush );															//ペンを元に戻す
	//グラフィックリソースの破棄
	for(long ii=0;ii<8;ii++){
		DeleteObject( m_color_pen[ii] );															//ペンを破棄
		DeleteObject( m_color_brush[ii] );															//ブラシを破棄
	}
	//
	
	//デバイスの切り離し
	ReleaseDC( m_hwnd , src_hdc ) ;																	//src_hdcをm_hwnから切り離す（　GetDC( m_hwnd )と対　）

	//ビットマップとデバイスコンテキストの破棄
	SelectObject( m_Mem_hdc , m_old_Mem_bitmap ) ;													//元のビットマップに戻す
	DeleteObject( mem_bitmap );																		//裏のビットマップを破棄
	DeleteDC( m_Mem_hdc );																			//裏のＤＣを破棄
}



//------------------------------------------------------------------------------
//		表示
//------------------------------------------------------------------------------
long x_simuA::disp_man_one( HDC hdc , long index )
{

	S_MAN*	p_man=		&m_man_ptr[index];
	//
	dPOINT	center;
	real_to_view( p_man->m_now_pos.x , p_man->m_now_pos.y , &center.x , &center.y ) ;															//中心座標
	dPOINT	left_top;
	real_to_view( p_man->m_now_pos.x - p_man->body_haba , p_man->m_now_pos.y - p_man->body_haba , &left_top.x , &left_top.y ) ;					//左上
	dPOINT	right_bottom;
	real_to_view( p_man->m_now_pos.x + p_man->body_haba , p_man->m_now_pos.y + p_man->body_haba , &right_bottom.x , &right_bottom.y ) ;			//右下
	
	//人の本体
	SelectObject( hdc , m_color_pen[p_man->m_color_code] );																						//指定職で円を描く
	SelectObject( hdc , m_color_brush[p_man->m_brush_code] );																					//指定職で塗りつぶす
	Ellipse( hdc , (long)(left_top.x + 0.5 ) , (long)(left_top.y  + 0.5 ), (long)(right_bottom.x + 0.5 ) , (long)(right_bottom.y + 0.5 ) );		//円を描く
	
	//向きベクトルの描画
	if( m_disp_vector_sw == 1 ){
		//向きを描く  //fuzzy
		double	bairitu=		1;
		//
		dPOINT	target;
		bairitu=		p_man->m_walk_speed;			//歩行速度を倍率にする
		real_to_view( p_man->m_now_pos.x + p_man->m_next_dir.x * bairitu , p_man->m_now_pos.y+ p_man->m_next_dir.y * bairitu , &target.x , &target.y ) ;			//決定方向を描く
		SelectObject( hdc , m_color_pen[7] );
		MoveToEx( hdc , (long)(center.x + 0.5 ) , (long)(center.y + 0.5 ) , NULL );
		LineTo( hdc , (long)(target.x + 0.5 ) , (long)(target.y + 0.5 ) );
		//
		dPOINT	fdir;
		real_to_view( p_man->m_now_pos.x + p_man->m_sum_fuzzy_vect.x * bairitu , p_man->m_now_pos.y+ p_man->m_sum_fuzzy_vect.y * bairitu , &fdir.x , &fdir.y ) ;		//ファジィー力を描く
		SelectObject( hdc , m_color_pen[6] );
		MoveToEx( hdc , (long)(center.x + 0.5 ) , (long)(center.y + 0.5 ) , NULL );
		LineTo( hdc , (long)(fdir.x + 0.5 ) , (long)(fdir.y + 0.5 ) );
		//
		dPOINT	deside;
		real_to_view( p_man->m_now_pos.x + p_man->m_now_target_dir.x * bairitu , p_man->m_now_pos.y+ p_man->m_now_target_dir.y * bairitu , &deside.x , &deside.y ) ;	//ターゲット方向を描く
		SelectObject( hdc , m_color_pen[2] );
		MoveToEx( hdc , (long)(center.x + 0.5 ) , (long)(center.y + 0.5 ) , NULL );
		LineTo( hdc , (long)(deside.x + 0.5 ) , (long)(deside.y + 0.5 ) );
	}

	return 1 ;
}


//-------------------------------------------------------------------------------------
//		シミュレーション用実座標をスクリーン座標に変換
//-------------------------------------------------------------------------------------
void x_simuA::real_to_view( double rx1 , double ry1 , double* vx1 , double* vy1 )
{
	*vx1=  rx1 * m_disp_ratio;
	*vy1= -ry1 * m_disp_ratio + m_disp_offset_y;
}


//-------------------------------------------------------------------------------------
//		スクリーン座標をシミュレーション用実座標に変換
//-------------------------------------------------------------------------------------
void x_simuA::view_to_real( double vx1 , double vy1 , double* rx1 , double* ry1 )
{
	*rx1=	 vx1 / m_disp_ratio ;
	*ry1=	-(vy1 - m_disp_offset_y) / m_disp_ratio ;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//		全員の移動処理　１ステップ実行
//------------------------------------------------------------------------------
long	x_simuA::next_step_all()
{
	for(long ii=0;ii<m_man_max;ii++){
		next_step_one( ii );			//この中にも	for(long ii=0;ii<m_man_max;ii++){}　がある。しかも最大４回呼び出す
														//100人ならば  100×100×4
														//1000人ならば 1000×1000×4 となり、人数の2乗で計算量が増える。
	}
	return 0;
}


//------------------------------------------------------------------------------
//		一人の移動
//------------------------------------------------------------------------------
long x_simuA::next_step_one( long index )
{
	//
	make_fuzzy_new_vector(index);						//ファジィベクトルの計算
	//
	S_MAN*	p_man=		&m_man_ptr[index];
	//目標までの単位ベクトル
	xfunc_unit_vect_pp( p_man->m_now_pos , p_man->m_now_target_pos , &p_man->m_now_target_dir );		//目的地点方向ベクトル
	//ファジィで修正 //fuzzy
	dPOINT	fuzzy_dd=	p_man->m_now_target_dir + p_man->m_sum_fuzzy_vect;							//目的方向ベクトルとファジィ方向ベクトルの和
	dPOINT	first_next_dir;
	xfunc_unit_vect( 0 , 0 , fuzzy_dd.x , fuzzy_dd.y , &first_next_dir.x , &first_next_dir.y );		//最初の進む方向を単位ベクトルに
	p_man->m_next_dir=	first_next_dir;															//次に進む方向に設定
	long	next_ok=	check_next_position( index ) ;											//進む方向に人がいるか
	if( next_ok == FALSE ){																		//進む方向に人が居たら
		//右に進む
		p_man->m_next_dir.x=		 first_next_dir.y;												//右に進む
		p_man->m_next_dir.y=		-first_next_dir.x;
		next_ok=	check_next_position( index ) ;												//進む方向に人がいるか
		if( next_ok == FALSE ){
			//左に進む
			p_man->m_next_dir.x=	-first_next_dir.y;												//左に進む
			p_man->m_next_dir.y=	 first_next_dir.x;
			next_ok=	check_next_position( index ) ;											//進む方向に人がいるか
			if( next_ok == FALSE ){
				//後ろに進む
				p_man->m_next_dir.x=	-first_next_dir.x;											//後ろに進む
				p_man->m_next_dir.y=	-first_next_dir.y;
				next_ok=	check_next_position( index ) ;										//進む方向に人がいるか
				if( next_ok == FALSE ){
					return FALSE;																//移動を諦める
				}
			}
		}
	}
	//
	p_man->m_now_pos=				p_man->m_now_pos + p_man->m_next_dir * p_man->m_walk_speed * mf_cal_interval;		//次のステップの地点に移動
	//移動後の目的ポイント方向の更新
	xfunc_unit_vect_pp( p_man->m_now_pos , p_man->m_now_target_pos , &p_man->m_now_target_dir );		//最初の進行方向（単位ベクトル）
	p_man->m_now_target_angle=		xfunc_kakudo( 0,0 , p_man->m_now_target_dir.x , p_man->m_now_target_dir.y , NULL , NULL );															//進行方向角度[deg]
	//
	return TRUE;
}


//------------------------------------------------------------------------------
//		次の位置に移動可能かどうかチェック
//------------------------------------------------------------------------------
long	x_simuA::check_next_position( long index )
{
	S_MAN*	a_san=		&m_man_ptr[index];
	dPOINT	next_pos=	a_san->m_now_pos + a_san->m_next_dir * a_san->m_walk_speed * mf_cal_interval;		//次の位置
	long	over_sw=	TRUE;
	for(long ii=0;ii<m_man_max;ii++){												//全員をチェック
		if( ii == index ) continue;													//自分は無視
		S_MAN* b_san=			&m_man_ptr[ii];
		double	next_dist=		xfunc_dist_pp( b_san->m_now_pos , next_pos );			//次の距離[m]
		if( next_dist > ( a_san->body_haba + b_san->body_haba ) ) continue;			//接触しない
		//
		double	now_dist=		xfunc_dist_pp( b_san->m_now_pos , a_san->m_now_pos );	//現在の距離[m]
		if( next_dist > now_dist ) continue;										//接触していても遠ざかる方向へは移動を許す
		//
		over_sw=		FALSE;														//移動はできない
		break;
	}
	return over_sw;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//		ファジィーで回避ベクトルを計算
//	相手までの距離　と　相手の向き　を使って
//	自分の意識の程度　を　推定し
//	進行方向と直交するベクトルを作成する
//------------------------------------------------------------------------------
long	x_simuA::make_fuzzy_new_vector( long index )
{

	S_MAN*	a_san=		&m_man_ptr[index];			//自分
	a_san->m_sum_fuzzy_vect=		0.0;				//ファジィ推論の結果を合計した値　(周囲に影響する人がいなかった場合はゼロ）
	//
	if( a_san->m_walk_speed <= 0.0 ){				//動かない時は計算はいらない
		return FALSE;								//結果なし
	}
	//
	//ファジィー数
	dPOINT	sum_fuzzy_dd=			{0.0,0.0};		//ファジィ推論の結果を合計するための変数
	long	sum_man=				0;				//影響を受けた人数
	//
	for(long ii=0;ii<m_man_max;ii++){																//★全員をチェック
		if( ii == index ) continue;																	//自分は無視する
		//
		S_MAN* b_san=		&m_man_ptr[ii];															//相手（Ｂさん）の属性の構造体ポインタ
		//前方に居ない人は無視
		double	a_b_angle=			xfunc_kakudo( a_san->m_now_pos.x , a_san->m_now_pos.y , b_san->m_now_pos.x , b_san->m_now_pos.y , NULL,NULL);	//Ａから見たＢの方向[deg]
		double	a_mesen_b_angle=	xfunc_angle_pm180( a_b_angle - a_san->m_now_target_angle ) ;	//Ａの進行方向を基準にしたＢの方向[deg]
		if( fabs( a_mesen_b_angle ) > a_san->m_zenpou_angle ) continue ;							//▲前方に居ない人は視界に入らないとして無視する

		//影響を推測
		double	fuzzy_v=			fuzzy_suiron( index , ii );										//ファジィ推論
		if( fuzzy_v <= 0.0 ) continue;																//影響のない人は無視
		//
		sum_man++;																					//影響した人数
		//ファジィによる回避強度の集計(考え方でいろいろ出来る)ここでは目的方向に対し直交する方向に作用すると考える。
		{
			//避ける向き　自分の向きと直行方向
			double	aite_side=	a_mesen_b_angle ;													//進行方向を基準にした相手方向
			double	fuzzy_angle ;
			if( aite_side > 0.0 ){																	//相手は左側にいる
				fuzzy_angle=	xfunc_angle360( a_san->m_now_target_angle - 90.0 );					//右側に逃げる
			}else{																					//相手は右側にいる
				fuzzy_angle=	xfunc_angle360( a_san->m_now_target_angle + 90.0 );					//左側に逃げる
			}
			//ＸＹ成分で記録（aite_sideで正負を決めてfuzzy_vを合計しておいて最後にsum_fuzzy_ddを計算しても同じ）
			double fuzzy_dx=		fuzzy_v * cos( fuzzy_angle / 180.0 * u_PAI );
			double fuzzy_dy=		fuzzy_v * sin( fuzzy_angle / 180.0 * u_PAI );
			sum_fuzzy_dd.x+=		fuzzy_dx;
			sum_fuzzy_dd.y+=		fuzzy_dy;

		}
	}
	//
	if( sum_man > 0 ){								//周囲に影響する人が居た場合（進行方向に直行するベクトルになる）
		a_san->m_sum_fuzzy_vect=		sum_fuzzy_dd;
	}
	//
	return TRUE ;									//結果あり
}


//-------------------------------------------------------------------------------------
//		相手から受ける影響の程度
//-------------------------------------------------------------------------------------
double x_simuA::fuzzy_suiron(long my_index , long aite_index )
{
	S_MAN* a_san=				&m_man_ptr[my_index];												//自分（Ａさん）の属性の構造体ポインタ
	S_MAN* b_san=				&m_man_ptr[aite_index];												//相手（Ｂさん）の属性の構造体ポインタ

	//距離
	double	body_size2=			a_san->body_haba + b_san->body_haba ;								//自分と相手の半径の和が中心間距離の限界距離[m]
	double	now_dist=			xfunc_dist_pp( b_san->m_now_pos , a_san->m_now_pos ) - body_size2 ;	//現在の間距離[m]
	if( now_dist > a_san->far_end_pos )  return 0.0 ;												//絶対遠いと感じる距離より遠い人は無視する

	//相手から見た自分の角度
	double	b_a_angle=			xfunc_kakudo_pp( b_san->m_now_pos , a_san->m_now_pos , NULL,NULL);	//Ｂから見たＡの方向[deg]
	double	b_mesen_a_angle=	xfunc_angle_pm180( b_a_angle - b_san->m_now_target_angle );			//Ｂの進行方向を基準にしたＡの方向[deg]
	double	check_angle=		fabs(b_mesen_a_angle);

	//前方で交差しない位置関係は無視
	double	a_b_angle=			xfunc_kakudo_pp( a_san->m_now_pos , b_san->m_now_pos , NULL,NULL);	//Ａから見たＢの方向[deg]
	double	a_mesen_b_angle=	xfunc_angle_pm180( a_b_angle - a_san->m_now_target_angle ) ;		//Ａの進行方向を基準にしたＢの方向[deg]
	double	closs_angle=		fabs( b_mesen_a_angle + a_mesen_b_angle );							//角度の合計の絶対値(0-360)[deg]
	if( closs_angle > 180.0 ) return 0.0;															//前方で交差しない位置関係

	//ファジィ真理値
	double	f_nears=			xfunc_fuzzy_strong( now_dist	, a_san->near_start_pos		, a_san->near_end_pos	) ;	//近いと感じる程度
	double	f_farss=			xfunc_fuzzy_strong( now_dist	, a_san->far_start_pos		, a_san->far_end_pos	) ;	//遠いと感じる程度
	double	f_kuru=				xfunc_fuzzy_strong( check_angle	, a_san->kuru_start_pos		, a_san->kuru_end_pos	) ;	//向かって来ると感じる程度
	double	f_konai=			xfunc_fuzzy_strong( check_angle	, a_san->konai_start_pos	, a_san->konai_end_pos	) ;	//遠ざかると感じる程度

	//台形の面積
	double	f_nears_kuru=		xfunc_daikei_area( f_nears * f_kuru		, 1 , 1 ) ;				//近い・近づく
	double	f_nears_konai=		xfunc_daikei_area( f_nears * f_konai	, 1 , 1 ) ;				//近い・遠ざかる
	double	f_farss_kuru=		xfunc_daikei_area( f_farss * f_kuru		, 1 , 1 ) ;				//遠い・近づく
	double	f_farss_konai=		xfunc_daikei_area( f_farss * f_konai	, 1 , 1 ) ;				//遠い・遠ざかる
		
	//推論結果（4つの台形の重心位置）水本雅晴の「代数積-加算-重心法」の中の一つ	<=	マムダニ先生の「min-max-重心法」に変わる推論方法
	double	fuzzy_v= (  f_nears_kuru  * a_san->near_kuru_base + f_nears_konai * a_san->near_konai_base
					  + f_farss_kuru  * a_san->fars_kuru_base + f_farss_konai * a_san->fars_konai_base )
					/ ( f_nears_kuru + f_nears_konai + f_farss_kuru + f_farss_konai ) ;
	//
	return fuzzy_v;
}



//----------------------------------------------------------------------------
//		台形の面積
//----------------------------------------------------------------------------
double	 x_simuA::xfunc_daikei_area(double height, double bottom_length , double top_height )
{
	double	area=		( bottom_length * ((top_height - height) / top_height) + bottom_length ) * height / 2.0 ;
	return area ;
}


//----------------------------------------------------------------------------
//		ファジィ真理値
//		start_pos以下なら偽（値0)	end_pos以上なら真(値1)
//----------------------------------------------------------------------------
double	 x_simuA::xfunc_fuzzy_strong( double val , double start_pos , double end_pos )
{
	double	strongness=		0;
	if( start_pos < end_pos ){		//開始が終了より小さい		start_pos____/~~~~~end_pos
		if( val < start_pos ){
			strongness=		0 ;
		}else if( val < end_pos ){
			strongness=		(val - start_pos) / (end_pos - start_pos);		//0-1の値
		}else{
			strongness=		1;
		}
	}else{							//開始が終了より大きい		end_pos~~~~\______start_pos
		if( val < end_pos ){
			strongness=		1;
		}else if( val < start_pos ){
			strongness=		(start_pos - val) / (start_pos - end_pos);		//0-1の値
		}else{
			strongness=		0;
		}
	}
	return strongness;
}

