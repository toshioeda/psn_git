

//eda
//------------------------------------------------------------------------------
//		�W�c�s���V�~�����[�V�������K�T���v��
//����{�I�ȃx�[�X��Windows��WIN32�A�v���ł��BMicrosoft���쐬���āA�E�B�U�[�h�ɂ��Ă��镔���ł��B
//	��������������� //eda�@�`�@//eda end �Ŋ����Ă���܂��B
//------------------------------------------------------------------------------
//	psn.cpp			�E�B���h�E�Y���C���֐�
//	x_simuA.cpp		�V�~�����[�^�[�`
//	x_simuB.cpp		�V�~�����[�^�[�a�i���b�V�����p�j
//	x_head.h		���ʍ\���̐錾
//	x_function.h	���ʊ֐�
//------------------------------------------------------------------------------
//	x_mesh.cpp		���b�V������T���v��(x_simuB�Ŏg�p)
//	���̑��̓}�C�N���\�t�g���W���ŗe�Ղ���t�@�C��
//------------------------------------------------------------------------------
//eda end

//------------------------------------------------------------------------------
//		Windows�̕W���C���N���[�h�w�b�_�[�t�@�C��
//------------------------------------------------------------------------------
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN		// Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
#include <windows.h>			// Windows �w�b�_�[ �t�@�C��:

//------------------------------------------------------------------------------
//		Window�����p
//------------------------------------------------------------------------------
#define MAX_LOADSTRING 200
// �O���[�o���ϐ�:
	HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
	TCHAR sz_Title[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
	TCHAR sz_WindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��



//eda
//
#include <tchar.h>				//_tWinMain�����C�h�����񑀍�
		//	�}���`�o�C�g��������g�p�iunicode�����Z�b�g�͎g�p���Ȃ��j
		//		1�o�C�g�n�̕����������ʓ|�Ȃ̂ŁA�}���`�o�C�g�ɂ��Ă��܂����Aunicode���g����������I�ł��B
		//	�v���R���p�C���σw�b�_�[(strafx)�͎g�p���Ȃ��B
		//		�g���Ă����Q�͂قƂ�Ǐo�܂��񂪁A�g���K�v���Ȃ��̂ŁA���̃v���W�F�N�g����͍폜���Ă��܂��B
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//		�쐬���Ēǉ������C���N���[�h�t�@�C��
//------------------------------------------------------------------------------
#include	"x_functions.h"				//�l�̕W���֐���C�̕W���I�錾
#include	"x_simuA.h"					//�V�~�����[�^�[
#include	"x_simuB.h"					//�V�~�����[�^�[
#include	"psn_main.h"						//���̃��C���v���O����
#pragma warning(disable : 4996)			//VS2013�ȍ~�̈��S�Ȋ֐��ȍ~�ւ̍Ñ��x���𖳌��ɂ���

//------------------------------------------------------------------------------
//		�X�[�p�[�^�C�}�[
//------------------------------------------------------------------------------
#include	<Mmsystem.h>				//�X�[�p�[�^�C�}�[
#pragma comment( lib , "winmm.lib" )	//�X�[�p�[�^�C�}�[���C�u����
#define AA_TIMER			25000		//�X�[�p�[�^�C�}�[���b�Z�[�W�ԍ�(�K���ɏd���������l�ɐݒ肷��)
	long	main_Super_timer_res ;		//0:��~��	!=0:�N����
	long	main_Super_tmID ;			//�^�C�}�[�h�c
	long	main_Super_timer_prc ;		//���荞�݋��t���O( TRUE:���@FALSE:�֎~)
	HWND	parent_hWnd ;				//�e�E�B���h�E
	long	main_Super_interval ;		//���ݐݒ肳��Ă���C���^�[�o�����L�^����ϐ�
void	main_Super_SetTimer( long mili_second_intarval );		//�^�C�}�[�X�^�[�g
long	main_Super_KillTimer();
void	CALLBACK	main_Super_OnTimeProc( UINT uID , UINT uMsg , DWORD dwUser , DWORD dw1 , DWORD dw2 );

//-----------------------------------------------------------------------------
//		�O���[�o���ϐ�
//-----------------------------------------------------------------------------
HWND			g_main_hWnd;	//�E�B���h�E�n���h��
#ifdef  cdef_MESH_METHOD
x_simuB*		g_simu;			//���b�V�����p�V�~�����[�V����
#else
x_simuA*		g_simu;			//���b�V���Ȃ��V�~�����[�V����
#endif
//----------------------------------------------------------------------
//eda end


//----------------------------------------------------------------------

//----------------------------------------------------------------------
// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
//----------------------------------------------------------------------
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


//-------------------------------------------------------------------------------------------------------------
//		�E�B���h�E�Y�v���O�����̃��C�����[�`��
//-------------------------------------------------------------------------------------------------------------
int APIENTRY _tWinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPTSTR lpCmdLine , int nCmdShow ){


	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;

	// �O���[�o������������������Ă��܂��B
	strcpy( sz_Title , "person move" );							//�E�C���h�E�^�C�g���i�K���ł����j
	strcpy( sz_WindowClass , "PersonClass" );					//�E�C���h�E�N���X���́i�K���ł����j
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:					//�E�C���h�E�̐�����������
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}


//eda
#ifdef  cdef_MESH_METHOD
	g_simu=				new x_simuB();							//�V�~�����[�V�����N���X�𐶐�	//main
#else
	g_simu=				new x_simuA();							//�V�~�����[�V�����N���X�𐶐�	//main
#endif
	g_simu->m_hwnd=		g_main_hWnd;							//�E�C���h�E�n���h����`����
	//
	MoveWindow( g_simu->m_hwnd , 0 , 0 , (long)( g_simu->m_field_length * g_simu->m_disp_ratio ) , (long)( g_simu->m_field_length / 1.6 * g_simu->m_disp_ratio ) , TRUE );		//�ŏ��̃E�B���h�E
	//
//------------------------------------------------------------------------------
	//�^�C�}�[����
	main_Super_timer_res=		0 ;					//�N�����Ă��Ȃ�
	main_Super_tmID=			0 ;
	main_Super_timer_prc=		FALSE ;				//�^�C�}�[���f
	parent_hWnd=				g_simu->m_hwnd ;	//�e�E�B���h�E�n���h��
	main_Super_SetTimer( (long)( g_simu->mf_cal_interval * 1000 / 2  ) );			//�^�C�}�[�X�^�[�g		//�������x��2�{��
//------------------------------------------------------------------------------
	g_simu->m_simu_start_time=		timeGetTime();		//�v�Z���Ԍv���p
//eda end


//�n�r�Ƃ���������Ƃ���B�n�r�����b�Z�[�W�𐧌䂵�Ă����B
//------------------------------------------------------------------------------
	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
//------------------------------------------------------------------------------


//eda
	delete	g_simu;										//�V�~�����[�V�����N���X��j��
//eda end


	return (int) msg.wParam;
}



//------------------------------------------------------------------------------
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
//------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= sz_WindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//------------------------------------------------------------------------------
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst=		hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B
   hWnd=		CreateWindow(sz_WindowClass, sz_Title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd){
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

//eda
   g_main_hWnd=				hWnd;		//�E�B���h�E�n���h���̋L�^
//eda end

   return TRUE;
}

//------------------------------------------------------------------------------
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
//------------------------------------------------------------------------------
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

//eda
	POINT	now_point;
	now_point.x=		(short)LOWORD( lParam ) ;	//�J�[�\���̈ʒu�����o���Ēu��
	now_point.y=		(short)HIWORD( lParam ) ;
//eda end

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC paint_hdc;

	switch (message)
	{

		//�E�B���h�E���b�Z�[�W---------------------------------------------------------------------------------------------------
		case WM_COMMAND:{
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// �I�����ꂽ���j���[�̉��:
			{
				switch (wmId)
				{
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
			}
		}break;	//wm_command



//eda
//---------------------------------------------------
		//�X�[�p�[�^�C�}�[
//---------------------------------------------------
		case AA_TIMER:{								//�^�C�}�[���荞�݂����������痈��
			main_Super_timer_prc=	FALSE ;			//�^�C�}�[���f
			//
			g_simu->next_step_all();				//�P�X�e�b�v�O�i
			//
			g_simu->disp_simu();					//�l�̕`��
			//
			g_simu->m_counta++;						//�J�E���g�A�b�v
			g_simu->m_genzai_jikoku_sec=			g_simu->m_counta * g_simu->mf_cal_interval;		//���ݎ���[�b]
			//
			main_Super_timer_prc=	TRUE ;			//�^�C�}�[�ĊJ
			//
		}break;		//timer
//eda end

//---------------------------------------------------
//---------------------------------------------------
	case WM_KEYDOWN:{								//�L�[�������ꂽ���ɗ���
//eda
		long	key=	(long)wParam & 0xff ;
		if( key == 'W' ){							//�����x�N�g���\���A��\��
			if( g_simu->m_disp_vector_sw != 0 ){
				g_simu->m_disp_vector_sw=	0;
			}else{
				g_simu->m_disp_vector_sw=	1;
			}
			//
		}else if( key == 'S' ){						//��~�A�ĊJ
			if( main_Super_timer_prc != TRUE ){
				unsigned int tyuudann_end=			timeGetTime();		//�ĊJ����
				g_simu->m_simu_tyuudann_time+=		(tyuudann_end - g_simu->m_simu_tyuudann_start );
				main_Super_timer_prc=	TRUE;		//�^�C�}�[�ĊJ
			}else{
				main_Super_timer_prc=	FALSE;		//�^�C�}�[���f
				g_simu->m_simu_tyuudann_start=		timeGetTime();		//���f����
			}
			//
		}else if( key == 'R' ){						//���߂����蒼��
			main_Super_timer_prc=	FALSE;			//�^�C�}�[���f
			//
			long	old_vector_sw=			g_simu->m_disp_vector_sw;		//�����x�N�g���\���A��\�����L�^
			delete g_simu;												//�V�~�����[�V�����N���X�j��
//			xfunc_randomize();											//�����ݒ�(���g�p)
#ifdef cdef_MESH_METHOD
			g_simu=							new x_simuB();					//���b�V���t���V�~�����[�V�����N���X�𐶐�		//restart
#else
			g_simu=							new x_simuA();					//�V�~�����[�V�����N���X�𐶐�		//restart
#endif
			g_simu->m_hwnd=					g_main_hWnd;					//�E�C���h�E�n���h����`����
			g_simu->m_disp_vector_sw=		old_vector_sw;				//�����x�N�g���\���A��\���̕���
			//���T�C�Y�ύX���̏����Ɠ�������������
			RECT	rect;
			GetClientRect( g_main_hWnd , &rect );
			g_simu->m_disp_ratio=			rect.right / g_simu->m_field_length ;
			g_simu->m_disp_offset_y=		rect.bottom / 2 ;
			//
			g_simu->m_simu_start_time=		timeGetTime();		//�v�Z�J�n����
			g_simu->m_simu_tyuudann_time=	0;
			g_simu->m_counta=				0;
			main_Super_timer_prc=			TRUE;			//�^�C�}�[�ĊJ
			//
		}
		g_simu->disp_simu();						//�l�̕`��
//eda end
	}break;	//key-down


//---------------------------------------------------
//---------------------------------------------------
	case WM_ERASEBKGND:{					//�T�C�Y�ύX�Ȃǔw�i�����������K�v���o����ƌĂ΂��B
											//�������Ȃ�
	}break;


//---------------------------------------------------
//---------------------------------------------------
	case WM_SIZE:{					//�E�B���h�E�T�C�Y���ύX���ꂽ���ɌĂ΂��
//eda
		long	cx=		(short)LOWORD( lParam ) ;	//�T�C�Y�ύX���Window�̕�
		long	cy=		(short)HIWORD( lParam ) ;
		if( cx <= 0 || cy <= 0 ) break;
		if( g_simu == NULL ) break;
		g_simu->m_disp_ratio=		cx / g_simu->m_field_length ;
		g_simu->m_disp_offset_y=	cy / 2 ;

		g_simu->disp_simu();						//�l�̕`��
//eda end
	}break;


	case WM_PAINT:{				//�E�B���h�E�ɍĕ\�����K�v�ȂƂ��Ă΂��
		paint_hdc=		BeginPaint(hWnd, &ps);
		// TODO: �`��R�[�h�������ɒǉ����Ă�������...


		//end of todo
		EndPaint(hWnd, &ps);
	}break;	//paint

//---------------------------------------------------
		//�}�E�X�A�b�v�_�E��
//---------------------------------------------------
	//case WM_LBUTTONDOWN:{
	//}break;		//L-down

	//case WM_LBUTTONUP:{
	//}break;	//L-up

	//case WM_RBUTTONDOWN:{		//�_�E��
	//}break;	//R-down

	//case WM_MOUSEMOVE:{
	//}break;		//move


//---------------------------------------------------
	case WM_DESTROY:
//eda
		//�X�[�p�[�^�C�}�[�I��
		main_Super_KillTimer();
//eda end

		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}





//eda
//------------------------------------------------------------------------------
//		�^�C�}�[�X�^�[�g
//------------------------------------------------------------------------------
void	main_Super_SetTimer( long mili_second_intarval )
{
	if( main_Super_timer_res != 0 ) return ;										//���ɋN�����Ȃ牽�����Ȃ�
	TIMECAPS	Super_tcap ;														//�^�C�}�[�̐��x
	long	mres=		timeGetDevCaps( &Super_tcap , sizeof(   TIMECAPS ) ) ;		//�^�C�}�[�̐��x���擾
	if( mres == TIMERR_NOERROR ){													//TIMERR_NOERROR if successful or TIMERR_STRUCT if it fails to return
		main_Super_timer_res=			min( max( Super_tcap.wPeriodMin , 1 ) , Super_tcap.wPeriodMax ) ;
		//�^�C�}�[���\�[�X�̊m��
		long	bres=					timeBeginPeriod( main_Super_timer_res ) ;
		if( bres == TIMERR_NOERROR ){
			//�C�x���g�̐ݒ�ƊJ�n
			long	timer_intarval=		max( mili_second_intarval , main_Super_timer_res ) ;		//�o�b�̍ŏ��C���^�[�o���Ǝw�肳�ꂽ�C���^�[�o���̂����A�傫�����Ŏw�肷��B
			main_Super_tmID=			timeSetEvent( timer_intarval , main_Super_timer_res , (LPTIMECALLBACK)main_Super_OnTimeProc , (DWORD)NULL , TIME_PERIODIC ) ;
			main_Super_timer_prc=		TRUE ;		//�^�C�}�[�ĊJ
		}
	}
	main_Super_interval=	mili_second_intarval;
}
//------------------------------------------------------------------------------
//		�^�C�}�[��~
//------------------------------------------------------------------------------
long	main_Super_KillTimer()
{
	main_Super_timer_prc=		FALSE ;		//�^�C�}�[���f
	
	timeKillEvent( main_Super_tmID ) ;
	timeEndPeriod( main_Super_timer_res ) ;

	main_Super_timer_res=		0 ;

	return	main_Super_interval;			//�Â��C���^�[�o����Ԃ�

}

//------------------------------------------------------------------------------
//		�^�C�}�[����Ă΂��֐�
//------------------------------------------------------------------------------
//�R�[���o�b�N�֐�
//	uID		�^�C�}���ʎq�B
//	uMsg	���b�Z�[�W�B�\�񂳂�Ă��邽�ߎg���܂���B
//	dwUser	�����Ă����C�ӂ̃f�[�^�B
//	dw1		�V�X�e���\��ς݁B
//	dw2		�V�X�e���\��ς݁B
//------------------------------------------------------------------------------
void	CALLBACK	main_Super_OnTimeProc( UINT uID , UINT uMsg , DWORD dwUser , DWORD dw1 , DWORD dw2 )
{
	if( main_Super_timer_prc == TRUE ){			//�^�C�}�[���s���Ȃ��
		main_Super_timer_prc=	FALSE ;			//�^�C�}�[���f
		::PostMessage( parent_hWnd , AA_TIMER , 0 , 0 ) ;			//�^�C�}�[�R�[�h���|�X�g����B
	}
}
//------------------------------------------------------------------------------


//eda end