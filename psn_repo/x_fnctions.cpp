//------------------------------------------------------------------------------
//		��{�֐��p�b�N
//------------------------------------------------------------------------------
#include <Windows.h>
#include <math.h>
//
#include "x_head.h"
#include "x_functions.h"
//
#pragma warning(disable : 4996)

//--------------------------------------------------
//		�p�x��360�x�ȓ��ɕϊ�����
//--------------------------------------------------
double	xfunc_angle360( double angle )
{
	while( angle < 0		) angle+=	360.0;
	while( angle > 360.0	) angle-=	360.0 ;
	return angle ;
}


//--------------------------------------------------
//		�p�x���}180�x�ɕϊ�����
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
//		�p�x[deg]
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
//		�P�ʃx�N�g��
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
//		2�_�ԋ���
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
	if( ppdx2 < 1e-200 ) ppdx2=	0.0;			//�\���������l�̂��݂�r��
	if( ppdy2 < 1e-200 ) ppdy2=	0.0;
	double	dist=		sqrt( ppdx2 + ppdy2 );
	if( dist < 1e-200 ) dist=	0.0;			//�\���������l�̂��݂�r��
	return	dist;
}


//------------------------------------------------------------------------------
//		�����̕\��
//------------------------------------------------------------------------------
void	xfunc_print_text( HDC hdc , char* wchar , double tx , double ty , long height , long f_color , char* font_name , long back_color , double angle , long bold )
{
	if( height <= 0 ) return;

	// �����֘A
	HFONT		newfont ;
	HFONT		oldfont ;
	LOGFONT		fontatr ;
	//���O�t�H���g
	memset( &fontatr , 0 , sizeof(LOGFONT) );
	//����
	fontatr.lfHeight=			(long)(height+0.5);			// ����
	fontatr.lfWidth=			0;							// �v�����s�b�`
	fontatr.lfEscapement=		(long)( angle * 10.0 + 0.5 );			// Angle(1/10deg)
	fontatr.lfOrientation=		0;							// ENABLE	Durection of Each Character
	fontatr.lfWeight=			400;						//400 	BOLD( 700 )
	if( bold != 0 )fontatr.lfWeight=			700;
	fontatr.lfItalic=			0;							// Italic ( ==0:Not Italic !=0 Italic )
	fontatr.lfUnderline=		0;							// Under Line( ==0 No !=0 Under Line )
	fontatr.lfStrikeOut=		0;							// 0�ȊO�̂Ƃ��A�ł��������t���̃t�H���g�������܂��B
	fontatr.lfCharSet=			DEFAULT_CHARSET;				// �t�H���g�̕����Z�b�g�������܂��B
	fontatr.lfOutPrecision=		OUT_TT_PRECIS;				// �t�H���g�̏o�͐��x�������܂��B
	fontatr.lfOutPrecision=		OUT_DEFAULT_PRECIS;
	fontatr.lfClipPrecision=	CLIP_EMBEDDED;				// �N���b�s���O���x�������܂��B
	fontatr.lfClipPrecision=	CLIP_DEFAULT_PRECIS;
	fontatr.lfQuality=			DEFAULT_QUALITY;				// �t�H���g�̏o�͕i���������܂��B
	fontatr.lfPitchAndFamily=	DEFAULT_PITCH | FF_DONTCARE;	// �t�H���g�̃s�b�`�ƃt�@�~���������܂��B
	// �t�H���g
	if( font_name != NULL && font_name[0] != 0x00 ){
		strcpy( fontatr.lfFaceName , font_name ) ;
	}else{
		strcpy( fontatr.lfFaceName , "�l�r �o����" ) ;			// �t�H���g�̏��̖��������܂��B
	}
	SetROP2( hdc , 13 ) ;										// �`�惂�[�h or:15	xor:7  pset:13	reset:1
	//�w�i�F
	if( back_color >= 0 ){
		SetBkColor( hdc , back_color );
		SetBkMode( hdc , OPAQUE  );								//�h��Ԃ�
	}else{
		SetBkMode( hdc , TRANSPARENT );							//����
	}
	// �t�H���g�̐F
	SetTextColor( hdc , f_color ) ;
	//
	newfont=		CreateFontIndirect( &fontatr );								//�`�悷��t�H���g�̐���
	oldfont=		( HFONT )SelectObject( hdc , newfont );						// �t�H���g�̑I��
	int	bytelen=	(int)strlen( wchar );										//������o�C�g����
	TextOut( hdc , long( tx ) , long( ty ) , wchar , bytelen );	// �v�h���s�b�`�̕���
	SelectObject( hdc,oldfont );												// �t�H���g��߂�
	DeleteObject( newfont );													// �t�H���g�̍폜

}

//------------------------------------------------------------------------------
//		�����̗���(0.0-1.0)
//------------------------------------------------------------------------------
double	xfunc_rnd()
{
	double	rv=		(double)rand() / ( RAND_MAX + 1 );
	return rv;
}
//------------------------------------------------------------------------------
//����������
//------------------------------------------------------------------------------
void xfunc_randomize()
{
	unsigned int	seed=		timeGetTime();			//�V�X�e���������~���b�P�ʂŎ擾���܂��B�V�X�e�������� Windows ���N�����Ă���o�߂������Ԃł��B
	srand( seed );
}


//------------------------------------------------------------------------------
//		GDI�̊�{�I�Ȏg����
//------------------------------------------------------------------------------
void use_GDI( HWND hwnd )
{
	//�O���t�B�b�N���\�[�X�̐���
	HDC		hdc=		GetDC( hwnd );								//�E�B���h�E�̃n���h���Ɋ֘A�t������Ă���f�o�C�X�R���e�L�X�g�̃n���h�������o��
	HPEN	new_pen=	CreatePen( PS_SOLID , 1 , RGB(255,0,0) );	//�y�����쐬
	HBRUSH	new_brush=	CreateSolidBrush( RGB(0,255,0) );			//�u���V���쐬
	HPEN	old_pen=	(HPEN)SelectObject( hdc , new_pen );		//�f�o�C�X�R���e�L�X�g�Ɏg���y�����Z�b�g�B��ɃZ�b�g����Ă����y���̃n���h�����Ԃ�B
	HBRUSH	old_brush=	(HBRUSH)SelectObject( hdc , new_brush );	//�f�o�C�X�R���e�L�X�g�Ɏg���u���V���Z�b�g�B��ɃZ�b�g����Ă����u���V�̃n���h�����Ԃ�B
	//

	//���̍��W
	long	x1=		100;
	long	y1=		200;
	long	x2=		200;
	long	y2=		300;
	//
	MoveToEx( hdc , x1 , y1 , NULL );						//�n�_
	LineTo( hdc , x2 , y2 );								//�I�_
	//
	Ellipse( hdc , x1 , y1 , x2 , y2 );						//�~
	//
	SelectObject( hdc , GetStockObject( NULL_BRUSH ) );		//�h�ׂ�����
	Rectangle( hdc , x1 , y2 , x2 , y2 );					//�l�p�`
	//
	
	//�O���t�B�b�N���\�[�X�̔j��
	SelectObject( hdc , old_pen );							//�f�o�C�X�R���e�L�X�g�Ŏg���y�����ŏ��ɃZ�b�g����Ă����y���Ǝ�芷����B
	SelectObject( hdc , old_brush );						//�f�o�C�X�R���e�L�X�g�Ŏg���u���V���ŏ��ɃZ�b�g����Ă����u���V�Ǝ�芷����B
	DeleteObject( new_pen );								//������y����j������
	DeleteObject( new_brush );								//������u���V��j������
	ReleaseDC( hwnd , hdc );								//�E�B���h�E�̃n���h���Ɋ֘A�t������Ă���f�o�C�X�R���e�L�X�g�̃n���h�������R�ɂ���
}
