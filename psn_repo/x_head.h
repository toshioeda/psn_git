#pragma once


//------------------------------------------------------------------------------
//		特別にシミュレーション条件をここで定義する
//		普通は可搬性を考えて、ここでは定義しない
//------------------------------------------------------------------------------
#define cdef_MESH_METHOD			//メッシュ利用のプログラムにする時の宣言
#undef cdef_MESH_METHOD				//この行を有効にするとA　　コメントアウトでB（メッシュ利用）のシミュレーションになる

#define		chead_NINZUU			160			//☆　計算人数[psn]
#define		chead_FIELD_LENGTH		20			//☆　フィールド長さ[m]
#define		chead_FIELD_WIDTH		1			//☆　初期配置幅[m]



//------------------------------------------------------------------------------
//	全体に共通する宣言
//------------------------------------------------------------------------------
#define		u_PAI					3.14159265358979


//------------------------------------------------------------------------------
//	ダブル精度の座標の構造体
//------------------------------------------------------------------------------
typedef struct	s_dPOINT{
	BOOL operator == ( s_dPOINT ) ;			//equequ
	s_dPOINT operator = ( double );			//equdouble
	s_dPOINT operator + ( double );			//plusdouble
	s_dPOINT operator - ( double );			//mainusdouble
	s_dPOINT operator * ( double );			//times
	s_dPOINT operator / ( double );			//waru



	s_dPOINT operator - ( s_dPOINT );		//dpointmainasudpoint
	s_dPOINT operator + ( s_dPOINT );		//pluspoint
	s_dPOINT operator * ( s_dPOINT );		//timestimes
	double	x ;
	double	y ;
} dPOINT ;


inline	dPOINT dPOINT::operator = ( double vv )		//equdouble
{
	dPOINT	ansd ;
	x=	ansd.x=		vv ; 
	y=	ansd.y=		vv ;
	return ansd ;
};
inline	dPOINT dPOINT::operator + ( double vv )		//plusdouble
{
	dPOINT	ansd ;
	ansd.x=		x + vv ; 
	ansd.y=		y + vv ;
	return ansd ;
};
inline	dPOINT dPOINT::operator - ( double vv )		//mainusdouble
{
	dPOINT	ansd ;
	ansd.x=		x - vv ; 
	ansd.y=		y - vv ;
	return ansd ;
};

inline	dPOINT dPOINT::operator * ( double vv )		//times
{
	dPOINT	ansd ;
	ansd.x=		x * vv; 
	ansd.y=		y * vv;
	return ansd;
};
inline	dPOINT dPOINT::operator / ( double vv )		//waru
{
	dPOINT	ansd ;
	ansd.x=		x / vv; 
	ansd.y=		y / vv;
	return ansd;
};



inline	BOOL dPOINT::operator == ( dPOINT dp )			//equequ
{
	double	lv=			0.000000001 ;
	if( fabs(x - dp.x )<lv && fabs(y - dp.y )<lv ){
		return TRUE ;
	}
	return FALSE ;
};


inline	dPOINT dPOINT::operator - ( dPOINT dp )		//dpointmainasudpoint
{
	dPOINT	ansd ;
	ansd.x=			x - dp.x ; 
	ansd.y=			y - dp.y ;
	return ansd ;
};

inline	dPOINT dPOINT::operator + ( dPOINT dp )	//pluspoint
{
	dPOINT	ansd ;
	ansd.x=			x + dp.x ; 
	ansd.y=			y + dp.y ;
	return ansd ;
};


inline	dPOINT dPOINT::operator * ( dPOINT dp )		//timestimes
{
	dPOINT	ansd ;
	ansd.x=			x * dp.x ; 
	ansd.y=			y * dp.y ;
	return ansd ;
};


//------------------------------------------------------------------------------
//	人の属性の構造体
//------------------------------------------------------------------------------
typedef struct s_man{
//個人の設定項目
	//性能
	double	body_haba;				//人の半径[m]
	double	m_walk_speed;			//歩行速度[m/s]
	double	m_zenpou_angle;			//前方にいると判断できる視野角
	//ファジィー　メンバーシップ
	//近い
	double	near_start_pos;			//近いと感じ始める距離[m]（中心間距離から、体半径を除いた、空隙距離）
	double	near_end_pos;			//絶対近いと感じる距離[m]
	//遠い
	double	far_start_pos;			//遠いと感じ始める距離[m]
	double	far_end_pos;			//絶対遠いと感じる距離[m]
	//向かってくる
	double	kuru_start_pos;			//「向かってくる」と感じはじめる相手の顔の角度[deg](0-90)
	double	kuru_end_pos;			//絶対「向かってくる」と感じる相手の顔の角度[deg](0-90) < start
	//遠ざかる
	double	konai_start_pos;		//「遠ざかる」と感じはじめる角度[deg](0-180)
	double	konai_end_pos;			//絶対「遠ざかる」と感じる角度[deg](0-180) > start
	//ファジィー　強度
	double	near_kuru_base;			//近い・近づく　　場合の設定強度[0-1]
	double	near_konai_base;		//近い・遠ざかる　場合の設定強度[0-1]
	double	fars_kuru_base;			//遠い・近づく　　場合の設定強度[0-1]
	double	fars_konai_base;		//遠い・遠ざかる　場合の設定強度[0-1]
//位置情報
	dPOINT	m_now_pos;				//現在位置
	dPOINT	m_now_target_pos;		//目的位置


	long	m_mesh_heya_bangou;		//メッシュの登録インデックス


//計算結果
	dPOINT	m_now_target_dir;		//目的位置の方向(単位ベクトル)
	double	m_now_target_angle;		//目的方向の角度[deg]
	dPOINT	m_next_dir;				//次（今）の進行方向(単位ベクトル)
	dPOINT	m_sum_fuzzy_vect;		//１対１で発生する回避意識強度の周囲全員の合計ベクトル（この決め方は自由）
//その他
	long	m_color_code;			//色分け用コード(1:青 2:赤 3:紫 4:緑 5:水色 6:黄色 7:白）
	long	m_brush_code;			//色分け用コード(1:青 2:赤 3:紫 4:緑 5:水色 6:黄色 7:白）
	//
}S_MAN;


