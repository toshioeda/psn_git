#pragma once
//------------------------------------------------------------------------------
//�l�𐶐������蓮�������肷�郁�C���̃N���X�錾
//------------------------------------------------------------------------------

#include	"x_mesh.h"				//���b�V������N���X

class x_simuB
{

public:
	x_simuB();
	~x_simuB();

//�ϐ�
public:
	//Windows�̃V�X�e���ϐ�
	HWND		m_hwnd;					//�\���̃E�C���h�E�n���h���iwindow�̎��ʕϐ��j
	//�ݒ�
	double		m_field_width;			//�t�B�[���h�̕�[m]
	double		m_field_length;			//�t�B�[���h�̒���[m](�l�͂��̃t�B�[���h�̍��E�ɕ������ς��ɏW�܂�܂�)
	double		m_disp_ratio;			//�\������1.0[m]�̃h�b�g��
	long		m_disp_offset_y;		//�x���W���S�ʒu
	long		m_man_max;				//�l�̑S�̐l��
	S_MAN*		m_man_ptr;				//�e�l�̑����i��Ől�����m�ۂ���j
	//�V�~�����[�V��������ϐ�
	long		m_counta;				//��������� �J�E���g
	double		mf_cal_interval;		//�v�Z�s�b�`����[sec]	�P�J�E���g�Ői�ގ���
	double		m_genzai_jikoku_sec;	//�v�Z��̎���[sec]
	//�O���t�B�b�N���\�[�X
	HPEN		m_color_pen[8];			//�y��
	HBRUSH		m_color_brush[8];		//�u���V
	//�\�����[�h
	long		m_disp_vector_sw;		//�����x�N�g���̕\����\�����[�h
	//�����Ԍv��(Window�n����̃~���b�J�E���^)[milli sec]
	unsigned int	m_simu_tyuudann_start;		//���f�J�n����
	unsigned int	m_simu_tyuudann_time;		//���f���Ă�������
	unsigned int	m_simu_start_time;			//�v�Z�J�n����


//�֐�
public:
void	x_simuB::disp_simu();																	//�`��
long	x_simuB::make_fuzzy_new_vector( long index );											//�t�@�W�B�[�ŉ���x�N�g�����v�Z
double	x_simuB::fuzzy_suiron(long my_index , long aite_index );								//�t�@�W�B���_�v�Z
double	x_simuB::xfunc_daikei_area(double height, double bottom_length , double top_height );	//��`�̖ʐ�
double	x_simuB::xfunc_fuzzy_strong( double val , double start_pos , double end_pos );			//�t�@�W�B���x�̌v�Z
void	x_simuB::real_to_view( double rx1 , double ry1 , double* vx1 , double* vy1 );			//�����W���r���[���W�ɕϊ�
void	x_simuB::view_to_real( double vx1 , double vy1 , double* rx1 , double* ry1 );			//�r���[�����ɕϊ�
long	x_simuB::disp_man_one( HDC hdc , long index );											//��l�`��
long	x_simuB::next_step_all();																//�S���̎��̃X�e�b�v���v�Z
long	x_simuB::next_step_one( long index );													//��l�̎��̃X�e�b�v���v�Z
long	x_simuB::check_next_position( long index );												//���̈ʒu�Ɉړ��\���ǂ�����T��





public:
x_mesh*		m_xmesh;

public:
void	x_simuB::set_mesh_all();																	//���b�V������


};