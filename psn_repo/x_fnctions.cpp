//------------------------------------------------------------------------------
//		基本関数パック
//------------------------------------------------------------------------------
#include <Windows.h>
#include <math.h>
//
#include "x_head.h"
#include "x_functions.h"
//
#pragma warning(disable : 4996)

//--------------------------------------------------
//		角度を360度以内に変換する
//--------------------------------------------------
double	xfunc_angle360( double angle )
{
	while( angle < 0		) angle+=	360.0;
	while( angle > 360.0	) angle-=	360.0 ;
	return angle ;
}


//--------------------------------------------------
//		角度を±180度に変換する
//--------------------------------------------------
double	xfunc_angle_pm180( double angle )
{
	angle=		xfunc_angle360(angle);
	if( angle > 180.0 ){
		angle-=			360.0 ;
	}
	return angle ;
}


//------------------------------------------------------------------------------
//		角度[deg]
//------------------------------------------------------------------------------
double	xfunc_kakudo_pp( dPOINT p1 , dPOINT p2 , double* p_deg , double* p_rad )
{
	return xfunc_kakudo( p1.x , p1.y , p2.x , p2.y , p_deg , p_rad );
}

double	xfunc_kakudo( double x1,double y1,double x2,double y2,double* p_deg,double* p_rad)
{
	double	dx=		x2 - x1 ;
	double	dy=		y2 - y1 ;
	double	rad_angle ;
	if( dx == 0.0 && dy == 0.0 ){
		rad_angle=	0.0 ;
	}else{
		rad_angle=	atan2( dy , dx );
	}
	if( rad_angle < 0 ){
		rad_angle+=		u_PAI ;
		rad_angle+=		u_PAI ;
	}
	if( p_rad != NULL ){
		*p_rad=			rad_angle ;
	}
	double	deg_angle=	rad_angle / u_PAI * 180 ;
	if( p_deg != NULL ){
		*p_deg=			deg_angle ;
	}
	return deg_angle ;
}


//------------------------------------------------------------------------------
//		単位ベクトル
//------------------------------------------------------------------------------
double	xfunc_unit_vect_pp( dPOINT p1 , dPOINT p2 , dPOINT* unit_vect )
{
	return xfunc_unit_vect( p1.x , p1.y , p2.x , p2.y , &unit_vect->x , &unit_vect->y );
}
double	xfunc_unit_vect( double x1 , double y1 , double x2 , double y2 , double*unit_x , double*unit_y )
{
	double	dd=		xfunc_dist( x1 , y1 , x2 , y2 );
	double	dx=		x2-x1;
	double	dy=		y2-y1;
	if( dd > 1e-10 ){
		dx/=	dd ;
		dy/=	dd ;
	}else{
		dx=		0.0;
		dy=		0.0;
	}

	if( unit_x		!= NULL ) *unit_x=		dx ;
	if( unit_y		!= NULL ) *unit_y=		dy ;
	return dd ;
}


//------------------------------------------------------------------------------
//		2点間距離
//------------------------------------------------------------------------------
double	xfunc_dist_pp( dPOINT p1 , dPOINT p2 )
{
	return xfunc_dist( p1.x , p1.y , p2.x , p2.y );
}
double	xfunc_dist( double x1 , double y1 , double x2 , double y2 )
{
	double	ppdx=		x2 - x1;
	double	ppdy=		y2 - y1;
	double	ppdx2=		ppdx * ppdx;
	double	ppdy2=		ppdy * ppdy;
	if( ppdx2 < 1e-200 ) ppdx2=	0.0;			//十分小さい値のごみを排除
	if( ppdy2 < 1e-200 ) ppdy2=	0.0;
	double	dist=		sqrt( ppdx2 + ppdy2 );
	if( dist < 1e-200 ) dist=	0.0;			//十分小さい値のごみを排除
	return	dist;
}


//------------------------------------------------------------------------------
//		文字の表示
//------------------------------------------------------------------------------
void	xfunc_print_text( HDC hdc , char* wchar , double tx , double ty , long height , long f_color , char* font_name , long back_color , double angle , long bold )
{
	if( height <= 0 ) return;

	// 文字関連
	HFONT		newfont ;
	HFONT		oldfont ;
	LOGFONT		fontatr ;
	//ログフォント
	memset( &fontatr , 0 , sizeof(LOGFONT) );
	//高さ
	fontatr.lfHeight=			(long)(height+0.5);			// 高さ
	fontatr.lfWidth=			0;							// Ｗｉｎピッチ
	fontatr.lfEscapement=		(long)( angle * 10.0 + 0.5 );			// Angle(1/10deg)
	fontatr.lfOrientation=		0;							// ENABLE	Durection of Each Character
	fontatr.lfWeight=			400;						//400 	BOLD( 700 )
	if( bold != 0 )fontatr.lfWeight=			700;
	fontatr.lfItalic=			0;							// Italic ( ==0:Not Italic !=0 Italic )
	fontatr.lfUnderline=		0;							// Under Line( ==0 No !=0 Under Line )
	fontatr.lfStrikeOut=		0;							// 0以外のとき、打ち消し線付きのフォントを示します。
	fontatr.lfCharSet=			DEFAULT_CHARSET;				// フォントの文字セットを示します。
	fontatr.lfOutPrecision=		OUT_TT_PRECIS;				// フォントの出力精度を示します。
	fontatr.lfOutPrecision=		OUT_DEFAULT_PRECIS;
	fontatr.lfClipPrecision=	CLIP_EMBEDDED;				// クリッピング精度を示します。
	fontatr.lfClipPrecision=	CLIP_DEFAULT_PRECIS;
	fontatr.lfQuality=			DEFAULT_QUALITY;				// フォントの出力品質を示します。
	fontatr.lfPitchAndFamily=	DEFAULT_PITCH | FF_DONTCARE;	// フォントのピッチとファミリを示します。
	// フォント
	if( font_name != NULL && font_name[0] != 0x00 ){
		strcpy( fontatr.lfFaceName , font_name ) ;
	}else{
		strcpy( fontatr.lfFaceName , "ＭＳ Ｐ明朝" ) ;			// フォントの書体名を示します。
	}
	SetROP2( hdc , 13 ) ;										// 描画モード or:15	xor:7  pset:13	reset:1
	//背景色
	if( back_color >= 0 ){
		SetBkColor( hdc , back_color );
		SetBkMode( hdc , OPAQUE  );								//塗りつぶし
	}else{
		SetBkMode( hdc , TRANSPARENT );							//透過
	}
	// フォントの色
	SetTextColor( hdc , f_color ) ;
	//
	newfont=		CreateFontIndirect( &fontatr );								//描画するフォントの生成
	oldfont=		( HFONT )SelectObject( hdc , newfont );						// フォントの選択
	int	bytelen=	(int)strlen( wchar );										//文字列バイト長さ
	TextOut( hdc , long( tx ) , long( ty ) , wchar , bytelen );	// ＷＩｎピッチの文字
	SelectObject( hdc,oldfont );												// フォントを戻す
	DeleteObject( newfont );													// フォントの削除

}

//------------------------------------------------------------------------------
//		実数の乱数(0.0-1.0)
//------------------------------------------------------------------------------
double	xfunc_rnd()
{
	double	rv=		(double)rand() / ( RAND_MAX + 1 );
	return rv;
}
//------------------------------------------------------------------------------
//乱数初期化
//------------------------------------------------------------------------------
void xfunc_randomize()
{
	unsigned int	seed=		timeGetTime();			//システム時刻をミリ秒単位で取得します。システム時刻は Windows が起動してから経過した時間です。
	srand( seed );
}


//------------------------------------------------------------------------------
//		GDIの基本的な使い方
//------------------------------------------------------------------------------
void use_GDI( HWND hwnd )
{
	//グラフィックリソースの生成
	HDC		hdc=		GetDC( hwnd );								//ウィンドウのハンドルに関連付けされているデバイスコンテキストのハンドルを取り出す
	HPEN	new_pen=	CreatePen( PS_SOLID , 1 , RGB(255,0,0) );	//ペンを作成
	HBRUSH	new_brush=	CreateSolidBrush( RGB(0,255,0) );			//ブラシを作成
	HPEN	old_pen=	(HPEN)SelectObject( hdc , new_pen );		//デバイスコンテキストに使うペンをセット。先にセットされていたペンのハンドルが返る。
	HBRUSH	old_brush=	(HBRUSH)SelectObject( hdc , new_brush );	//デバイスコンテキストに使うブラシをセット。先にセットされていたブラシのハンドルが返る。
	//

	//仮の座標
	long	x1=		100;
	long	y1=		200;
	long	x2=		200;
	long	y2=		300;
	//
	MoveToEx( hdc , x1 , y1 , NULL );						//始点
	LineTo( hdc , x2 , y2 );								//終点
	//
	Ellipse( hdc , x1 , y1 , x2 , y2 );						//円
	//
	SelectObject( hdc , GetStockObject( NULL_BRUSH ) );		//塗潰し無し
	Rectangle( hdc , x1 , y2 , x2 , y2 );					//四角形
	//
	
	//グラフィックリソースの破棄
	SelectObject( hdc , old_pen );							//デバイスコンテキストで使うペンを最初にセットされていたペンと取り換える。
	SelectObject( hdc , old_brush );						//デバイスコンテキストで使うブラシを最初にセットされていたブラシと取り換える。
	DeleteObject( new_pen );								//作ったペンを破棄する
	DeleteObject( new_brush );								//作ったブラシを破棄する
	ReleaseDC( hwnd , hdc );								//ウィンドウのハンドルに関連付けされているデバイスコンテキストのハンドルを自由にする
}
