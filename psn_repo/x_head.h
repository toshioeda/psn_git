#pragma once


//------------------------------------------------------------------------------
//	�S�̂ɋ��ʂ���錾
//------------------------------------------------------------------------------
#define		u_PAI	3.14159265358979


//------------------------------------------------------------------------------
//	�_�u�����x�̍��W�̍\����
//------------------------------------------------------------------------------
typedef struct	s_dPOINT{
	BOOL operator == ( s_dPOINT ) ;			//equequ
	s_dPOINT operator = ( double );			//equdouble
	s_dPOINT operator - ( s_dPOINT );		//dpointmainasudpoint
	s_dPOINT operator + ( s_dPOINT );		//pluspoint
	s_dPOINT operator * ( double );			//times
	s_dPOINT operator * ( s_dPOINT );		//timestimes
	double	x ;
	double	y ;
} dPOINT ;

inline	BOOL dPOINT::operator == ( dPOINT dp )			//equequ
{
	double	lv	= 0.000000001 ;
	if( fabs(x - dp.x )<lv && fabs(y - dp.y )<lv ){
		return TRUE ;
	}
	return FALSE ;
};

inline	dPOINT dPOINT::operator = ( double vv )		//equdouble
{
	dPOINT	ansd ;
	x=	ansd.x=		vv ; 
	y=	ansd.y=		vv ;
	return ansd ;
};

inline	dPOINT dPOINT::operator - ( dPOINT dp )		//dpointmainasudpoint
{
	dPOINT	ansd ;
	ansd.x=			x - dp.x ; 
	ansd.y=			y - dp.y ;
	return ansd ;
};

inline	dPOINT dPOINT::operator + ( dPOINT dp )	//pluspoint
{
	dPOINT	ansd ;
	ansd.x=			x + dp.x ; 
	ansd.y=			y + dp.y ;
	return ansd ;
};

inline	dPOINT dPOINT::operator * ( double vv )		//times
{
	dPOINT	ansd ;
	ansd.x=		x * vv; 
	ansd.y=		y * vv;
	return ansd;
};

inline	dPOINT dPOINT::operator * ( dPOINT dp )		//timestimes
{
	dPOINT	ansd ;
	ansd.x=			x * dp.x ; 
	ansd.y=			y * dp.y ;
	return ansd ;
};


//------------------------------------------------------------------------------
//	�l�̑����̍\����
//------------------------------------------------------------------------------
typedef struct s_man{
//�l�̐ݒ荀��
	//���\
	double	body_haba;				//�l�̔��a[m]
	double	m_walk_speed;			//���s���x[m/s]
	double	m_zenpou_angle;			//�O���ɂ���Ɣ��f�ł��鎋��p
	//�t�@�W�B�[�@�����o�[�V�b�v
	//�߂�
	double	near_start_pos;			//�߂��Ɗ����n�߂鋗��[m]�i���S�ԋ�������A�̔��a���������A�󌄋����j
	double	near_end_pos;			//��΋߂��Ɗ����鋗��[m]
	//����
	double	far_start_pos;			//�����Ɗ����n�߂鋗��[m]
	double	far_end_pos;			//��Ή����Ɗ����鋗��[m]
	//�������Ă���
	double	kuru_start_pos;			//�u�������Ă���v�Ɗ����͂��߂鑊��̊�̊p�x[deg](0-90)
	double	kuru_end_pos;			//��΁u�������Ă���v�Ɗ����鑊��̊�̊p�x[deg](0-90) < start
	//��������
	double	konai_start_pos;		//�u��������v�Ɗ����͂��߂�p�x[deg](0-180)
	double	konai_end_pos;			//��΁u��������v�Ɗ�����p�x[deg](0-180) > start
	//�t�@�W�B�[�@���x
	double	near_kuru_base;			//�߂��E�߂Â��@�@�ꍇ�̐ݒ苭�x[0-1]
	double	near_konai_base;		//�߂��E��������@�ꍇ�̐ݒ苭�x[0-1]
	double	fars_kuru_base;			//�����E�߂Â��@�@�ꍇ�̐ݒ苭�x[0-1]
	double	fars_konai_base;		//�����E��������@�ꍇ�̐ݒ苭�x[0-1]
//�ʒu���
	dPOINT	m_now_pos;				//���݈ʒu
	dPOINT	m_now_target_pos;		//�ړI�ʒu
//�v�Z����
	dPOINT	m_now_target_dir;		//�ړI�ʒu�̕���(�P�ʃx�N�g��)
	double	m_now_target_angle;		//�ړI�����̊p�x[deg]
	dPOINT	m_next_dir;				//���i���j�̐i�s����(�P�ʃx�N�g��)
	dPOINT	m_sum_fuzzy_vect;		//�P�΂P�Ŕ����������ӎ����x�̎��͑S���̍��v�x�N�g���i���̌��ߕ��͎��R�j
//���̑�
	long	m_color_code;			//�F�����p�R�[�h(1:�� 2:�� 3:�� 4:�� 5:���F 6:���F 7:���j
	long	m_brush_code;			//�F�����p�R�[�h(1:�� 2:�� 3:�� 4:�� 5:���F 6:���F 7:���j
	//
}S_MAN;


