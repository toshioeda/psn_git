

//------------------------------------------------------------------------------
//�l�𐶐������蓮�������肷�郁�C���֐��ł��B
//------------------------------------------------------------------------------
//�� �l�i��ς��Ċώ@����̂ɓs���̗ǂ��ϐ��ł��B
//�� �l�i�ݒ荀�ڂł��B
//
#include <Windows.h>
//
#include	"x_functions.h"			//�l�̕W���֐�
#include	"x_simuA.h"
//
#pragma warning(disable : 4996)			//�Â��^�C�v�̊֐����g���Əo�ė��钍�ӂ��~�߂�����錾���B


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//		�R���X�g���N�^�i�������̏����j
//------------------------------------------------------------------------------
x_simuA::x_simuA()
{

	//�ݒ肷�����
	m_field_width=			chead_FIELD_WIDTH;			//��	//�����z�u��[m]
	m_field_length=			chead_FIELD_LENGTH;			//��	//�t�B�[���h�̒���[m]	(�l�͂��̃t�B�[���h�̍��E�ɕ������ς��ɏW�܂�܂�)
	m_man_max=				chead_NINZUU;				//��	//�l�̑S�̐l��[psn]
	//
	
	//�V�~�����[�V��������ϐ�
	mf_cal_interval=		1.0/30;			//��	//�v�Z�s�b�`����[sec]	�P�J�E���g�Ői�ގ���
	m_counta=				0;				//���������
	m_genzai_jikoku_sec=	0.0;			//����[sec]

	//�`��֘A
	m_hwnd=					NULL;			//�\������E�C���h�E�n���h���iwindow�̎��ʕϐ��j
	m_disp_vector_sw=		0;				//�����x�N�g���̕\����\��

	//�ŏ��̃E�B���h�E�T�C�Y
	HWND	desktop_hwnd=	GetDesktopWindow();			//�f�X�N�g�b�v�̃E�C���h�E�n���h��
	RECT	rect;
	GetClientRect( desktop_hwnd , &rect );				//�E�B���h�E�̕`��͈͂̃T�C�Y�擾
	m_disp_ratio=			( rect.right - rect.left ) * 0.95 / m_field_length ;				//20170629
	m_disp_offset_y=		( rect.bottom - rect.top ) / 2 ;

	//�e�l�̑������Z�b�g
	double	pos_y;
	m_man_ptr=				(S_MAN*)calloc( m_man_max , sizeof( S_MAN ) );		//�e�l�̑����i�l�����m�ۂ���j
	//�S���̃��[�v
	for(long nn=0;nn<m_man_max;nn++){
		S_MAN*	pman=	&m_man_ptr[nn];					//n�Ԗڂ̐l�N���X�̐l�̃|�C���^
		//�t�@�W�B�[�@���x
		pman->body_haba=			0.2;			//���l�̔��a[m]
		pman->m_walk_speed=			1.0;			//�����s���x[m/s]
		pman->m_zenpou_angle=		80.0;			//���O���ɂ���Ɣ��f�ł��鎋��p[deg]
		//�t�@�W�B�[�@�����o�[�V�b�v
		//�����߂�
		pman->near_start_pos=		1.0;//[m]		//���߂��Ɗ����n�߂鋗��[m]�i���S�ԋ�������A�̔��a���������A�󌄋����j
		pman->near_end_pos=			0.1;//[m]		//����΋߂��Ɗ����鋗��[m]
		//����
		pman->far_start_pos=		0.5;//[m]		//�������Ɗ����n�߂鋗��[m]�i���S�ԋ�������A�̔��a���������A�󌄋����j
		pman->far_end_pos=			2.0;//[m]		//����Ή����Ɗ����鋗��[m]
//		pman->far_end_pos=			5.0;//[m]		//����Ή����Ɗ����鋗��[m]

		//�����������Ă���
		pman->kuru_start_pos=		60.0;//[deg]	//���u�������Ă���v�Ɗ����͂��߂鑊��̊�̊p�x[deg]
		pman->kuru_end_pos=			15.0;//[deg]	//����΁u�������Ă���v�Ɗ����鑊��̊�̊p�x[deg]
		//pman->kuru_start_pos=		45.0;//[deg]	//���u�������Ă���v�Ɗ����͂��߂�p�x[deg]
		//pman->kuru_end_pos=			15.0;//[deg]	//����΁u�������Ă���v�Ɗ�����p�x[deg]
		//�������Ă��Ȃ�
		pman->konai_start_pos=		45.0;//[deg]	//���u��������v�Ɗ����͂��߂�p�x[deg]
		pman->konai_end_pos=		90.0;//[deg]	//����΁u��������v�Ɗ�����p�x[deg]
		//pman->konai_start_pos=		45.0;//[deg]	//���u��������v�Ɗ����͂��߂�p�x[deg]
		//pman->konai_end_pos=		110.0;//[deg]	//����΁u��������v�Ɗ�����p�x[deg]

		//�ӎ����x
		pman->near_kuru_base=		1.0;			//���߂��E�߂Â��@�ꍇ�̌W��
		pman->near_konai_base=		0.5;			//���߂��E��������@�ꍇ�̌W��
		pman->fars_kuru_base=		0.2;			//�������E�߂Â��@�ꍇ�̌W��
		pman->fars_konai_base=		0.0;			//�������E��������@�ꍇ�̌W��
		//
//���E2�`�[���ɕ�����
		long	team=				( nn % 2 );							//�����Ɗ�ŕ�����
		switch( team ){
			case 0:{				//�����`�[��
				//�x���W���v�Z
				long	tiem_i=					nn / 2 ;				//�`�[�����̔ԍ�0,1,2,3,
				pos_y=	((tiem_i+1)/2) * m_field_width / m_man_max;		//�x���W�͓�l����Βl�������ɂȂ�
				if( ( tiem_i % 2 ) == 0 ) pos_y=	-pos_y;				//�����Ԗڂ̐l�͏㑤�ɔz�u

				//�ŏ��̈ʒu
				pman->m_now_pos.x=				0;						//�����͍�
				pman->m_now_pos.y=				-pos_y;					//�������ς��ɐ���@�Ȃ̂Ł@�ŏ��͏d�Ȃ��Ă���

				pman->m_color_code=				5;	//���F				//�~�̐F�ԍ�
				pman->m_brush_code=				5;	//���F				//�h��Ԃ��̐F�ԍ�
				//�ړI�̈ʒu
				pman->m_now_target_pos.x=		m_field_length;			//�����ʒu�̔��Α�
				pman->m_now_target_pos.y=		0;						//���̒��S�ʒu
			}break;

			case 1:{				//�E���`�[��
				//�x���W�͍��`�[���Ɠ���
				//�ŏ��̈ʒu
				pman->m_now_pos.x=				m_field_length;			//��͉E
				pman->m_now_pos.y=				-pos_y;					//�������ς��ɐ���@�Ȃ̂Ł@�ŏ��͏d�Ȃ��Ă���

				pman->m_color_code=				6;	//���F				//�~�̐F�ԍ�
				pman->m_brush_code=				0;	//��				//�h��Ԃ��̐F�ԍ�
				//�ړI�̈ʒu
				pman->m_now_target_pos.x=		0;						//�����ʒu�̔��Α�
				pman->m_now_target_pos.y=		0;						//���̒��S�ʒu
			}break;

		}
		//
		xfunc_unit_vect_pp( pman->m_now_pos , pman->m_now_target_pos , &pman->m_now_target_dir );		//�ŏ��̐i�s�����i�P�ʃx�N�g���j
		pman->m_now_target_angle=		xfunc_kakudo( 0,0 , pman->m_now_target_dir.x , pman->m_now_target_dir.y , NULL , NULL );							//�i�s�����p�x[deg]
	}
}


//------------------------------------------------------------------------------
//		�f�X�g���N�^�i�j�����鎞�̏����j
//------------------------------------------------------------------------------
x_simuA::~x_simuA()
{
	free( m_man_ptr );							//�e�l�̑����̔j��
}




//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//		�`��i���ŏ����ĕ\�ɏo���j
//------------------------------------------------------------------------------
void	x_simuA::disp_simu()
{
	//�쐬����r�b�g�}�b�v�̃T�C�Y
	RECT	rect;
	GetWindowRect( m_hwnd , &rect );																//�E�B���h�E�̃T�C�Y���擾
	long	width=				rect.right - rect.left;
	long	height=				rect.bottom - rect.top;
	
	//�r�b�g�}�b�v�̐���
	HDC		src_hdc=			GetDC( m_hwnd );													//�\�̂g�c�b���擾
	HBITMAP	mem_bitmap=			CreateCompatibleBitmap( src_hdc , width , height ) ;				//�r�b�g�}�b�v���쐬
	HDC		m_Mem_hdc=			CreateCompatibleDC( src_hdc ) ;										//���c�b�𐶐�
	HBITMAP	m_old_Mem_bitmap=	(HBITMAP)SelectObject( m_Mem_hdc , mem_bitmap ) ;					//���c�b�Ƀr�b�g�}�b�v�������N
	//

	//�y���ƃu���V�����i���Ԃ̂����鏈���Ȃ̂Œ�����Ȃ��j
	long	rgb[8]=				{0 , RGB(0,0,255) , RGB(255,0,0) , RGB(255,0,255) , RGB(0,255,0) , RGB(0,255,255) , RGB(255,255,0) , RGB(255,255,255)};	//��{��7�F
	m_color_pen[0]=				(HPEN)GetStockObject( NULL_PEN );								//�����ȃy��
	m_color_brush[0]=			(HBRUSH)GetStockObject( NULL_BRUSH );							//�����ȃu���V
	for( long ii=1;ii<8;ii++){
		m_color_pen[ii]=		CreatePen( PS_SOLID , 1 , rgb[ii] );							//�y��
		m_color_brush[ii]=		CreateSolidBrush( rgb[ii] );									//�u���V
	}
	//
	HBRUSH	old_brush=			(HBRUSH)SelectObject( m_Mem_hdc , GetStockObject( NULL_BRUSH ) );	//�Ƃ肠���������ȃu���V��I���iHDC�͑I�����ꂽ�u���V�����b�N���Ă��܂��̂ŁA�Ō�Ɍ��ɖ߂����߁j
	HPEN	old_pen=			(HPEN)SelectObject( m_Mem_hdc , m_color_pen[0] );					//�Ƃ肠�����𓌖��y����I���iHDC�͑I�����ꂽ�y�������b�N���Ă��܂��̂ŁA�Ō�Ɍ��ɖ߂����߁j
	//
	
	//�S���`��
	for(long ii=0;ii<m_man_max;ii++){																//�S�����[�v
		disp_man_one( m_Mem_hdc , ii );																//��l�Â`��
	}
	//	
	
	//�󋵕�����̕\��
	double	cal_sokudo=			m_genzai_jikoku_sec * 1000.0 / (double)( timeGetTime() - m_simu_start_time - m_simu_tyuudann_time );		//�v�Z���x
	char	wwc[200];
	sprintf( wwc , "A %6.1f[�b](%5.3f�{)%5.3f[�b/�X�e�b�v] %d[�l] W:�x�N�g���@S:��~/�ĊJ�@R:���߂���" , m_genzai_jikoku_sec , cal_sokudo , mf_cal_interval , m_man_max );	//�o�ߎ��ԂƐl��
	xfunc_print_text( m_Mem_hdc , wwc , 0 , 0 , 32 , RGB(255,255,255) , "�l�r �S�V�b�N" , -1, 0.0 , 0 );
	
	//���ŏ������r�b�g�}�b�v��\�ɓ]��
	BitBlt( src_hdc , 0 , 0 , width , height , m_Mem_hdc , 0 , 0 , SRCCOPY );						//�\�ɏo��


	//���\�[�X�̊J��
	SelectObject( m_Mem_hdc , old_pen );															//�u���V�����ɖ߂�
	SelectObject( m_Mem_hdc , old_brush );															//�y�������ɖ߂�
	//�O���t�B�b�N���\�[�X�̔j��
	for(long ii=0;ii<8;ii++){
		DeleteObject( m_color_pen[ii] );															//�y����j��
		DeleteObject( m_color_brush[ii] );															//�u���V��j��
	}
	//
	
	//�f�o�C�X�̐؂藣��
	ReleaseDC( m_hwnd , src_hdc ) ;																	//src_hdc��m_hwn����؂藣���i�@GetDC( m_hwnd )�Ƒ΁@�j

	//�r�b�g�}�b�v�ƃf�o�C�X�R���e�L�X�g�̔j��
	SelectObject( m_Mem_hdc , m_old_Mem_bitmap ) ;													//���̃r�b�g�}�b�v�ɖ߂�
	DeleteObject( mem_bitmap );																		//���̃r�b�g�}�b�v��j��
	DeleteDC( m_Mem_hdc );																			//���̂c�b��j��
}



//------------------------------------------------------------------------------
//		�\��
//------------------------------------------------------------------------------
long x_simuA::disp_man_one( HDC hdc , long index )
{

	S_MAN*	p_man=		&m_man_ptr[index];
	//
	dPOINT	center;
	real_to_view( p_man->m_now_pos.x , p_man->m_now_pos.y , &center.x , &center.y ) ;															//���S���W
	dPOINT	left_top;
	real_to_view( p_man->m_now_pos.x - p_man->body_haba , p_man->m_now_pos.y - p_man->body_haba , &left_top.x , &left_top.y ) ;					//����
	dPOINT	right_bottom;
	real_to_view( p_man->m_now_pos.x + p_man->body_haba , p_man->m_now_pos.y + p_man->body_haba , &right_bottom.x , &right_bottom.y ) ;			//�E��
	
	//�l�̖{��
	SelectObject( hdc , m_color_pen[p_man->m_color_code] );																						//�w��E�ŉ~��`��
	SelectObject( hdc , m_color_brush[p_man->m_brush_code] );																					//�w��E�œh��Ԃ�
	Ellipse( hdc , (long)(left_top.x + 0.5 ) , (long)(left_top.y  + 0.5 ), (long)(right_bottom.x + 0.5 ) , (long)(right_bottom.y + 0.5 ) );		//�~��`��
	
	//�����x�N�g���̕`��
	if( m_disp_vector_sw == 1 ){
		//������`��  //fuzzy
		double	bairitu=		1;
		//
		dPOINT	target;
		bairitu=		p_man->m_walk_speed;			//���s���x��{���ɂ���
		real_to_view( p_man->m_now_pos.x + p_man->m_next_dir.x * bairitu , p_man->m_now_pos.y+ p_man->m_next_dir.y * bairitu , &target.x , &target.y ) ;			//���������`��
		SelectObject( hdc , m_color_pen[7] );
		MoveToEx( hdc , (long)(center.x + 0.5 ) , (long)(center.y + 0.5 ) , NULL );
		LineTo( hdc , (long)(target.x + 0.5 ) , (long)(target.y + 0.5 ) );
		//
		dPOINT	fdir;
		real_to_view( p_man->m_now_pos.x + p_man->m_sum_fuzzy_vect.x * bairitu , p_man->m_now_pos.y+ p_man->m_sum_fuzzy_vect.y * bairitu , &fdir.x , &fdir.y ) ;		//�t�@�W�B�[�͂�`��
		SelectObject( hdc , m_color_pen[6] );
		MoveToEx( hdc , (long)(center.x + 0.5 ) , (long)(center.y + 0.5 ) , NULL );
		LineTo( hdc , (long)(fdir.x + 0.5 ) , (long)(fdir.y + 0.5 ) );
		//
		dPOINT	deside;
		real_to_view( p_man->m_now_pos.x + p_man->m_now_target_dir.x * bairitu , p_man->m_now_pos.y+ p_man->m_now_target_dir.y * bairitu , &deside.x , &deside.y ) ;	//�^�[�Q�b�g������`��
		SelectObject( hdc , m_color_pen[2] );
		MoveToEx( hdc , (long)(center.x + 0.5 ) , (long)(center.y + 0.5 ) , NULL );
		LineTo( hdc , (long)(deside.x + 0.5 ) , (long)(deside.y + 0.5 ) );
	}

	return 1 ;
}


//-------------------------------------------------------------------------------------
//		�V�~�����[�V�����p�����W���X�N���[�����W�ɕϊ�
//-------------------------------------------------------------------------------------
void x_simuA::real_to_view( double rx1 , double ry1 , double* vx1 , double* vy1 )
{
	*vx1=  rx1 * m_disp_ratio;
	*vy1= -ry1 * m_disp_ratio + m_disp_offset_y;
}


//-------------------------------------------------------------------------------------
//		�X�N���[�����W���V�~�����[�V�����p�����W�ɕϊ�
//-------------------------------------------------------------------------------------
void x_simuA::view_to_real( double vx1 , double vy1 , double* rx1 , double* ry1 )
{
	*rx1=	 vx1 / m_disp_ratio ;
	*ry1=	-(vy1 - m_disp_offset_y) / m_disp_ratio ;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//		�S���̈ړ������@�P�X�e�b�v���s
//------------------------------------------------------------------------------
long	x_simuA::next_step_all()
{
	for(long ii=0;ii<m_man_max;ii++){
		next_step_one( ii );			//���̒��ɂ�	for(long ii=0;ii<m_man_max;ii++){}�@������B�������ő�S��Ăяo��
														//100�l�Ȃ��  100�~100�~4
														//1000�l�Ȃ�� 1000�~1000�~4 �ƂȂ�A�l����2��Ōv�Z�ʂ�������B
	}
	return 0;
}


//------------------------------------------------------------------------------
//		��l�̈ړ�
//------------------------------------------------------------------------------
long x_simuA::next_step_one( long index )
{
	//
	make_fuzzy_new_vector(index);						//�t�@�W�B�x�N�g���̌v�Z
	//
	S_MAN*	p_man=		&m_man_ptr[index];
	//�ڕW�܂ł̒P�ʃx�N�g��
	xfunc_unit_vect_pp( p_man->m_now_pos , p_man->m_now_target_pos , &p_man->m_now_target_dir );		//�ړI�n�_�����x�N�g��
	//�t�@�W�B�ŏC�� //fuzzy
	dPOINT	fuzzy_dd=	p_man->m_now_target_dir + p_man->m_sum_fuzzy_vect;							//�ړI�����x�N�g���ƃt�@�W�B�����x�N�g���̘a
	dPOINT	first_next_dir;
	xfunc_unit_vect( 0 , 0 , fuzzy_dd.x , fuzzy_dd.y , &first_next_dir.x , &first_next_dir.y );		//�ŏ��̐i�ޕ�����P�ʃx�N�g����
	p_man->m_next_dir=	first_next_dir;															//���ɐi�ޕ����ɐݒ�
	long	next_ok=	check_next_position( index ) ;											//�i�ޕ����ɐl�����邩
	if( next_ok == FALSE ){																		//�i�ޕ����ɐl��������
		//�E�ɐi��
		p_man->m_next_dir.x=		 first_next_dir.y;												//�E�ɐi��
		p_man->m_next_dir.y=		-first_next_dir.x;
		next_ok=	check_next_position( index ) ;												//�i�ޕ����ɐl�����邩
		if( next_ok == FALSE ){
			//���ɐi��
			p_man->m_next_dir.x=	-first_next_dir.y;												//���ɐi��
			p_man->m_next_dir.y=	 first_next_dir.x;
			next_ok=	check_next_position( index ) ;											//�i�ޕ����ɐl�����邩
			if( next_ok == FALSE ){
				//���ɐi��
				p_man->m_next_dir.x=	-first_next_dir.x;											//���ɐi��
				p_man->m_next_dir.y=	-first_next_dir.y;
				next_ok=	check_next_position( index ) ;										//�i�ޕ����ɐl�����邩
				if( next_ok == FALSE ){
					return FALSE;																//�ړ�����߂�
				}
			}
		}
	}
	//
	p_man->m_now_pos=				p_man->m_now_pos + p_man->m_next_dir * p_man->m_walk_speed * mf_cal_interval;		//���̃X�e�b�v�̒n�_�Ɉړ�
	//�ړ���̖ړI�|�C���g�����̍X�V
	xfunc_unit_vect_pp( p_man->m_now_pos , p_man->m_now_target_pos , &p_man->m_now_target_dir );		//�ŏ��̐i�s�����i�P�ʃx�N�g���j
	p_man->m_now_target_angle=		xfunc_kakudo( 0,0 , p_man->m_now_target_dir.x , p_man->m_now_target_dir.y , NULL , NULL );															//�i�s�����p�x[deg]
	//
	return TRUE;
}


//------------------------------------------------------------------------------
//		���̈ʒu�Ɉړ��\���ǂ����`�F�b�N
//------------------------------------------------------------------------------
long	x_simuA::check_next_position( long index )
{
	S_MAN*	a_san=		&m_man_ptr[index];
	dPOINT	next_pos=	a_san->m_now_pos + a_san->m_next_dir * a_san->m_walk_speed * mf_cal_interval;		//���̈ʒu
	long	over_sw=	TRUE;
	for(long ii=0;ii<m_man_max;ii++){												//�S�����`�F�b�N
		if( ii == index ) continue;													//�����͖���
		S_MAN* b_san=			&m_man_ptr[ii];
		double	next_dist=		xfunc_dist_pp( b_san->m_now_pos , next_pos );			//���̋���[m]
		if( next_dist > ( a_san->body_haba + b_san->body_haba ) ) continue;			//�ڐG���Ȃ�
		//
		double	now_dist=		xfunc_dist_pp( b_san->m_now_pos , a_san->m_now_pos );	//���݂̋���[m]
		if( next_dist > now_dist ) continue;										//�ڐG���Ă��Ă�������������ւ͈ړ�������
		//
		over_sw=		FALSE;														//�ړ��͂ł��Ȃ�
		break;
	}
	return over_sw;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//		�t�@�W�B�[�ŉ���x�N�g�����v�Z
//	����܂ł̋����@�Ɓ@����̌����@���g����
//	�����̈ӎ��̒��x�@���@���肵
//	�i�s�����ƒ�������x�N�g�����쐬����
//------------------------------------------------------------------------------
long	x_simuA::make_fuzzy_new_vector( long index )
{

	S_MAN*	a_san=		&m_man_ptr[index];			//����
	a_san->m_sum_fuzzy_vect=		0.0;				//�t�@�W�B���_�̌��ʂ����v�����l�@(���͂ɉe������l�����Ȃ������ꍇ�̓[���j
	//
	if( a_san->m_walk_speed <= 0.0 ){				//�����Ȃ����͌v�Z�͂���Ȃ�
		return FALSE;								//���ʂȂ�
	}
	//
	//�t�@�W�B�[��
	dPOINT	sum_fuzzy_dd=			{0.0,0.0};		//�t�@�W�B���_�̌��ʂ����v���邽�߂̕ϐ�
	long	sum_man=				0;				//�e�����󂯂��l��
	//
	for(long ii=0;ii<m_man_max;ii++){																//���S�����`�F�b�N
		if( ii == index ) continue;																	//�����͖�������
		//
		S_MAN* b_san=		&m_man_ptr[ii];															//����i�a����j�̑����̍\���̃|�C���^
		//�O���ɋ��Ȃ��l�͖���
		double	a_b_angle=			xfunc_kakudo( a_san->m_now_pos.x , a_san->m_now_pos.y , b_san->m_now_pos.x , b_san->m_now_pos.y , NULL,NULL);	//�`���猩���a�̕���[deg]
		double	a_mesen_b_angle=	xfunc_angle_pm180( a_b_angle - a_san->m_now_target_angle ) ;	//�`�̐i�s��������ɂ����a�̕���[deg]
		if( fabs( a_mesen_b_angle ) > a_san->m_zenpou_angle ) continue ;							//���O���ɋ��Ȃ��l�͎��E�ɓ���Ȃ��Ƃ��Ė�������

		//�e���𐄑�
		double	fuzzy_v=			fuzzy_suiron( index , ii );										//�t�@�W�B���_
		if( fuzzy_v <= 0.0 ) continue;																//�e���̂Ȃ��l�͖���
		//
		sum_man++;																					//�e�������l��
		//�t�@�W�B�ɂ�������x�̏W�v(�l�����ł��낢��o����)�����ł͖ړI�����ɑ΂�������������ɍ�p����ƍl����B
		{
			//����������@�����̌����ƒ��s����
			double	aite_side=	a_mesen_b_angle ;													//�i�s��������ɂ����������
			double	fuzzy_angle ;
			if( aite_side > 0.0 ){																	//����͍����ɂ���
				fuzzy_angle=	xfunc_angle360( a_san->m_now_target_angle - 90.0 );					//�E���ɓ�����
			}else{																					//����͉E���ɂ���
				fuzzy_angle=	xfunc_angle360( a_san->m_now_target_angle + 90.0 );					//�����ɓ�����
			}
			//�w�x�����ŋL�^�iaite_side�Ő��������߂�fuzzy_v�����v���Ă����čŌ��sum_fuzzy_dd���v�Z���Ă������j
			double fuzzy_dx=		fuzzy_v * cos( fuzzy_angle / 180.0 * u_PAI );
			double fuzzy_dy=		fuzzy_v * sin( fuzzy_angle / 180.0 * u_PAI );
			sum_fuzzy_dd.x+=		fuzzy_dx;
			sum_fuzzy_dd.y+=		fuzzy_dy;

		}
	}
	//
	if( sum_man > 0 ){								//���͂ɉe������l�������ꍇ�i�i�s�����ɒ��s����x�N�g���ɂȂ�j
		a_san->m_sum_fuzzy_vect=		sum_fuzzy_dd;
	}
	//
	return TRUE ;									//���ʂ���
}


//-------------------------------------------------------------------------------------
//		���肩��󂯂�e���̒��x
//-------------------------------------------------------------------------------------
double x_simuA::fuzzy_suiron(long my_index , long aite_index )
{
	S_MAN* a_san=				&m_man_ptr[my_index];												//�����i�`����j�̑����̍\���̃|�C���^
	S_MAN* b_san=				&m_man_ptr[aite_index];												//����i�a����j�̑����̍\���̃|�C���^

	//����
	double	body_size2=			a_san->body_haba + b_san->body_haba ;								//�����Ƒ���̔��a�̘a�����S�ԋ����̌��E����[m]
	double	now_dist=			xfunc_dist_pp( b_san->m_now_pos , a_san->m_now_pos ) - body_size2 ;	//���݂̊ԋ���[m]
	if( now_dist > a_san->far_end_pos )  return 0.0 ;												//��Ή����Ɗ����鋗����艓���l�͖�������

	//���肩�猩�������̊p�x
	double	b_a_angle=			xfunc_kakudo_pp( b_san->m_now_pos , a_san->m_now_pos , NULL,NULL);	//�a���猩���`�̕���[deg]
	double	b_mesen_a_angle=	xfunc_angle_pm180( b_a_angle - b_san->m_now_target_angle );			//�a�̐i�s��������ɂ����`�̕���[deg]
	double	check_angle=		fabs(b_mesen_a_angle);

	//�O���Ō������Ȃ��ʒu�֌W�͖���
	double	a_b_angle=			xfunc_kakudo_pp( a_san->m_now_pos , b_san->m_now_pos , NULL,NULL);	//�`���猩���a�̕���[deg]
	double	a_mesen_b_angle=	xfunc_angle_pm180( a_b_angle - a_san->m_now_target_angle ) ;		//�`�̐i�s��������ɂ����a�̕���[deg]
	double	closs_angle=		fabs( b_mesen_a_angle + a_mesen_b_angle );							//�p�x�̍��v�̐�Βl(0-360)[deg]
	if( closs_angle > 180.0 ) return 0.0;															//�O���Ō������Ȃ��ʒu�֌W

	//�t�@�W�B�^���l
	double	f_nears=			xfunc_fuzzy_strong( now_dist	, a_san->near_start_pos		, a_san->near_end_pos	) ;	//�߂��Ɗ�������x
	double	f_farss=			xfunc_fuzzy_strong( now_dist	, a_san->far_start_pos		, a_san->far_end_pos	) ;	//�����Ɗ�������x
	double	f_kuru=				xfunc_fuzzy_strong( check_angle	, a_san->kuru_start_pos		, a_san->kuru_end_pos	) ;	//�������ė���Ɗ�������x
	double	f_konai=			xfunc_fuzzy_strong( check_angle	, a_san->konai_start_pos	, a_san->konai_end_pos	) ;	//��������Ɗ�������x

	//��`�̖ʐ�
	double	f_nears_kuru=		xfunc_daikei_area( f_nears * f_kuru		, 1 , 1 ) ;				//�߂��E�߂Â�
	double	f_nears_konai=		xfunc_daikei_area( f_nears * f_konai	, 1 , 1 ) ;				//�߂��E��������
	double	f_farss_kuru=		xfunc_daikei_area( f_farss * f_kuru		, 1 , 1 ) ;				//�����E�߂Â�
	double	f_farss_konai=		xfunc_daikei_area( f_farss * f_konai	, 1 , 1 ) ;				//�����E��������
		
	//���_���ʁi4�̑�`�̏d�S�ʒu�j���{�됰�́u�㐔��-���Z-�d�S�@�v�̒��̈��	<=	�}���_�j�搶�́umin-max-�d�S�@�v�ɕς�鐄�_���@
	double	fuzzy_v= (  f_nears_kuru  * a_san->near_kuru_base + f_nears_konai * a_san->near_konai_base
					  + f_farss_kuru  * a_san->fars_kuru_base + f_farss_konai * a_san->fars_konai_base )
					/ ( f_nears_kuru + f_nears_konai + f_farss_kuru + f_farss_konai ) ;
	//
	return fuzzy_v;
}



//----------------------------------------------------------------------------
//		��`�̖ʐ�
//----------------------------------------------------------------------------
double	 x_simuA::xfunc_daikei_area(double height, double bottom_length , double top_height )
{
	double	area=		( bottom_length * ((top_height - height) / top_height) + bottom_length ) * height / 2.0 ;
	return area ;
}


//----------------------------------------------------------------------------
//		�t�@�W�B�^���l
//		start_pos�ȉ��Ȃ�U�i�l0)	end_pos�ȏ�Ȃ�^(�l1)
//----------------------------------------------------------------------------
double	 x_simuA::xfunc_fuzzy_strong( double val , double start_pos , double end_pos )
{
	double	strongness=		0;
	if( start_pos < end_pos ){		//�J�n���I����菬����		start_pos____/~~~~~end_pos
		if( val < start_pos ){
			strongness=		0 ;
		}else if( val < end_pos ){
			strongness=		(val - start_pos) / (end_pos - start_pos);		//0-1�̒l
		}else{
			strongness=		1;
		}
	}else{							//�J�n���I�����傫��		end_pos~~~~\______start_pos
		if( val < end_pos ){
			strongness=		1;
		}else if( val < start_pos ){
			strongness=		(start_pos - val) / (start_pos - end_pos);		//0-1�̒l
		}else{
			strongness=		0;
		}
	}
	return strongness;
}

