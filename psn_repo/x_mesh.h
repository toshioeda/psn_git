#pragma once
//------------------------------------------------------------------------------
//�l�����b�V���ɕ����ĊǗ�����N���X�̐錾
//------------------------------------------------------------------------------
//
//�錾
#define		c_mesh_psn_size		40				//���b�V���T�C�Y�@2m�~2m�@�@����@10�l/�u
typedef struct s_mesh_image{
	long	aki_heya[c_mesh_psn_size];			//�󂫕������X�g
	long	aki_max;							//�󂫎���
	long	kashi_heya[c_mesh_psn_size];		//�݂��������X�g
	long	kashi_max;							//�݂������i�󂫕������܂ށj
}S_MESH_IMAGE;

//�N���X�錾
class x_mesh
{
public:
	x_mesh();
	~x_mesh();
//�ϐ�
public:
	//���b�V���\��
	dPOINT	m_origin;		//���b�V�����_���W[m]
	double	m_mesh_width;	//���b�V����[m]


//���b�V���L�^�ϐ�
POINT			m_psn_mesh_size;				//�m�ۂ��郁�b�V�����i�w,�x�����j
S_MESH_IMAGE*	m_psn_mesh_ptr;					//���b�V���̃��X�g
S_MESH_IMAGE**	m_psn_mesh_jjii;				//���b�V���̃��X�g�s�̐擪�A�h���X


//�֐�
public:
void x_mesh::creat_mesh( long size_x , long size_y );			//�������̊m��
POINT x_mesh::real_to_mesh_index( dPOINT pos );					//���W���烁�b�V���C���f�b�N�X���v�Z
long x_mesh::into_mesh_real( dPOINT pos , long man_index );		//���W�œo�^
long x_mesh::into_mesh( long ii , long jj , long man_index );	//�o�^
long x_mesh::out_mesh_real(  dPOINT pos , long heya_bangou );	//�폜
long x_mesh::out_mesh( long ii , long jj , long heya_bangou );	//�폜


};