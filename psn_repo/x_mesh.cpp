//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//			�e�L�X�g�ɍڂ���T���v��
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�l�����b�V���ɕ����ĊǗ�����N���X
//------------------------------------------------------------------------------
//
#include <Windows.h>
//
#include	"x_functions.h"			//�l�̕W���֐�
#include	"x_mesh.h"				//���̃N���X
//
#pragma warning(disable : 4996)			//�Â��^�C�v�̊֐����g���Əo�ė��钍�ӂ��~�߂�����錾���B

//------------------------------------------------------------------------------
//		�R���X�g���N�^�i�������̏����j
//------------------------------------------------------------------------------
x_mesh::x_mesh()
{

	//���b�V���\��
	m_origin=				0.0;			//���b�V�����_���W[m]
	m_mesh_width=			2.0;			//���b�V����[m]
	//���b�V���L�^
	m_psn_mesh_size.x=		0;				//�m�ۂ��郁�b�V�����i�w�����j
	m_psn_mesh_size.y=		0;				//�m�ۂ��郁�b�V�����i�x�����j
	m_psn_mesh_ptr=			NULL;			//���b�V���̃��X�g
	m_psn_mesh_jjii=		NULL;			//���b�V���̃��X�g�s�̐擪�A�h���X
}

//------------------------------------------------------------------------------
//		�f�X�g���N�^�i�j�����鎞�̏����j
//------------------------------------------------------------------------------
x_mesh::~x_mesh()
{
	free( m_psn_mesh_ptr );					//���b�V���̃��X�g��j��
	free( m_psn_mesh_jjii );				//���b�V���̃��X�g�s�̐擪�A�h���X���X�g��j��
}

//------------------------------------------------------------------------------
//		���b�V���������m��	//���b�V���ɓ���ő�l���ň�C�ɍ��B
//------------------------------------------------------------------------------
void x_mesh::creat_mesh( long size_x , long size_y )
{
	m_psn_mesh_size.x=		size_x;
	m_psn_mesh_size.y=		size_y;
	m_psn_mesh_ptr=			(S_MESH_IMAGE*)calloc( m_psn_mesh_size.x * m_psn_mesh_size.y , sizeof(S_MESH_IMAGE) );		//���b�V���̃��X�g
	m_psn_mesh_jjii=		(S_MESH_IMAGE**)calloc( m_psn_mesh_size.y , sizeof(S_MESH_IMAGE*) );						//���b�V���̃��X�g�s�̐擪�A�h���X���X�g
	//�s�̐擪�A�h���X�̃��X�g�쐬	//���b�V���̃��X�g��[jj][ii]��2�����̌`�ŃA�N�Z�X�������̂ŁB
	for(long jj=0;jj<m_psn_mesh_size.y;jj++){
		m_psn_mesh_jjii[jj]=		&m_psn_mesh_ptr[m_psn_mesh_size.x * jj];
	}
}


//------------------------------------------------------------------------------
//		���W���烁�b�V���C���f�b�N�X���v�Z
//------------------------------------------------------------------------------
POINT x_mesh::real_to_mesh_index( dPOINT pos )
{
	POINT	ipos;
	ipos.x=		(long)( ( pos.x - m_origin.x ) / m_mesh_width );
	ipos.y=		(long)( ( pos.y - m_origin.y ) / m_mesh_width );
	return ipos;
}



//------------------------------------------------------------------------------
//		���b�V���ɓo�^����
//------------------------------------------------------------------------------
long x_mesh::into_mesh_real( dPOINT pos , long man_index )
{
	POINT	ipos=			real_to_mesh_index( pos );
	long	heya_bangou=	into_mesh( ipos.x , ipos.y , man_index );
	return heya_bangou;
}

//------------------------------------------------------------------------------
//		���b�V���ɓo�^����
//------------------------------------------------------------------------------
long x_mesh::into_mesh( long ii , long jj , long man_index )
{
	if( ii < 0  || ii >= m_psn_mesh_size.x ) return -1;			//�͈͂𒴂��Ă���
	if( jj < 0  || jj >= m_psn_mesh_size.y ) return -1; 		//�͈͂𒴂��Ă���
	S_MESH_IMAGE*	mesh_ptr=					&m_psn_mesh_jjii[jj][ii];				//���b�V��[jj][ii]�̋L�^�A�h���X
	//
	long	heya_bangou=						-1;										//�o�^�����ԍ�
	//
	if( mesh_ptr->aki_max == 0 ){														//�󂫕������Ȃ�

		if( mesh_ptr->kashi_max >= c_mesh_psn_size ) return -1;							//����

		heya_bangou=							mesh_ptr->kashi_max;						//���X�g�̓o�^�ʒu�͏Z�l���X�g�̍Ōと
		mesh_ptr->kashi_heya[heya_bangou]=		man_index;									//�Z�l�ԍ����L�^
		mesh_ptr->kashi_max++;																//�Z�l���X�g�̍Ō�����炷
		//
	}else{																				//�󂫕�������
		mesh_ptr->aki_max--;																//�󂫕����������炷
		heya_bangou=							mesh_ptr->aki_heya[mesh_ptr->aki_max];		//���X�g�̓o�^�ʒu�͍Ō�ɋ󂢂�������
		mesh_ptr->kashi_heya[heya_bangou]=		man_index;									//�Z�l���X�g�̋󂫕����ɓ���
	}
	return heya_bangou;																		//���X�g�̓o�^�ʒu��Ԃ���
}


//------------------------------------------------------------------------------
//		���b�V������o��
//------------------------------------------------------------------------------
long x_mesh::out_mesh_real( dPOINT pos , long heya_bangou )
{
	if( heya_bangou < 0 ) return -1;						//���X���Ȃ�
	POINT	ipos=			real_to_mesh_index( pos );
	return	out_mesh( ipos.x , ipos.y , heya_bangou );
}

//------------------------------------------------------------------------------
//		���b�V������o��
//------------------------------------------------------------------------------
long x_mesh::out_mesh( long ii , long jj , long heya_bangou )
{
	if( ii < 0  || ii >= m_psn_mesh_size.x ) return -1;			//�͈͂𒴂��Ă���
	if( jj < 0  || jj >= m_psn_mesh_size.y ) return -1; 		//�͈͂𒴂��Ă���
	S_MESH_IMAGE*	mesh_ptr=					&m_psn_mesh_jjii[jj][ii];					//���b�V��[jj][ii]�̋L�^�A�h���X
	//
	mesh_ptr->kashi_heya[heya_bangou]=			-1;											//�Z�l�ޏo
	mesh_ptr->aki_heya[mesh_ptr->aki_max]=		heya_bangou;								//�󂫕����ԍ����L�^
	mesh_ptr->aki_max++;																	//�󂫕������𑝂₷
	return -1;
}


