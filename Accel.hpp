// �ėp�����x�v�Z�@
// Copyright (C) 2024 TomyRailway

#ifndef ATS_ACCEL_HPP_INCLUDED
#define ATS_ACCEL_HPP_INCLUDED

#include <math.h>

class CAccel
{
private:
	float delta_speed;
	int delta_time;

public:
	int* Time; // ���ݎ���
	float* TrainSpeed; // ���x�v�̑��x[km/h]
	int* DeltaT; // �t���[������[ms/frame]

	float accel_kmhs; // �����x[km/h/s]

	// Initalize�Ŏ��s���܂�
	void initialize(void)
	{
		accel_kmhs = 0;
	}

	// Elapse�Ŏ��s���܂�
	void execute(void)
	{
		float speed = fabsf(*TrainSpeed); // ���x�̐�Βl[km/h]
		int flame_time = *Time - delta_time;

		if (flame_time != 0 && speed != 0) {
			accel_kmhs = (*TrainSpeed - delta_speed) / flame_time * 1000;
		}

		delta_speed = *TrainSpeed;
		delta_time = *Time;
	}
};	// CAccel

#endif	// ATS_ACCEL_HPP_INCLUDED