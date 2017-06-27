//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//			テキストに載せるサンプル
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//人をメッシュに分けて管理するクラス
//------------------------------------------------------------------------------
//
#include <Windows.h>
//
#include "x_mesh.h"
//
#pragma warning(disable : 4996)			//古いタイプの関数を使うと出て来る注意を止めさせる宣言文。

//------------------------------------------------------------------------------
//		コンストラクタ（生成時の処理）
//------------------------------------------------------------------------------
x_mesh::x_mesh()
{
	m_psn_mesh_size_x=		0;				//確保するメッシュ数（Ｘ方向）
	m_psn_mesh_size_y=		0;				//確保するメッシュ数（Ｙ方向）
	m_psn_mesh_ptr=			NULL;			//メッシュのリスト
	m_psn_mesh_jjii=		NULL;			//メッシュのリスト行の先頭アドレス
}

//------------------------------------------------------------------------------
//		デストラクタ（破棄する時の処理）
//------------------------------------------------------------------------------
x_mesh::~x_mesh()
{
	free( m_psn_mesh_ptr );					//メッシュのリストを破棄
	free( m_psn_mesh_jjii );				//メッシュのリスト行の先頭アドレスリストを破棄
}

//------------------------------------------------------------------------------
//		メッシュメモリ確保	//メッシュに入る最大人数で一気に作る。
//------------------------------------------------------------------------------
void x_mesh::creat_mesh( long size_x , long size_y )
{
	m_psn_mesh_size_x=		size_x;
	m_psn_mesh_size_y=		size_y;
	m_psn_mesh_ptr=			(S_MESH_IMAGE*)calloc( m_psn_mesh_size_x * m_psn_mesh_size_y , sizeof(S_MESH_IMAGE) );		//メッシュのリスト
	m_psn_mesh_jjii=		(S_MESH_IMAGE**)calloc( m_psn_mesh_size_y , sizeof(S_MESH_IMAGE*) );						//メッシュのリスト行の先頭アドレスリスト
	//行の先頭アドレスのリスト作成	//メッシュのリストに[jj][ii]の2次元の形でアクセスしたいので。
	for(long jj=0;jj<m_psn_mesh_size_y;jj++){
		m_psn_mesh_jjii[jj]=		&m_psn_mesh_ptr[m_psn_mesh_size_x * jj];
	}
}

//------------------------------------------------------------------------------
//		メッシュに登録する
//------------------------------------------------------------------------------
long x_mesh::into_mesh( long ii , long jj , long man_code )
{
	if( ii < 0  || ii >= m_psn_mesh_size_x ) return -1;			//範囲を超えている
	if( jj < 0  || jj >= m_psn_mesh_size_y ) return -1; 		//範囲を超えている
	S_MESH_IMAGE*	mesh_ptr=					&m_psn_mesh_jjii[jj][ii];				//メッシュ[jj][ii]の記録アドレス
	//
	long	heya_bangou=						-1;										//登録部屋番号
	//
	if( mesh_ptr->aki_max == 0 ){														//空き部屋がない
		heya_bangou=							mesh_ptr->kashi_max;						//リストの登録位置は住人リストの最後→
		mesh_ptr->kashi_heya[heya_bangou]=		man_code;									//住人番号を記録
		mesh_ptr->kashi_max++;																//住人リストの最後をずらす
		//
	}else{																				//空き部屋あり
		mesh_ptr->aki_max--;																//空き部屋数を減らす
		heya_bangou=							mesh_ptr->aki_heya[mesh_ptr->aki_max];		//リストの登録位置は最後に空いた部屋→
		mesh_ptr->kashi_heya[heya_bangou]=		man_code;									//住人リストの空き部屋に入居
	}
	return heya_bangou;																		//リストの登録位置を返す←
}

//------------------------------------------------------------------------------
//		メッシュから出る
//------------------------------------------------------------------------------
long x_mesh::out_mesh( long ii , long jj , long heya_bangou )
{
	if( ii < 0  || ii >= m_psn_mesh_size_x ) return -1;			//範囲を超えている
	if( jj < 0  || jj >= m_psn_mesh_size_y ) return -1; 		//範囲を超えている
	S_MESH_IMAGE*	mesh_ptr=					&m_psn_mesh_jjii[jj][ii];					//メッシュ[jj][ii]の記録アドレス
	//
	mesh_ptr->kashi_heya[heya_bangou]=			-1;											//住人退出
	mesh_ptr->aki_heya[mesh_ptr->aki_max]=		heya_bangou;								//空き部屋番号を記録
	mesh_ptr->aki_max++;																	//空き部屋数を増やす
	return -1;
}


