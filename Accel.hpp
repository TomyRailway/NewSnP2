// 汎用加速度計算機
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
	int* Time; // 現在時刻
	float* TrainSpeed; // 速度計の速度[km/h]
	int* DeltaT; // フレーム時間[ms/frame]

	float accel_kmhs; // 加速度[km/h/s]

	// Initalizeで実行します
	void initialize(void)
	{
		accel_kmhs = 0;
	}

	// Elapseで実行します
	void execute(void)
	{
		float speed = fabsf(*TrainSpeed); // 速度の絶対値[km/h]
		int flame_time = *Time - delta_time;

		if (flame_time != 0 && speed != 0) {
			accel_kmhs = (*TrainSpeed - delta_speed) / flame_time * 1000;
		}

		delta_speed = *TrainSpeed;
		delta_time = *Time;
	}
};	// CAccel

#endif	// ATS_ACCEL_HPP_INCLUDED