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
#include "x_mesh.h"
//
#pragma warning(disable : 4996)			//�Â��^�C�v�̊֐����g���Əo�ė��钍�ӂ��~�߂�����錾���B

//------------------------------------------------------------------------------
//		�R���X�g���N�^�i�������̏����j
//------------------------------------------------------------------------------
x_mesh::x_mesh()
{
	m_psn_mesh_size_x=		0;				//�m�ۂ��郁�b�V�����i�w�����j
	m_psn_mesh_size_y=		0;				//�m�ۂ��郁�b�V�����i�x�����j
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
	m_psn_mesh_size_x=		size_x;
	m_psn_mesh_size_y=		size_y;
	m_psn_mesh_ptr=			(S_MESH_IMAGE*)calloc( m_psn_mesh_size_x * m_psn_mesh_size_y , sizeof(S_MESH_IMAGE) );		//���b�V���̃��X�g
	m_psn_mesh_jjii=		(S_MESH_IMAGE**)calloc( m_psn_mesh_size_y , sizeof(S_MESH_IMAGE*) );						//���b�V���̃��X�g�s�̐擪�A�h���X���X�g
	//�s�̐擪�A�h���X�̃��X�g�쐬	//���b�V���̃��X�g��[jj][ii]��2�����̌`�ŃA�N�Z�X�������̂ŁB
	for(long jj=0;jj<m_psn_mesh_size_y;jj++){
		m_psn_mesh_jjii[jj]=		&m_psn_mesh_ptr[m_psn_mesh_size_x * jj];
	}
}

//------------------------------------------------------------------------------
//		���b�V���ɓo�^����
//------------------------------------------------------------------------------
long x_mesh::into_mesh( long ii , long jj , long man_code )
{
	if( ii < 0  || ii >= m_psn_mesh_size_x ) return -1;			//�͈͂𒴂��Ă���
	if( jj < 0  || jj >= m_psn_mesh_size_y ) return -1; 		//�͈͂𒴂��Ă���
	S_MESH_IMAGE*	mesh_ptr=					&m_psn_mesh_jjii[jj][ii];				//���b�V��[jj][ii]�̋L�^�A�h���X
	//
	long	heya_bangou=						-1;										//�o�^�����ԍ�
	//
	if( mesh_ptr->aki_max == 0 ){														//�󂫕������Ȃ�
		heya_bangou=							mesh_ptr->kashi_max;						//���X�g�̓o�^�ʒu�͏Z�l���X�g�̍Ōと
		mesh_ptr->kashi_heya[heya_bangou]=		man_code;									//�Z�l�ԍ����L�^
		mesh_ptr->kashi_max++;																//�Z�l���X�g�̍Ō�����炷
		//
	}else{																				//�󂫕�������
		mesh_ptr->aki_max--;																//�󂫕����������炷
		heya_bangou=							mesh_ptr->aki_heya[mesh_ptr->aki_max];		//���X�g�̓o�^�ʒu�͍Ō�ɋ󂢂�������
		mesh_ptr->kashi_heya[heya_bangou]=		man_code;									//�Z�l���X�g�̋󂫕����ɓ���
	}
	return heya_bangou;																		//���X�g�̓o�^�ʒu��Ԃ���
}

//------------------------------------------------------------------------------
//		���b�V������o��
//------------------------------------------------------------------------------
long x_mesh::out_mesh( long ii , long jj , long heya_bangou )
{
	if( ii < 0  || ii >= m_psn_mesh_size_x ) return -1;			//�͈͂𒴂��Ă���
	if( jj < 0  || jj >= m_psn_mesh_size_y ) return -1; 		//�͈͂𒴂��Ă���
	S_MESH_IMAGE*	mesh_ptr=					&m_psn_mesh_jjii[jj][ii];					//���b�V��[jj][ii]�̋L�^�A�h���X
	//
	mesh_ptr->kashi_heya[heya_bangou]=			-1;											//�Z�l�ޏo
	mesh_ptr->aki_heya[mesh_ptr->aki_max]=		heya_bangou;								//�󂫕����ԍ����L�^
	mesh_ptr->aki_max++;																	//�󂫕������𑝂₷
	return -1;
}


