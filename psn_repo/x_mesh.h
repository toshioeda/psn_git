#pragma once
//------------------------------------------------------------------------------
//人をメッシュに分けて管理するクラスの宣言
//------------------------------------------------------------------------------
//
//宣言
#define		c_mesh_psn_size		40				//メッシュサイズ　2m×2m　　定員　10人/㎡
typedef struct s_mesh_image{
	long	aki_heya[c_mesh_psn_size];			//空き部屋リスト
	long	aki_max;							//空き室数
	long	kashi_heya[c_mesh_psn_size];		//貸し部屋リスト
	long	kashi_max;							//貸し室数（空き部屋を含む）
}S_MESH_IMAGE;

//クラス宣言
class x_mesh
{
public:
	x_mesh();
	~x_mesh();
//変数
public:
//メッシュ記録変数
long			m_psn_mesh_size_x;				//確保するメッシュ数（Ｘ方向）
long			m_psn_mesh_size_y;				//確保するメッシュ数（Ｙ方向）
S_MESH_IMAGE*	m_psn_mesh_ptr;					//メッシュのリスト
S_MESH_IMAGE**	m_psn_mesh_jjii;				//メッシュのリスト行の先頭アドレス


//関数
public:
void x_mesh::creat_mesh( long size_x , long size_y );			//メモリの確保
long x_mesh::into_mesh( long ii , long jj , long man_code );	//登録
long x_mesh::out_mesh( long ii , long jj , long heya_bangou );	//削除


};