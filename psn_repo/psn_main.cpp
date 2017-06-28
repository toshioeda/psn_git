

//eda
//------------------------------------------------------------------------------
//		集団行動シミュレーション実習サンプル
//●基本的なベースはWindowsのWIN32アプリです。Microsoftが作成して、ウィザードにしてある部分です。
//	手を加えた部分は //eda　～　//eda end で括ってあります。
//------------------------------------------------------------------------------
//	psn.cpp			ウィンドウズメイン関数
//	x_simuA.cpp		シミュレーターＡ
//	x_simuB.cpp		シミュレーターＢ（メッシュ利用）
//	x_head.h		共通構造体宣言
//	x_function.h	共通関数
//------------------------------------------------------------------------------
//	x_mesh.cpp		メッシュ制御サンプル(x_simuBで使用)
//	その他はマイクロソフトが標準で容易するファイル
//------------------------------------------------------------------------------
//eda end

//------------------------------------------------------------------------------
//		Windowsの標準インクルードヘッダーファイル
//------------------------------------------------------------------------------
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから使用されていない部分を除外します。
#include <windows.h>			// Windows ヘッダー ファイル:

//------------------------------------------------------------------------------
//		Window生成用
//------------------------------------------------------------------------------
#define MAX_LOADSTRING 200
// グローバル変数:
	HINSTANCE hInst;								// 現在のインターフェイス
	TCHAR sz_Title[MAX_LOADSTRING];					// タイトル バーのテキスト
	TCHAR sz_WindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名



//eda
//
#include <tchar.h>				//_tWinMain等ワイド文字列操作
		//	マルチバイト文字列を使用（unicode文字セットは使用しない）
		//		1バイト系の文字処理が面倒なので、マルチバイトにしていますが、unicodeを使う方が現代的です。
		//	プリコンパイル済ヘッダー(strafx)は使用しない。
		//		使っても実害はほとんど出ませんが、使う必要もないので、このプロジェクトからは削除しています。
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//		作成して追加したインクルードファイル
//------------------------------------------------------------------------------
#include	"x_functions.h"				//個人の標準関数とCの標準的宣言
#include	"x_simuA.h"					//シミュレーター
#include	"x_simuB.h"					//シミュレーター
#include	"psn_main.h"						//このメインプログラム
#pragma warning(disable : 4996)			//VS2013以降の安全な関数以降への催促警告を無効にする

//------------------------------------------------------------------------------
//		スーパータイマー
//------------------------------------------------------------------------------
#include	<Mmsystem.h>				//スーパータイマー
#pragma comment( lib , "winmm.lib" )	//スーパータイマーライブラリ
#define AA_TIMER			25000		//スーパータイマーメッセージ番号(適当に重複し無い様に設定する)
	long	main_Super_timer_res ;		//0:停止中	!=0:起動中
	long	main_Super_tmID ;			//タイマーＩＤ
	long	main_Super_timer_prc ;		//割り込み許可フラグ( TRUE:許可　FALSE:禁止)
	HWND	parent_hWnd ;				//親ウィンドウ
	long	main_Super_interval ;		//現在設定されているインターバルを記録する変数
void	main_Super_SetTimer( long mili_second_intarval );		//タイマースタート
long	main_Super_KillTimer();
void	CALLBACK	main_Super_OnTimeProc( UINT uID , UINT uMsg , DWORD dwUser , DWORD dw1 , DWORD dw2 );

//-----------------------------------------------------------------------------
//		グローバル変数
//-----------------------------------------------------------------------------
HWND			g_main_hWnd;	//ウィンドウハンドル
#ifdef  cdef_MESH_METHOD
x_simuB*		g_simu;			//メッシュ利用シミュレーション
#else
x_simuA*		g_simu;			//メッシュなしシミュレーション
#endif
//----------------------------------------------------------------------
//eda end


//----------------------------------------------------------------------

//----------------------------------------------------------------------
// このコード モジュールに含まれる関数の宣言を転送します:
//----------------------------------------------------------------------
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


//-------------------------------------------------------------------------------------------------------------
//		ウィンドウズプログラムのメインルーチン
//-------------------------------------------------------------------------------------------------------------
int APIENTRY _tWinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPTSTR lpCmdLine , int nCmdShow ){


	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ここにコードを挿入してください。
	MSG msg;

	// グローバル文字列を初期化しています。
	strcpy( sz_Title , "person move" );							//ウインドウタイトル（適当でいい）
	strcpy( sz_WindowClass , "PersonClass" );					//ウインドウクラス名称（適当でいい）
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:					//ウインドウの生成も此処で
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}


//eda
#ifdef  cdef_MESH_METHOD
	g_simu=				new x_simuB();							//シミュレーションクラスを生成	//main
#else
	g_simu=				new x_simuA();							//シミュレーションクラスを生成	//main
#endif
	g_simu->m_hwnd=		g_main_hWnd;							//ウインドウハンドルを伝える
	//
	MoveWindow( g_simu->m_hwnd , 0 , 0 , (long)( g_simu->m_field_length * g_simu->m_disp_ratio ) , (long)( g_simu->m_field_length / 1.6 * g_simu->m_disp_ratio ) , TRUE );		//最初のウィンドウ
	//
//------------------------------------------------------------------------------
	//タイマー制御
	main_Super_timer_res=		0 ;					//起動していない
	main_Super_tmID=			0 ;
	main_Super_timer_prc=		FALSE ;				//タイマー中断
	parent_hWnd=				g_simu->m_hwnd ;	//親ウィンドウハンドル
	main_Super_SetTimer( (long)( g_simu->mf_cal_interval * 1000 / 2  ) );			//タイマースタート		//制限速度＝2倍速
//------------------------------------------------------------------------------
	g_simu->m_simu_start_time=		timeGetTime();		//計算時間計測用
//eda end


//ＯＳとやり取りをするところ。ＯＳがメッセージを制御してくれる。
//------------------------------------------------------------------------------
	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
//------------------------------------------------------------------------------


//eda
	delete	g_simu;										//シミュレーションクラスを破棄
//eda end


	return (int) msg.wParam;
}



//------------------------------------------------------------------------------
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
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
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst=		hInstance; // グローバル変数にインスタンス処理を格納します。
   hWnd=		CreateWindow(sz_WindowClass, sz_Title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd){
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

//eda
   g_main_hWnd=				hWnd;		//ウィンドウハンドルの記録
//eda end

   return TRUE;
}

//------------------------------------------------------------------------------
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//------------------------------------------------------------------------------
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

//eda
	POINT	now_point;
	now_point.x=		(short)LOWORD( lParam ) ;	//カーソルの位置を取り出して置く
	now_point.y=		(short)HIWORD( lParam ) ;
//eda end

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC paint_hdc;

	switch (message)
	{

		//ウィンドウメッセージ---------------------------------------------------------------------------------------------------
		case WM_COMMAND:{
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// 選択されたメニューの解析:
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
		//スーパータイマー
//---------------------------------------------------
		case AA_TIMER:{								//タイマー割り込みが発生したら来る
			main_Super_timer_prc=	FALSE ;			//タイマー中断
			//
			g_simu->next_step_all();				//１ステップ前進
			//
			g_simu->disp_simu();					//人の描画
			//
			g_simu->m_counta++;						//カウントアップ
			g_simu->m_genzai_jikoku_sec=			g_simu->m_counta * g_simu->mf_cal_interval;		//現在時刻[秒]
			//
			main_Super_timer_prc=	TRUE ;			//タイマー再開
			//
		}break;		//timer
//eda end

//---------------------------------------------------
//---------------------------------------------------
	case WM_KEYDOWN:{								//キーが押された時に来る
//eda
		long	key=	(long)wParam & 0xff ;
		if( key == 'W' ){							//方向ベクトル表示、非表示
			if( g_simu->m_disp_vector_sw != 0 ){
				g_simu->m_disp_vector_sw=	0;
			}else{
				g_simu->m_disp_vector_sw=	1;
			}
			//
		}else if( key == 'S' ){						//停止、再開
			if( main_Super_timer_prc != TRUE ){
				unsigned int tyuudann_end=			timeGetTime();		//再開時刻
				g_simu->m_simu_tyuudann_time+=		(tyuudann_end - g_simu->m_simu_tyuudann_start );
				main_Super_timer_prc=	TRUE;		//タイマー再開
			}else{
				main_Super_timer_prc=	FALSE;		//タイマー中断
				g_simu->m_simu_tyuudann_start=		timeGetTime();		//中断時刻
			}
			//
		}else if( key == 'R' ){						//初めからやり直し
			main_Super_timer_prc=	FALSE;			//タイマー中断
			//
			long	old_vector_sw=			g_simu->m_disp_vector_sw;		//方向ベクトル表示、非表示を記録
			delete g_simu;												//シミュレーションクラス破棄
//			xfunc_randomize();											//乱数設定(未使用)
#ifdef cdef_MESH_METHOD
			g_simu=							new x_simuB();					//メッシュ付きシミュレーションクラスを生成		//restart
#else
			g_simu=							new x_simuA();					//シミュレーションクラスを生成		//restart
#endif
			g_simu->m_hwnd=					g_main_hWnd;					//ウインドウハンドルを伝える
			g_simu->m_disp_vector_sw=		old_vector_sw;				//方向ベクトル表示、非表示の復活
			//窓サイズ変更時の処理と同じ処理をする
			RECT	rect;
			GetClientRect( g_main_hWnd , &rect );
			g_simu->m_disp_ratio=			rect.right / g_simu->m_field_length ;
			g_simu->m_disp_offset_y=		rect.bottom / 2 ;
			//
			g_simu->m_simu_start_time=		timeGetTime();		//計算開始時刻
			g_simu->m_simu_tyuudann_time=	0;
			g_simu->m_counta=				0;
			main_Super_timer_prc=			TRUE;			//タイマー再開
			//
		}
		g_simu->disp_simu();						//人の描画
//eda end
	}break;	//key-down


//---------------------------------------------------
//---------------------------------------------------
	case WM_ERASEBKGND:{					//サイズ変更など背景を書き直す必要が出来ると呼ばれる。
											//何もしない
	}break;


//---------------------------------------------------
//---------------------------------------------------
	case WM_SIZE:{					//ウィンドウサイズが変更された時に呼ばれる
//eda
		long	cx=		(short)LOWORD( lParam ) ;	//サイズ変更後のWindowの幅
		long	cy=		(short)HIWORD( lParam ) ;
		if( cx <= 0 || cy <= 0 ) break;
		if( g_simu == NULL ) break;
		g_simu->m_disp_ratio=		cx / g_simu->m_field_length ;
		g_simu->m_disp_offset_y=	cy / 2 ;

		g_simu->disp_simu();						//人の描画
//eda end
	}break;


	case WM_PAINT:{				//ウィンドウに再表示が必要なとき呼ばれる
		paint_hdc=		BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...


		//end of todo
		EndPaint(hWnd, &ps);
	}break;	//paint

//---------------------------------------------------
		//マウスアップダウン
//---------------------------------------------------
	//case WM_LBUTTONDOWN:{
	//}break;		//L-down

	//case WM_LBUTTONUP:{
	//}break;	//L-up

	//case WM_RBUTTONDOWN:{		//ダウン
	//}break;	//R-down

	//case WM_MOUSEMOVE:{
	//}break;		//move


//---------------------------------------------------
	case WM_DESTROY:
//eda
		//スーパータイマー終了
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
//		タイマースタート
//------------------------------------------------------------------------------
void	main_Super_SetTimer( long mili_second_intarval )
{
	if( main_Super_timer_res != 0 ) return ;										//既に起動中なら何もしない
	TIMECAPS	Super_tcap ;														//タイマーの精度
	long	mres=		timeGetDevCaps( &Super_tcap , sizeof(   TIMECAPS ) ) ;		//タイマーの精度を取得
	if( mres == TIMERR_NOERROR ){													//TIMERR_NOERROR if successful or TIMERR_STRUCT if it fails to return
		main_Super_timer_res=			min( max( Super_tcap.wPeriodMin , 1 ) , Super_tcap.wPeriodMax ) ;
		//タイマーリソースの確保
		long	bres=					timeBeginPeriod( main_Super_timer_res ) ;
		if( bres == TIMERR_NOERROR ){
			//イベントの設定と開始
			long	timer_intarval=		max( mili_second_intarval , main_Super_timer_res ) ;		//ＰＣの最小インターバルと指定されたインターバルのうち、大きい方で指定する。
			main_Super_tmID=			timeSetEvent( timer_intarval , main_Super_timer_res , (LPTIMECALLBACK)main_Super_OnTimeProc , (DWORD)NULL , TIME_PERIODIC ) ;
			main_Super_timer_prc=		TRUE ;		//タイマー再開
		}
	}
	main_Super_interval=	mili_second_intarval;
}
//------------------------------------------------------------------------------
//		タイマー停止
//------------------------------------------------------------------------------
long	main_Super_KillTimer()
{
	main_Super_timer_prc=		FALSE ;		//タイマー中断
	
	timeKillEvent( main_Super_tmID ) ;
	timeEndPeriod( main_Super_timer_res ) ;

	main_Super_timer_res=		0 ;

	return	main_Super_interval;			//古いインターバルを返す

}

//------------------------------------------------------------------------------
//		タイマーから呼ばれる関数
//------------------------------------------------------------------------------
//コールバック関数
//	uID		タイマ識別子。
//	uMsg	メッセージ。予約されているため使えません。
//	dwUser	送られてきた任意のデータ。
//	dw1		システム予約済み。
//	dw2		システム予約済み。
//------------------------------------------------------------------------------
void	CALLBACK	main_Super_OnTimeProc( UINT uID , UINT uMsg , DWORD dwUser , DWORD dw1 , DWORD dw2 )
{
	if( main_Super_timer_prc == TRUE ){			//タイマー実行中ならば
		main_Super_timer_prc=	FALSE ;			//タイマー中断
		::PostMessage( parent_hWnd , AA_TIMER , 0 , 0 ) ;			//タイマーコードをポストする。
	}
}
//------------------------------------------------------------------------------


//eda end